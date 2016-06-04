#include "kwmenu.h"
#include "../mainwindow.h"
#include "kwgraphicsview.h"

#include <QDebug>

KWMenu::KWMenu(MainWindow* mw):
    QObject(mw),
    MWPtr(mw)
{
    InitMenus();
    QVector<QString> tv;
    UpdateRecent(tv);
}

KWMenu::~KWMenu()
{
    //delete menuPlik;
    //menuPlik = 0;
}

void KWMenu::InitMenus()
{
    //-----------------------------------Menu Plik----------------------------------
    menuPlik = new QMenu(tr("&Plik"), 0);                                           //Menu -> Plik

    akcjaOtworz = new QAction(tr("&Otwórz"), menuPlik);                                     //Plik -> Otwórz
    akcjaOtworz->setShortcut(QKeySequence::Open);
    connect(akcjaOtworz, SIGNAL(triggered(bool)), this, SIGNAL(Otworz()));
    menuPlik->addAction(akcjaOtworz);

    menuRecent = new QMenu(tr("O&Statnio otwierane"), 0);                                   //Menu -> Ostatnio otwierane
    menuPlik->addMenu(menuRecent);

    akcjaRename = new QAction(tr("Z&Mień nazwę pliku"), menuPlik);                          //Plik -> Rename
    akcjaRename->setShortcut(QKeySequence(Qt::Key_F2));
    connect(akcjaRename, SIGNAL(triggered(bool)), MWw, SLOT(Rename()));
    menuPlik->addAction(akcjaRename);

    akcjaNoweOkno = new QAction(tr("Otwórz &Nowe okno"), menuPlik);                         //Plik -> Otwórz nowe okno
    akcjaNoweOkno->setShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_N));
    connect(akcjaNoweOkno, SIGNAL(triggered(bool)), MWw, SLOT(NoweOkno()));
    menuPlik->addAction(akcjaNoweOkno);

    menuPlik->addSeparator();                                                               //Separator

    akcjaZamknij = new QAction(tr("&Zamknij"), menuPlik);                                   //Plik -> Zamknij
    connect(akcjaZamknij, SIGNAL(triggered(bool)), MWw, SLOT(close()));
    menuPlik->addAction(akcjaZamknij);

    MWw->menuBar()->addMenu(menuPlik);                                              //Dodawanie do menu głównego okna

    //-----------------------------------Menu Edycja--------------------------------
    menuEdycja = new QMenu(tr("&Edycja"), 0);                                       //Menu -> Edycja

    akcjaUsun = new QAction(tr("&Usuń"), menuEdycja);                                       //Edycja -> Usuń
    connect(akcjaUsun, SIGNAL(triggered(bool)), MWw, SLOT(Usun()));
    menuEdycja->addAction(akcjaUsun);

    akcjaGifManager = new QAction(tr("&Gif manager"), menuEdycja);                          //Edycja -> Gif Manager
    connect(akcjaGifManager, SIGNAL(triggered(bool)), MWw, SLOT(GifManager()));
    menuEdycja->addAction(akcjaGifManager);

    MWw->menuBar()->addMenu(menuEdycja);                                            //Dodawanie do menu głównego okna

    //-----------------------------------Menu Obraz---------------------------------
    menuObraz = new QMenu(tr("&Informacje o obrazie"), 0);                          //Menu -> Obraz

    akcjaFileInfo = new QAction(tr("&Usuń"), menuEdycja);                                   //Obraz -> Informacje o pliku
    akcjaFileInfo->setShortcut(QKeySequence(Qt::Key_I));
    connect(akcjaFileInfo, SIGNAL(triggered(bool)), MWw, SLOT(FileInfo()));
    menuObraz->addAction(akcjaFileInfo);

    MWw->menuBar()->addMenu(menuObraz);                                            //Dodawanie do menu głównego okna

    //-----------------------------------Menu Widok---------------------------------
    menuWidok = new QMenu(tr("&Widok"), 0);                                         //Menu -> Widok

    akcjaResetZoom = new QAction(tr("&Oryginalny rozmiar"), menuWidok);                     //Widok -> Oryginalny rozmiar
    akcjaResetZoom->setShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_H));
    connect(akcjaResetZoom, SIGNAL(triggered(bool)), MWw->wyswietlacz, SLOT(ResetZoom()));
    menuWidok->addAction(akcjaResetZoom);

    akcjaLosowy = new QAction(tr("&Losowy obraz"), menuWidok);                      //Widok -> Losowy obraz
    akcjaLosowy->setShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_M));
    connect(akcjaLosowy, SIGNAL(triggered(bool)), MWw->motor, SLOT(RandImg()));
    menuWidok->addAction(akcjaLosowy);

    MWw->menuBar()->addMenu(menuWidok);                                             //Dodawanie do menu głównego okna

    //-----------------------------------Menu Pomoc---------------------------------
    menuPomoc = new QMenu(tr("P&omoc"), 0);                                         //Menu -> Pomoc

    akcjaOProgramie = new QAction(tr("&O programie"), menuPomoc);                           //Plik -> Otwórz
    connect(akcjaOProgramie, SIGNAL(triggered(bool)), this, SIGNAL(About()));
    menuPomoc->addAction(akcjaOProgramie);

    MWw->menuBar()->addMenu(menuPomoc);                                             //Dodawanie do menu głównego okna
}

void KWMenu::UpdateRecent(QVector<QString> recVec)
{
    menuRecent->clear();                                                            //Odłączanie akcji z menu

    for(int i=0;i<recentActions.length();++i)                                       //Usuwanie starych obiektów
        delete recentActions[i];
    recentActions.clear();

    if(recVec.isEmpty())
    {
        QAction* emptyAction = new QAction(tr("Brak"), NULL);                       //Brak plików
        emptyAction->setEnabled(false);
        menuRecent->addAction(emptyAction);
        recentActions.push_back(emptyAction);
    }
    else
    {
        for(int i=recVec.length()-1;i>=0;--i)
        {
            QAction* newAction = new QAction(recVec.at(i), NULL);
            connect(newAction, &QAction::triggered, this, [=](bool){emit OpenRec(i);});
            menuRecent->addAction(newAction);
            recentActions.push_back(newAction);
        }
    }
}
