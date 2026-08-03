#include "mainwindow.h"
#include "hexeditor.h"
#include "firmwareexplorer.h"
#include "hexis_models.h"
#include "hexis_patch.h"
#include "hexis_driver.h"
#include "hexis_chip.h"
#include <QDockWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QTabWidget>
#include <QTimer>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_tabWidget(nullptr), m_hexEditor(nullptr), m_fwExplorer(nullptr),
      m_infoTable(nullptr), m_consoleText(nullptr), m_driver(nullptr), m_ctx(nullptr)
{
    setWindowTitle("Hexis Hardware Pentesting Framework");
    resize(1200, 800);
    
    setupDashboard();
    setupDocks();
    
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::updateStatus);
    m_timer->start(100);
}

MainWindow::~MainWindow()
{
    if (m_ctx && m_driver) {
        m_driver->disconnect(m_ctx);
    }
    if (m_hexEditor && m_hexEditor->firmware()) {
        hexis_firmware_free(m_hexEditor->firmware());
    }
}

void MainWindow::setupDashboard()
{
    m_tabWidget = new QTabWidget(this);
    
    QWidget* dashWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(dashWidget);
    
    QLabel* header = new QLabel("<h2>Dashboard</h2>");
    header->setAlignment(Qt::AlignCenter);
    layout->addWidget(header);
    
    m_infoTable = new QTableWidget(6, 2);
    m_infoTable->setHorizontalHeaderLabels({"Property", "Value"});
    m_infoTable->horizontalHeader()->setStretchLastSection(true);
    m_infoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_infoTable->setSelectionMode(QAbstractItemView::NoSelection);
    
    m_infoTable->setItem(0, 0, new QTableWidgetItem("Programmer"));
    m_infoTable->setItem(0, 1, new QTableWidgetItem("Disconnected"));
    m_infoTable->setItem(1, 0, new QTableWidgetItem("Driver"));
    m_infoTable->setItem(1, 1, new QTableWidgetItem("None"));
    m_infoTable->setItem(2, 0, new QTableWidgetItem("Chip"));
    m_infoTable->setItem(2, 1, new QTableWidgetItem("Unknown"));
    m_infoTable->setItem(3, 0, new QTableWidgetItem("Capacity"));
    m_infoTable->setItem(3, 1, new QTableWidgetItem("0 bytes"));
    m_infoTable->setItem(4, 0, new QTableWidgetItem("Voltage"));
    m_infoTable->setItem(4, 1, new QTableWidgetItem("0.0V"));
    m_infoTable->setItem(5, 0, new QTableWidgetItem("Status"));
    m_infoTable->setItem(5, 1, new QTableWidgetItem("Idle"));
    
    layout->addWidget(m_infoTable);
    
    QWidget* btnWidget = new QWidget();
    QHBoxLayout* btnLayout = new QHBoxLayout(btnWidget);
    
    QPushButton* btnConnect = new QPushButton("Connect");
    QPushButton* btnRead = new QPushButton("Read");
    QPushButton* btnWrite = new QPushButton("Write");
    QPushButton* btnErase = new QPushButton("Erase");
    QPushButton* btnVerify = new QPushButton("Verify");
    
    connect(btnConnect, &QPushButton::clicked, this, &MainWindow::onConnect);
    connect(btnRead, &QPushButton::clicked, this, &MainWindow::onRead);
    connect(btnWrite, &QPushButton::clicked, this, &MainWindow::onWrite);
    connect(btnErase, &QPushButton::clicked, this, &MainWindow::onErase);
    connect(btnVerify, &QPushButton::clicked, this, &MainWindow::onVerify);
    
    btnLayout->addWidget(btnConnect);
    btnLayout->addWidget(btnRead);
    btnLayout->addWidget(btnWrite);
    btnLayout->addWidget(btnErase);
    btnLayout->addWidget(btnVerify);
    
    layout->addWidget(btnWidget);
    
    m_hexEditor = new HexEditor();
    m_fwExplorer = new FirmwareExplorer();
    
    m_tabWidget->addTab(dashWidget, "Dashboard");
    m_tabWidget->addTab(m_hexEditor, "Hex Editor");
    m_tabWidget->addTab(m_fwExplorer, "Firmware Explorer");
    
    setCentralWidget(m_tabWidget);
}

void MainWindow::setupDocks()
{
    QDockWidget *consoleDock = new QDockWidget("Console", this);
    consoleDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::RightDockWidgetArea);
    m_consoleText = new QTextEdit();
    m_consoleText->setReadOnly(true);
    m_consoleText->setText("Hexis Core Initialized.\nReady.");
    consoleDock->setWidget(m_consoleText);
    addDockWidget(Qt::BottomDockWidgetArea, consoleDock);
}

void MainWindow::logMessage(const QString& msg)
{
    if (m_consoleText) {
        m_consoleText->append(msg);
    }
}

void MainWindow::onConnect()
{
    m_driver = hexis_driver_get("ch341a");
    if (!m_driver) {
        logMessage("Error: ch341a driver not found.");
        return;
    }
    
    int res = m_driver->connect(&m_ctx);
    if (res != 0) {
        logMessage(QString("Failed to connect: Error %1").arg(res));
        return;
    }
    
    hexis_chip_info_t chip_info;
    res = m_driver->probe(m_ctx, &chip_info);
    if (res != 0) {
        logMessage("Failed to probe chip.");
        m_driver->disconnect(m_ctx);
        m_ctx = nullptr;
        return;
    }
    
    m_infoTable->item(0, 1)->setText("Connected");
    m_infoTable->item(1, 1)->setText(m_driver->name);
    m_infoTable->item(2, 1)->setText(QString("%1 %2").arg(chip_info.manufacturer, chip_info.model));
    m_infoTable->item(3, 1)->setText(QString("%1 bytes").arg(chip_info.capacity_bytes));
    
    logMessage(QString("Connected to %1 %2 (Capacity: %3 bytes)")
               .arg(chip_info.manufacturer, chip_info.model)
               .arg(chip_info.capacity_bytes));
}

void MainWindow::onRead()
{
    if (!m_ctx || !m_driver) {
        logMessage("Not connected to programmer.");
        return;
    }
    hexis_chip_info_t chip_info;
    m_driver->probe(m_ctx, &chip_info);
    
    HexisFirmware* fw = hexis_firmware_create(chip_info.capacity_bytes);
    if (!fw) return;
    
    uint8_t* buffer = (uint8_t*)malloc(chip_info.capacity_bytes);
    logMessage(QString("Reading %1 bytes...").arg(chip_info.capacity_bytes));
    
    int res = m_driver->read(m_ctx, 0, buffer, chip_info.capacity_bytes);
    if (res == 0) {
        hexis_firmware_write(fw, 0, buffer, chip_info.capacity_bytes);
        m_hexEditor->setFirmware(fw);
        m_fwExplorer->setFirmware(fw);
        logMessage("Read successful.");
    } else {
        logMessage("Read failed.");
        hexis_firmware_free(fw);
    }
    free(buffer);
}

void MainWindow::onWrite()
{
    logMessage("Write functionality invoked (Requires confirmation layer).");
}

void MainWindow::onErase()
{
    if (!m_ctx || !m_driver) {
        logMessage("Not connected to programmer.");
        return;
    }
    
    hexis_chip_info_t chip_info;
    m_driver->probe(m_ctx, &chip_info);
    logMessage(QString("Erasing %1 bytes...").arg(chip_info.capacity_bytes));
    
    int res = m_driver->erase(m_ctx, 0, chip_info.capacity_bytes);
    if (res == 0) {
        logMessage("Erase successful.");
    } else {
        logMessage("Erase failed.");
    }
}

void MainWindow::onVerify()
{
    logMessage("Verify functionality invoked.");
}

void MainWindow::updateStatus()
{
    // Fetch background job events here.
}
