#include "kwzoomer.h"
#include <algorithm>
#include <QDebug>
#include <QApplication>

KWZoomer::KWZoomer(QGraphicsScene* ngView, QObject *parent):
    QObject(parent),
    gView(ngView)
{
    lastPic = NULL;
    currZoom = 1;
    defMode = limScreen;
    limitWidth = 100;
    limitHeight = 100;
    tranMode = Qt::SmoothTransformation;
}

void KWZoomer::SetDefaultMode(defTypes newType, int newWidth, int newHeight)
{
    SetLimits(newWidth, newHeight);
    defMode = newType;
}

void KWZoomer::SetLimits(int newWidth, int newHeight)
{
    if(newWidth>100)
        limitWidth = newWidth;
    if(newHeight>100)
        limitHeight = newHeight;
}

void KWZoomer::ReCalcZoom()
{
    if(lastPic==NULL)
    {
        sendZoom();
        return;
    }

    int picHeight = lastPic->height();
    int picWidth = lastPic->width();
    float tempZoom = 1.0;

    float tempZoomH = (float)limitHeight/(float)picHeight;
    float tempZoomW = (float)limitWidth/(float)picWidth;

    if(defMode==limHeight)
        tempZoom = std::min(tempZoom, tempZoomH);
    else if(defMode==limWidth)
        tempZoom = std::min(tempZoom, tempZoomW);
    else if(defMode==limScreen)
        tempZoom = std::min(tempZoom, std::min(tempZoomH, tempZoomW));
//    qDebug() << "WKZoomer, zoom = " << tempZoom;
    currZoom = tempZoom;
    sendZoom();
    emit ReZoomed(currZoom*100);
}

void KWZoomer::ResetZoom()
{
    SetZoom(1.0);
}

void KWZoomer::NewPic(QPixmap* newPic, bool resetZoom)
{
    lastPic = newPic;

//    if(newPic==NULL)
//    {
//        currZoom = 1;
//        return;
//    }

    if(resetZoom)
        ReCalcZoom();
    else
        sendZoom();
}

void KWZoomer::sendZoom()
{
    gView->clear();

    if(lastPic==NULL)
        return;

    QPixmap temp = lastPic->scaledToHeight(lastPic->height()*currZoom, tranMode);

    gView->addPixmap(temp);
    gView->setSceneRect(temp.rect());
    emit ReZoomed(currZoom*100);
    qApp->processEvents();                                                          //Zapewnia wyświetlanie obrazu w stanie zamulenia
}

void KWZoomer::SetZoom(float newZoom)
{
    if((newZoom<MIN_ZOOM)||(newZoom>MAX_ZOOM))
        return;

    currZoom = newZoom;
    sendZoom();
}

void KWZoomer::ZoomIn()
{
    SetZoom(floor(currZoom*(1/DELTA_ZOOM)+DELTA_ZOOM)*DELTA_ZOOM+DELTA_ZOOM);
}

void KWZoomer::ZoomOut()
{
    SetZoom(ceil(currZoom*(1/DELTA_ZOOM)-DELTA_ZOOM)*DELTA_ZOOM-DELTA_ZOOM);
}

