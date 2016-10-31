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
    startLimit.setWidth(100);
    startLimit.setHeight(100);
    tranMode = Qt::SmoothTransformation;
}

void KWZoomer::SetDefaultMode(defTypes newType, QSize newLimit)
{
    SetLimits(newLimit);
    defMode = newType;
}

void KWZoomer::SetLimits(QSize newLimit)
{
    if((newLimit.width()>=100)&&(newLimit.height()>=100))
        startLimit = newLimit;
    if(!touched)
        ReCalcZoom();
}

void KWZoomer::ReCalcZoom()
{
    if(lastPic==NULL)
    {
        sendZoom(true);
        return;
    }

    int picHeight = lastPic->height();
    int picWidth = lastPic->width();
    float tempZoom = 1.0;

    float tempZoomH = (float)startLimit.height()/(float)picHeight;
    float tempZoomW = (float)startLimit.width()/(float)picWidth;

    if(defMode==limHeight)
        tempZoom = std::min(tempZoom, tempZoomH);
    else if(defMode==limWidth)
        tempZoom = std::min(tempZoom, tempZoomW);
    else if(defMode==limScreen)
        tempZoom = std::min(tempZoom, std::min(tempZoomH, tempZoomW));
//    qDebug() << "WKZoomer, zoom = " << tempZoom;
    currZoom = tempZoom;
    sendZoom(true);
    //emit ReZoomed(currZoom*100);
}

void KWZoomer::ResetZoom()
{
    SetZoom(1.0);
    touched = true;
}

void KWZoomer::NewPic(QPixmap* newPic, bool resetZoom)
{
    lastPic = newPic;

    if(resetZoom)
        ReCalcZoom();
    else
        sendZoom();
    touched = false;
}

void KWZoomer::sendZoom(bool sendSig)
{
    gView->clear();

    if(lastPic==NULL)
    {
        emit ReZoomed(100, QSize(0, 0));
        return;
    }

    QPixmap temp = lastPic->scaledToHeight(lastPic->height()*currZoom, tranMode);

    gView->addPixmap(temp);
    gView->setSceneRect(temp.rect());
    if(sendSig)
        emit ReZoomed(currZoom*100, temp.size());
    qApp->processEvents();                                                          //Zapewnia wyświetlanie obrazu w stanie zamulenia
}

void KWZoomer::SetZoom(float newZoom)
{
    if((newZoom<MIN_ZOOM)||(newZoom>MAX_ZOOM))
        return;

    currZoom = newZoom;
    sendZoom(true);
    touched = true;
}

void KWZoomer::ZoomIn()
{
    SetZoom(floor(currZoom*(1/DELTA_ZOOM)+DELTA_ZOOM)*DELTA_ZOOM+DELTA_ZOOM);
    touched = true;
}

void KWZoomer::ZoomOut()
{
    SetZoom(ceil(currZoom*(1/DELTA_ZOOM)-DELTA_ZOOM)*DELTA_ZOOM-DELTA_ZOOM);
    touched = true;
}

