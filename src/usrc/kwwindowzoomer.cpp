#include "kwwindowzoomer.h"

#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include "../../mainwindow.h"

KWWindowZoomer::KWWindowZoomer(MainWindow *mw):
    QObject(mw),
    MWPtr(mw)
{
}

void KWWindowZoomer::InitGeometry()
{
    disconnect(qApp, SIGNAL(applicationStateChanged(Qt::ApplicationState)), this, SLOT(InitGeometry()));

    QDesktopWidget desktop;
    fullResolution = desktop.screenGeometry().size();                               //Pobieranie rozdzielczości ekranu

    QRect screenGeometry = desktop.availableGeometry();                             //Pobieranie dostępnego rozmiaru pulpitu
    QRect windowGeometry = MWw->frameGeometry();                                    //Pobieranie rozmiaru zewnętrznego okna
    QRect useGeometry = MWw->geometry();                                            //Pobieranie rozmiaru wewnętrznego okna

    int titleBarHeight = abs(windowGeometry.y());                                   //Wysokość belki programu

    int paskiGora = MWw->menuBar()->height();                                       //Pobieranie wysokości paska menu
    int paskiDol = MWw->statusBar()->height();                                      //Pobieranie wartości paska statusu
    int ramkiGora = windowGeometry.height()-useGeometry.height()-titleBarHeight;    //Sumaryczna wysokość ramki okna na dole i górze
    deltaHeight = paskiDol+paskiGora+ramkiGora;                                     //Całkowite straty okna w wysokości
    deltaWidth = windowGeometry.width()-useGeometry.width();                        //Straty szerokości

    zeroPoint = QPoint(0, titleBarHeight);
    maxWindowSize = QSize(screenGeometry.width(), screenGeometry.height()-titleBarHeight);
    maxPicSize = QSize(maxWindowSize.width()-deltaWidth, maxWindowSize.height()-deltaHeight);
    maxPicSizeMaximized = QSize(maxWindowSize.width(), maxWindowSize.height()-deltaHeight+ramkiGora);

    this->setNormalMode();

    if(!MWw->startOpen.isNull())
        MWw->Otworz(MWw->startOpen);
}

void KWWindowZoomer::setNormalMode()
{
    currentMode = normalMode;
    emit setMaximumPicSize(maxPicSize);
}

void KWWindowZoomer::setMaximizeMode()
{
    currentMode = maximizedMode;
    emit setMaximumPicSize(maxPicSizeMaximized);
}

void KWWindowZoomer::setFullscreenMode()
{
    currentMode = fullscreenMode;
    emit setMaximumPicSize(fullResolution);
}

void KWWindowZoomer::PicReZoomed(int, QSize newSize)
{
    if(currentMode!=normalMode)
        return;

    newSize.setHeight(newSize.height()+deltaHeight);
    newSize.setWidth(newSize.width()+deltaWidth);

    if(newSize.height()>maxWindowSize.height())
        newSize.setHeight(maxWindowSize.height());
    if(newSize.width()>maxWindowSize.width())
        newSize.setWidth(maxWindowSize.width());

    MWw->setGeometry(QRect(zeroPoint, newSize));
}
