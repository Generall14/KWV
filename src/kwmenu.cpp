#include "kwmenu.h"
#include "../mainwindow.h"

#include <QDebug>

KWMenu::KWMenu(MainWindow* mw):
    QObject(mw),
    MWPtr(mw)
{
    InitMenus();
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
    menuEdycja = new QMenu(tr("&Edycja"), 0);                                       //Menu -> Pomoc

    akcjaUsun = new QAction(tr("&Usuń"), menuEdycja);                                       //Plik -> Usuń
    connect(akcjaUsun, SIGNAL(triggered(bool)), MWw, SLOT(Usun()));
    menuEdycja->addAction(akcjaUsun);

    akcjaGifManager = new QAction(tr("Gif manager"), menuEdycja);                           //Menu -> Gif Manager
    connect(akcjaGifManager, SIGNAL(triggered(bool)), MWw, SLOT(GifManager()));
    menuEdycja->addAction(akcjaGifManager);

    MWw->menuBar()->addMenu(menuEdycja);                                            //Dodawanie do menu głównego okna

    //-----------------------------------Menu Pomoc---------------------------------
    menuPomoc = new QMenu(tr("P&omoc"), 0);                                         //Menu -> Pomoc

    akcjaOProgramie = new QAction(tr("&O programie"), menuPomoc);                           //Plik -> Otwórz
    connect(akcjaOProgramie, SIGNAL(triggered(bool)), this, SIGNAL(About()));
    menuPomoc->addAction(akcjaOProgramie);

    MWw->menuBar()->addMenu(menuPomoc);                                             //Dodawanie do menu głównego okna
}
