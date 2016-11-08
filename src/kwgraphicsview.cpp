#include "kwgraphicsview.h"
#include "../mainwindow.h"
#include <QDebug>
#include <assert.h>

KWGraphicsView::KWGraphicsView(MainWindow *mw):
    QGraphicsView(mw),
    MWPtr(mw)
{
    scena = new QGraphicsScene(this);                                               //Tworzenie sceny
    this->setScene(scena);                                                          //Dodawanie sceny do widoku

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);                       //Wyłączenie pasków
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setMouseTracking(true);                                                         //Ustawianie śledzenia myszy

    if(MWw->testRun)
        scena->setBackgroundBrush(QBrush(QColor(100, 200, 150)));
    else
        scena->setBackgroundBrush(QBrush(QColor(0, 0, 0)));                         //Ustawianie tła
}

QGraphicsScene* KWGraphicsView::Scena()
{
    return scena;
}



void KWGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton)                                            //Prawy przycisk myszy
    {
        przeciaganie = true;                                                        //Ustawienie flagi przeciągania
        setCursor(QCursor(Qt::ClosedHandCursor));                                   //Ustawianie kursora
    }
    else
        QGraphicsView::mousePressEvent(event);                                      //Wywoływanie standardowej obsługi zdarzeń

    pozycja = event->globalPos();                                                   //Zapisanie pozycji kursora
}

void KWGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    przeciaganie = false;                                                           //Resetowanie flagi przeciągania

    setCursor(QCursor(Qt::ArrowCursor));                                            //Resetowanie kursora

    QGraphicsView::mouseReleaseEvent(event);                                        //Wywoływanie standardowej obsługi zdarzeń
}

void KWGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if(MWw->isFullScreen())                                                         //Wykrywanie trybu fullscreen
    {
        MWw->cursorTimer->start();                                                  //Restart timera
        QApplication::restoreOverrideCursor();                                      //Przywracanie kursora
    }

    if(przeciaganie)                                                                //Jeżeli ustawiony tryb przeciągania
    {
        QPoint temp = event->globalPos();                                           //Pobranie pozycji kursora
        int dx = temp.x() - pozycja.x();                                            //Obliczanie przesunięć
        int dy = temp.y() - pozycja.y();
        verticalScrollBar()->setValue(verticalScrollBar()->value()-dy);             //Przesunięcie obrazu
        horizontalScrollBar()->setValue(horizontalScrollBar()->value()-dx);
        pozycja = temp;                                                             //Nadpisanie pozycji
    }
    else
        QGraphicsView::mouseMoveEvent(event);                                       //Wywoływanie standardowej obsługi zdarzeń
}

void KWGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)                                             //Lewy przycisk myszy
        emit ToggleFullscreen();                                                    //Żądanie przełączenia trybu fullscreen
}

void KWGraphicsView::keyPressEvent(QKeyEvent *event)
{
    if(!MWw->picObj->GetPicInfo()->isOk)
    {
        QGraphicsView::keyPressEvent(event);                                        //Wywołanie standardowej obsługi klawiszy
        return;
    }

    QPixmap* qpt = MWw->picObj->GetFrame();
    QSize trect;
    if(qpt)
        trect = qpt->size();
    QSize max;

    if(MWw->isMaximized())                                                          //Dobranie odpowiedniego odniesienia
        max = MWw->windowZoomerObj->getMaxPicSizeMaximized();
    else if(MWw->isFullScreen())
        max = MWw->windowZoomerObj->getMaxPicSizeFullscreen();
    else
        max = MWw->windowZoomerObj->getMaxPicSize();

    float zum = MWw->zoomerObj->getCurrentZoom();

    bool przeskalowaneW = false, przeskalowaneH = false;                            //Sprawdzanie czy obraz jest powiększony ponad okno
    if(trect.width()*zum>(max.width()))
        przeskalowaneW = true;
    if(trect.height()*zum>(max.height()))
        przeskalowaneH = true;

    if(event->key()==Qt::Key_Right)                                                 //Strzałka w prawo
    {
        if(!przeskalowaneW)                                                         //Obraz przeskalowany -> przesuwanie obrazu
            emit Next();
        else                                                                        //W przeciwnym wypadku -> zmiana obrazu
            QGraphicsView::keyPressEvent(event);
    }
    else if(event->key()==Qt::Key_Left)                                             //Strzałka w lewo
    {
        if(!przeskalowaneW)
            emit Back();
        else
            QGraphicsView::keyPressEvent(event);
    }
    else if(event->key()==Qt::Key_Up)                                               //Strzałka w górę
    {
        if(!przeskalowaneH)                                                         //Obraz przeskalowany -> przesuwanie obrazu
            emit Next();
        else                                                                        //W przeciwnym wypadku -> zmiana obrazu
            QGraphicsView::keyPressEvent(event);
    }
    else if(event->key()==Qt::Key_Down)                                             //Strzałka w dół
    {
        if(!przeskalowaneH)
            emit Back();
        else
            QGraphicsView::keyPressEvent(event);
    }
    else
        QGraphicsView::keyPressEvent(event);                                        //Wywołanie standardowej obsługi klawiszy
}

void KWGraphicsView::wheelEvent(QWheelEvent* event)
{
    if((event->modifiers()==Qt::ControlModifier)||(event->buttons()==Qt::MidButton))//Sprawdzanie czy wciśnięto CTRL lub ŚPM
    {
        if(event->angleDelta().y()>0)                                               //Skalowanie obrazu
            emit ZoomInReq();
        else
            emit ZoomOutReq();
    }
    else
    {
        if(event->angleDelta().y()>0)                                               //Przełączanie obrazów
            emit Next();
        else
            emit Back();
    }
}
