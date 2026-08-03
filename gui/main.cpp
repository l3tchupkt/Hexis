#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include <hexis.h>

void applyDarkMode(QApplication& app) {
    app.setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(30, 30, 30));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(18, 18, 18));
    darkPalette.setColor(QPalette::AlternateBase, QColor(30, 30, 30));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    app.setPalette(darkPalette);
    
    app.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
}

int main(int argc, char *argv[]) {
    // Initialize Hexis core first
    hexis_init();
    
    QApplication a(argc, argv);
    applyDarkMode(a);
    
    MainWindow w;
    w.show();
    return a.exec();
}
