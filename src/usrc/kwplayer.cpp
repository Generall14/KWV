#include "kwplayer.h"

KWPlayer::KWPlayer(KWPic *pic, KWZoomer *zoomerPtr, QObject *parent):
    QObject(parent),
    picPtr(pic)
{
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(NextFrame()));

    Halt();

    connect(this, SIGNAL(ShowNewPic(QPixmap*,bool)), zoomerPtr, SLOT(NewPic(QPixmap*,bool)));
}

KWPlayer::~KWPlayer()
{
    delete timer;
}

void KWPlayer::Halt()
{
    frames = 0;
    currFrame = 0;
    delay = 0;
    defDelay = 0;
    cicle = false;
    right = true;
    reverse = false;
    timer->stop();
}

void KWPlayer::NewPic(const KWPicInfo *pi)
{
    Halt();

    if(!pi->fileInfo.isFile())
    {
        emit ShowNewPic(NULL, true);
        emit LoadedNewPic();
        return;
    }

    if(pi->frames>1)
    {
        frames = pi->frames;
        delay = pi->frameDelay;
        defDelay = delay;
        timer->start(delay);
    }

    emit ShowNewPic(picPtr->GetFrame(), true);
    emit LoadedNewPic();
}

void KWPlayer::NextFrame()
{
    if(!cicle)
    {
        if(!reverse)
            currFrame++;
        else
            currFrame--;
        if(currFrame>=frames)
            currFrame = 0;
        if(currFrame<0)
            currFrame = frames-1;
    }
    else
    {
        if(right)
            currFrame++;
        else
            currFrame--;
        if(currFrame>=frames)
        {
            currFrame = frames-2;
            right = !right;
        }
        if(currFrame<0)
        {
            currFrame = 1;
            right = !right;
        }
    }

    emit ShowNewPic(picPtr->GetFrame(currFrame), false);
}

void KWPlayer::PrevFrame()
{
    if(!cicle)
    {
        if(reverse)
            currFrame++;
        else
            currFrame--;
        if(currFrame>=frames)
            currFrame = 0;
        if(currFrame<0)
            currFrame = frames-1;
    }
    else
    {
        if(!right)
            currFrame++;
        else
            currFrame--;
        if(currFrame>=frames)
        {
            currFrame = frames-2;
            right = !right;
        }
        if(currFrame<0)
        {
            currFrame = 1;
            right = !right;
        }
    }

    emit ShowNewPic(picPtr->GetFrame(currFrame), false);
}

int KWPlayer::getFrameDelay() const
{
    return delay;
}

int KWPlayer::getCyclic() const
{
    return cicle;
}

int KWPlayer::getReverse() const
{
    return reverse;
}

int KWPlayer::RestoreDefaultFrameDelay()
{
    delay = defDelay;
    timer->stop();
    if(frames)
        timer->start(delay);
    return defDelay;
}

void KWPlayer::PlayPause()
{
    if(timer->isActive())
        timer->stop();
    else
        timer->start();
}

void KWPlayer::setDelay(int newDelayMs)
{
    delay = newDelayMs;
    timer->stop();
    if(frames)
        timer->start(delay);
}

void KWPlayer::setCyclic(bool newCyclic)
{
    cicle = newCyclic;
}

void KWPlayer::setReverse(bool newReverse)
{
    reverse = newReverse;
}
