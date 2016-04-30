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
#include "mwptr.h"

class MainWindow;

class KWGraphicsView : public QGraphicsView, public MWPtr
{
    Q_OBJECT
public:
    KWGraphicsView(MainWindow* mw);
    ~KWGraphicsView();

    static constexpr float minZoom = .1;                                                                                    //Minimalne powiększenie
    static constexpr float maxZoom = 5;                                                                                     //Maksymalne powiększenie
    static const int fastest = 5;                                                                                           //Minimalne opóźnienie pomiędzy klatkami
    static const int slowest = 200;                                                                                         //Maksymalne opóźnienie pomiędzy klatkami

    void Wyswietl(QVector<QPixmap> nowyObraz, int t=0);                                                                     //Wyświetla nowy obraz
    void Odswiez();                                                                                                         //Odświeża rozmiar okna i obrazu
    int DefDelay();                                                                                                         //Zwraca prędkość animacji
    bool GifTryb();                                                                                                         //Zwraca tryb animacji
    bool Animacja();                                                                                                        //Zwraca flagę animacji

    long getTimeUs();                                                                                                       //Zwraca czas wyświetlania obrazu

private:
    QGraphicsScene* scena;                                                                                                  //Scena obrazu

    //-----------------------------------------------Animacja---------------------------------------------------------------
    QVector<QPixmap> gifVec;                                                                                                //Wektor obrazów animacji
    int defDelay = 10;                                                                                                      //Domyślny czas pomiędzy klatkami
    QTimer* timer;                                                                                                          //Timer do przełączania klatek
    bool animacja = false;                                                                                                  //Flaga oznaczająca animację
    bool forward = true;                                                                                                    //Kierunek odtwarzania
    int currFrame;                                                                                                          //Aktualna klatka
    int totalFrame;                                                                                                         //Liczba klatek
    bool trybKol = false;                                                                                                   //Tryb odtwarzania

    float zum;                                                                                                              //Współczynnik skalowania obrazu

    std::chrono::steady_clock::time_point czas_start, czas_end;                                                             //Pomiar czasu

    //-----------------------------------------------Zmienne określające rozmiar okien--------------------------------------
    int marSzer;                                                                                                            //Boczne merginesy okna
    int marGora;                                                                                                            //Gorny margines okna
    int marDOl;                                                                                                             //Dolny margines okna
    int paskiGora;                                                                                                          //Wysokość górnych pasków
    int paskiDol;                                                                                                           //Wysokość dolnych pasków
    QRect avFull;                                                                                                           //Dostępna rozdzielczość w trybie pełnoekranowym
    QRect maxOkno;                                                                                                          //Maksymalny rozmiar okna
    QRect maxObraz;                                                                                                         //Maksymalny rozmiar wyświetlanego obrazu
    QRect maxObrazMax;                                                                                                      //Maksymalny rozmiar wyświetlanego obrazu przy zmaksymalizowanym oknie

    //-----------------------------------------------Operacje łączące oknmo z obrazem---------------------------------------
    void DopasujOkno(QRect trect);                                                                                          //Dopasowuje rozmiary okna i obrazu
    void DopasujOknoDoObrazu(QRect trect);                                                                                  //Dopasowuje rozmiar okna do obrazu
    void DopasujObrazDoOkna(QRect trect);                                                                                   //Dopasowuje rozmiar obrazu do okna

    QRect DodajMar(QRect rwe);                                                                                              //Dodaje marginesy początkowe i rozmiaru

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
    void Zoomed(int z);                                                                                                     //Zmiane powiększenia obrazu
    void Next();                                                                                                            //Żądanie następnego obrazu
    void Back();                                                                                                            //Żądanie poprzedniego obrazu
    void ToggleFullscreen();                                                                                                //Żądanie przełączenia trybu fulscreen
    void Nowy();                                                                                                            //Nowy obraz

public slots:
    void InitGeometry();                                                                                                    //Inicjalizacja geometri okna i ekranu, musi zostać wykonana po wyświetleniu okna

    void ZoomIn();                                                                                                          //Powiększenie o jeden skok
    void ZoomOut();                                                                                                         //Pomniejszenie o jeden skok
    void SetZoom(float z);                                                                                                  //Przeskalowuje obraz z dopasowaniem okna
    void ResetZoom();                                                                                                       //Resetuje powiększenie

    void GifNext();                                                                                                         //Następna klatka animacji
    void GifBack();                                                                                                         //Poprzednia klatka animacji
    void GifPlayPause();                                                                                                    //Włącza / wyłącza odtwarzanie
    void GifSetSpeed(int s);                                                                                                //ustawia opóźnienie klatki
    void GifSetTryb(bool b);                                                                                                //Ustawia tryb odtwarzania

private slots:
    void Zoom(float z, QPixmap px=QPixmap());                                                                               //Przeskalowanie obrazu (bez dopasowania okna)
    void GifUpd();
};

#endif // KWGRAPHICSVIEW_H
