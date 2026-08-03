#include "mainwindow.h"
#include <QDockWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Hexis Hardware Hacking Framework");
    resize(1200, 800);
    
    setupDashboard();
    setupDocks();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupDashboard()
{
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    
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
    setCentralWidget(centralWidget);
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
