#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    try
    {
        return a.exec();
    }
    catch(QString a)
    {
        QMessageBox::critical(0, "FATAL ERROR!", a);
    }
}
