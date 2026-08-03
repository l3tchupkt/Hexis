#include "mainwindow.h"
#include "hexeditor.h"
#include "firmwareexplorer.h"
#include "hexis_models.h"
#include "hexis_patch.h"
#include <QDockWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QTabWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_tabWidget(nullptr), m_hexEditor(nullptr), m_fwExplorer(nullptr)
{
    setWindowTitle("Hexis Hardware Hacking Framework");
    resize(1200, 800);
    
    setupDashboard();
    setupDocks();
}

MainWindow::~MainWindow()
{
    if (m_hexEditor && m_hexEditor->firmware()) {
        hexis_firmware_free(m_hexEditor->firmware());
    }
}

void MainWindow::setupDashboard()
{
    m_tabWidget = new QTabWidget(this);
    
    // -- Dashboard Tab --
    QWidget* dashWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(dashWidget);
    
    QLabel* header = new QLabel("<h2>Dashboard</h2>");
    header->setAlignment(Qt::AlignCenter);
    layout->addWidget(header);
    
    QTableWidget* infoTable = new QTableWidget(6, 2);
    infoTable->setHorizontalHeaderLabels({"Property", "Value"});
    infoTable->horizontalHeader()->setStretchLastSection(true);
    infoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    infoTable->setSelectionMode(QAbstractItemView::NoSelection);
    
    infoTable->setItem(0, 0, new QTableWidgetItem("Programmer"));
    infoTable->setItem(0, 1, new QTableWidgetItem("Disconnected"));
    infoTable->setItem(1, 0, new QTableWidgetItem("Driver"));
    infoTable->setItem(1, 1, new QTableWidgetItem("None"));
    infoTable->setItem(2, 0, new QTableWidgetItem("Chip"));
    infoTable->setItem(2, 1, new QTableWidgetItem("Unknown"));
    infoTable->setItem(3, 0, new QTableWidgetItem("Capacity"));
    infoTable->setItem(3, 1, new QTableWidgetItem("0 bytes"));
    infoTable->setItem(4, 0, new QTableWidgetItem("Voltage"));
    infoTable->setItem(4, 1, new QTableWidgetItem("0.0V"));
    infoTable->setItem(5, 0, new QTableWidgetItem("Status"));
    infoTable->setItem(5, 1, new QTableWidgetItem("Idle"));
    
    layout->addWidget(infoTable);
    
    QWidget* btnWidget = new QWidget();
    QHBoxLayout* btnLayout = new QHBoxLayout(btnWidget);
    btnLayout->addWidget(new QPushButton("Connect"));
    btnLayout->addWidget(new QPushButton("Read"));
    btnLayout->addWidget(new QPushButton("Write"));
    btnLayout->addWidget(new QPushButton("Erase"));
    btnLayout->addWidget(new QPushButton("Verify"));
    
    layout->addWidget(btnWidget);
    
    // -- Hex Editor Tab --
    m_hexEditor = new HexEditor();
    
    // Mock data for Hex Editor display testing via Backend API
    HexisFirmware* fw = hexis_firmware_create(1024 * 1024);
    if (fw) {
        for (int i=0; i<1024; i++) {
            uint8_t val = i % 256;
            hexis_firmware_write(fw, i, &val, 1);
        }
        
        // Mocking a Patch Engine transaction
        HexisPatchQueue* pq = hexis_patch_queue_create();
        uint8_t patched_magic[] = {0xDE, 0xAD, 0xBE, 0xEF};
        hexis_patch_add(pq, 0x10, NULL, patched_magic, 4, "Injecting test marker");
        hexis_patch_apply_transaction(fw, pq);
        hexis_patch_queue_free(pq);
        
        m_hexEditor->setFirmware(fw);
    }
    
    // -- Firmware Explorer Tab --
    m_fwExplorer = new FirmwareExplorer();
    if (fw) {
        m_fwExplorer->setFirmware(fw);
    }
    
    m_tabWidget->addTab(dashWidget, "Dashboard");
    m_tabWidget->addTab(m_hexEditor, "Hex Editor");
    m_tabWidget->addTab(m_fwExplorer, "Firmware Explorer");
    
    setCentralWidget(m_tabWidget);
}

void MainWindow::setupDocks()
{
    QDockWidget *consoleDock = new QDockWidget("Console", this);
    consoleDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::RightDockWidgetArea);
    QTextEdit *consoleText = new QTextEdit();
    consoleText->setReadOnly(true);
    consoleText->setText("Hexis Core Initialized.\nReady.");
    consoleDock->setWidget(consoleText);
    addDockWidget(Qt::BottomDockWidgetArea, consoleDock);
}
