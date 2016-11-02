#ifndef KWSTATUSBAR_H
#define KWSTATUSBAR_H

#include "osrc/mwptr.h"
#include "osrc/MyQLineEdit.hpp"
#include "src/usrc/kwpicinfo.h"
#include <QStatusBar>
#include <QLabel>
#include <QLine>
#include <QLineEdit>
#include <QDateTime>
#include <QString>
#include <QToolButton>
#include <QIcon>

class MainWindow;

class KWStatusBar : public QObject, public MWPtr
{
    Q_OBJECT
public:
    KWStatusBar(QStatusBar* bar, MainWindow* mw);

private:
    QStatusBar* pasek;                                                                                                      //Wskaźnik na pasek głównego okna

    //-----------------------------------------------Obiekty QLabel do wyświetlania tekstu----------------------------------
    QLabel* zoom;                                                                                                           //Powiększenie
    QLabel* res;                                                                                                            //Rozdzielczość
    MyQLineEdit* plikia;                                                                                                    //Aktualny plik
    QLabel* plikib;                                                                                                         //Liczba plików
    QLabel* data;                                                                                                           //Data modyfikacji
    QLabel* rozmiar;                                                                                                        //Rozmiar pliku

    //-----------------------------------------------Obiekty QPushButton na pasku-------------------------------------------
    QToolButton* resetZoom;                                                                                                 //Przycisk resetujący zoom
    QToolButton* btnNext;                                                                                                   //Następny obraz
    QToolButton* btnBack;                                                                                                   //Poprzedni obraz

    //-----------------------------------------------Obiekty QLabel dla trybu pełnoekranowego-------------------------------
    QLabel* plikiF;                                                                                                         //Liczba plików w trybie fullscreen
    QLabel* plikF;                                                                                                          //Nazwa pliku w trybie fullscreen
    QLabel* zoomF;                                                                                                          //Powiększenie w trybie fullscreen

    int akt=0;                                                                                                              //Aktualny plik
    int all=0;                                                                                                              //Liczba plików

    void Init();                                                                                                            //Inicjalizacja obiektu
    void SetNoPicData();                                                                                                    //Zeruje informacje na pasku

signals:
    void Zmiana(int nr);                                                                                                    //Żądanie zmiany pliku
    void ResetZoom();                                                                                                       //Oryginalny rozmiar
    void Next();                                                                                                            //Żądanie kolejnego obrazu
    void Back();                                                                                                            //Żądanie poprzedniego obrazu

public slots:
    void fullscreenOn();                                                                                                    //Wyświetla napisy pełnoekranowe
    void fullscreenOff();                                                                                                   //Usuwa napisy pełnoekranowe

    void ZmienPlik();                                                                                                       //Zmienia plik po wpisaniu nmeru

    void SetNewData(const KWPicInfo *pi, int curr, int total);                                                              //Ustawia dane obrazu (od KWMotor)
    void UstawZoom(int z);                                                                                                  //Ustawia wyświetlane powiększenie (od KWZoomer)
};

#endif // KWSTATUSBAR_H
