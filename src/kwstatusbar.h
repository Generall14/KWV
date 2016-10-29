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
    QLabel* zoom;                                                                                                           //Powiększenie<REF>
    QLabel* res;                                                                                                            //Rozdzielczość<REF>
    MyQLineEdit* plikia;                                                                                                      //Aktualny plik<REF>
    QLabel* plikib;                                                                                                         //Liczba plików<REF>
    QLabel* data;                                                                                                           //Data modyfikacji<REF>
    QLabel* rozmiar;                                                                                                        //Rozmiar pliku<REF>

    //-----------------------------------------------Obiekty QPushButton na pasku-------------------------------------------
    QToolButton* resetZoom;                                                                                                 //Przycisk resetujący zoom<REF>
    QToolButton* btnNext;                                                                                                   //Następny obraz<REF>
    QToolButton* btnBack;                                                                                                   //Poprzedni obraz<REF>

    //-----------------------------------------------Obiekty QLabel dla trybu pełnoekranowego-------------------------------
    QLabel* plikiF;                                                                                                         //Liczba plików w trybie fullscreen<REF>
    QLabel* plikF;                                                                                                          //Nazwa pliku w trybie fullscreen<REF>
    QLabel* zoomF;                                                                                                          //Powiększenie w trybie fullscreen<REF>

    int akt=0;                                                                                                              //Aktualny plik<REF>
    int all=0;                                                                                                              //Liczba plików<REF>

    void Init();                                                                                                            //Inicjalizacja obiektu<REF>
    void SetNoPicData();                                                                                                    //Zeruje informacje na pasku

signals:
    void Zmiana(int nr);                                                                                                    //Żądanie zmiany pliku<REF>
    void ResetZoom();                                                                                                       //Oryginalny rozmiar<REF>
    void Next();                                                                                                            //Żądanie kolejnego obrazu<REF>
    void Back();                                                                                                            //Żądanie poprzedniego obrazu<REF>

public slots:

    //void UstawRozdzielczosc(int w, int h, int k=0, int d=0);                                                                //Ustawia wyświetlaną rozdzielczość<REF>
    //void UstawLicznik(int c, int a);                                                                                        //Ustawia wyświetlany licznik plików<REF>
    //void UstawDate(QDateTime d);                                                                                            //Ustawia wyświetlaną datę<REF>
    //void UstawRozmiar(int KiB);                                                                                             //Ustawia wyświetlany rozmiar<REF>
    //void UstawPlik(QString pl);                                                                                             //Ustawia nazwę pliku<REF>

    void fullscreenOn();                                                                                                    //Wyświetla napisy pełnoekranowe<REF>
    void fullscreenOff();                                                                                                   //Usuwa napisy pełnoekranowe<REF>

    void ZmienPlik();                                                                                                       //Zmienia plik po wpisaniu nmeru

    void SetNewData(const KWPicInfo *pi, int curr, int total);                                                              //Ustawia dane obrazu (od KWMotor)
    void UstawZoom(int z);                                                                                                  //Ustawia wyświetlane powiększenie (od KWZoomer)
};

#endif // KWSTATUSBAR_H
