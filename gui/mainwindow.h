#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "hexis_driver.h"
#include "hexis_chip.h"

class QTabWidget;
class HexEditor;
class FirmwareExplorer;
class QTableWidget;
class QTextEdit;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConnect();
    void onRead();
    void onWrite();
    void onErase();
    void onVerify();
    void updateStatus();

private:
    void setupDashboard();
    void setupDocks();
    void logMessage(const QString& msg);
    
    QTabWidget* m_tabWidget;
    HexEditor* m_hexEditor;
    FirmwareExplorer* m_fwExplorer;
    QTableWidget* m_infoTable;
    QTextEdit* m_consoleText;
    
    const hexis_driver_t* m_driver;
    hexis_device_context_t* m_ctx;
    QTimer* m_timer;
};

#endif // MAINWINDOW_H
