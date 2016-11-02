#ifndef KWGRAPHICSVIEW_H
#define KWGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QScrollBar>
#include <QWheelEvent>
#include <QVector>
#include <QTimer>
#include <chrono>
#include "osrc/mwptr.h"

class MainWindow;

class KWGraphicsView : public QGraphicsView, public MWPtr
{
    Q_OBJECT
public:
    KWGraphicsView(MainWindow* mw);

    QGraphicsScene* Scena();

private:
    QGraphicsScene* scena;                                                                                                  //Scena obrazu

    //-----------------------------------------------Obsługa zdarzeń myszy i klawiatury-------------------------------------
    bool przeciaganie;                                                                                                      //Flaga oznaczająca przeciąganie obrazu
    QPoint pozycja;                                                                                                         //Zapisana pozycja kursora
    virtual void mousePressEvent(QMouseEvent *event);                                                                       //Wciśnięcie klawisza myszy
    virtual void mouseReleaseEvent(QMouseEvent *event);                                                                     //Zwolnienie klawisza myszy
    virtual void mouseMoveEvent(QMouseEvent *event);                                                                        //Przesunięcie myszy
    virtual void mouseDoubleClickEvent(QMouseEvent *event);                                                                 //Podwójne kliknięcie
    virtual void wheelEvent(QWheelEvent *event);                                                                            //Użycie kółka myszy
    virtual void keyPressEvent(QKeyEvent *event);                                                                           //Wciśnięcie klawisza klawiatury

signals:
    void Next();                                                                                                            //Żądanie następnego obrazu
    void Back();                                                                                                            //Żądanie poprzedniego obrazu
    void ToggleFullscreen();                                                                                                //Żądanie przełączenia trybu fulscreen
//    void Nowy();                                                                                                            //Nowy obraz

    void ZoomInReq();
    void ZoomOutReq();
};

#endif // KWGRAPHICSVIEW_H
