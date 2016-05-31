#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <QMenuBar>
#include <QPushButton>
#include <QPixmap>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
#include <QShortcut>
#include <QEvent>
#include <QWindowStateChangeEvent>
#include <QLabel>
#include <QApplication>
#include <QFile>

#include <chrono>

#include <stdlib.h>
#include "hplik.h"
#include "src/kwmenu.h"
#include "src/kwgraphicsview.h"
#include "src/kwstatusbar.h"
#include "src/kwmotor.h"
#include "src/3rdforms/kw3rdgif.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class KWMenu;
    friend class KWGraphicsView;

    enum trybOkna{Normalny, Zmaksymalizowany};
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static const int minSzerokosc = 600;
    static const int minWysokosc = 700;

    QString startOpen;                                                                                                      //Adres obrazu podanego jako argument

private:
    bool testRun = false;                                                                                                   //Tryb testowy

    //-----------------------------------------------Obiekty składowe programu----------------------------------------------
    KWMenu* menu;                                                                                                           //Tworzy pasek menu
    KWGraphicsView* wyswietlacz;                                                                                            //Tworzy wyświetlacz obrazów
    KWStatusBar* pasekDolny;                                                                                                //Obsługuje pasek stanu
    KWMotor* motor;                                                                                                         //Silnik obsługujący pliki obrazów

    trybOkna prev;                                                                                                          //Tryb okna przed wejściem do fullscreen
    QPushButton* invisibleCloser;                                                                                           //Służy jako przycisk zamykania trybu fullscreen

    QVector<QShortcut*> rep;                                                                                                //Skróty powtórzone z paska menu (włączane tylko w fullscreen)

    QTimer* cursorTimer = NULL;                                                                                             //Timer do ukrywania kursora w trybie fullscreen

    KW3rdGif* okno = NULL;

    //-----------------------------------------------Funkcje inicjalizujące obiekt------------------------------------------
    void InitWidgets();                                                                                                     //Inicjalizuje widżety
    void InitConnections();                                                                                                 //Inicjalizuje połączenia
    void InitShortcuts();                                                                                                   //Inicjalizacja skrótów klawiszowych

    virtual void keyPressEvent(QKeyEvent *event);                                                                           //Wciśnięcie klawisza klawiatury

    void Benchmark();                                                                                                       //Testy prędkości

public slots:
    void testOpenA();
    void publicTest();

    void Otworz(QString adres);                                                                                             //Otwarcie pliku
    void Usun();                                                                                                            //Usunięcie pliku

    void TitleBar(QString name);                                                                                            //Ustawia napis na pasku okna
    void About();                                                                                                           //Wyświetla informacje o programie
    void Error(QString er);                                                                                                 //Wyświetlenie błędu

    void GifManager();                                                                                                      //Wyświetla okno menedżera animacji

private slots:
    void setFullsscreen();                                                                                                  //Włącza tryb fullscreen
    void resetFullscreen();                                                                                                 //Włącza normalny tryb
    void ToggleFullscreen();                                                                                                //Przełącza tryb fullscreen

    void NoweOkno();                                                                                                        //Otwiera nowe okno programu

    void HideCursor();                                                                                                      //Ukrywa kursor

    virtual bool event(QEvent* event);                                                                                      //Obsługa eventów
};

#endif // MAINWINDOW_H
