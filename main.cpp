#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    for(int i=0;i<qApp->arguments().length();++i)
    {
        if((qApp->arguments().at(i)=="-h")||(qApp->arguments().at(i)=="--help"))
        {
            std::cout << "-h  --help   -> ta wiadomość\n"
                         "-t           -> tryb testowy\n";
            return 0;
        }
    }

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
