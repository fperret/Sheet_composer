#include "mainwindow.h"
#include "centralwidget.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    CentralWidget l_centralWidget(&w);

    l_centralWidget.setConfig(&w.m_config);
    w.setCentralWidget(&l_centralWidget);
    w.show();
    return a.exec();
}
