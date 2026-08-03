#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTabWidget;
class HexEditor;
class FirmwareExplorer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupDashboard();
    void setupDocks();
    
    QTabWidget* m_tabWidget;
    HexEditor* m_hexEditor;
    FirmwareExplorer* m_fwExplorer;
};

#endif // MAINWINDOW_H
