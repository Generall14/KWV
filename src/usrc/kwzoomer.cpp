#include "kwzoomer.h"
#include <algorithm>

KWZoomer::KWZoomer(QGraphicsScene* ngView):
    gView(ngView)
{
    lastPic = NULL;
    currZoom = 1;
    defMode = noZoom;
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

void KWZoomer::ResetZoom()
{
    if(lastPic==NULL)
        return;

    int picHeight = lastPic->height();
    int picWidth = lastPic->width();
    float tempZoom = 1.0;

    float tempZoomH = limitHeight/picHeight;
    float tempZoomW = limitWidth/picWidth;

    if(defMode==limHeight)
        tempZoom = std::min(tempZoom, tempZoomH);
    else if(defMode==limWidth)
        tempZoom = std::min(tempZoom, tempZoomW);
    else if(defMode==limScreen)
        tempZoom = std::min(tempZoom, std::min(tempZoomH, tempZoomW));

    currZoom = tempZoom;
    sendZoom();
    emit ReZoomed(currZoom*100);
}

void KWZoomer::NewPic(QPixmap* newPic, bool resetZoom)
{
    lastPic = newPic;

    if(newPic==NULL)
    {
        currZoom = 1;
        return;
    }

    if(resetZoom)
        ResetZoom();
    else
        sendZoom();
}

void KWZoomer::sendZoom()
{
    if(lastPic==NULL)
        return;

    QPixmap temp = lastPic->scaledToHeight(lastPic->height()*currZoom, tranMode);

    gView->clear();
    gView->addPixmap(temp);
    gView->setSceneRect(temp.rect());
}
