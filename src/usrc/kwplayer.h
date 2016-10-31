#ifndef KWPLAYER_H
#define KWPLAYER_H

//Automatycznie tworzy połączenia z KWZoomer

#include <QObject>
#include <QTimer>
#include "kwpic.h"
#include "kwzoomer.h"

class KWPlayer : public QObject
{
    Q_OBJECT
public:
    KWPlayer(KWPic* pic, KWZoomer* zoomerPtr, QObject* parent = 0);
    ~KWPlayer();

    static const int FASTEST_GIF = 5;
    static const int SLOWEST_GIF = 200;

    int getFrameDelay() const;
    int getCyclic() const;
    int getReverse() const;
    bool isGif() const;

public slots:
    void NewPic(const KWPicInfo *pi);                                                                                       //Wyświetlaj nowy obraz
    void NextFrame();                                                                                                       //Kolejna klatka
    void PrevFrame();                                                                                                       //Poprzednia klatka

    void setDelay(int newDelayMs);
    void setCyclic(bool newCyclic);
    void setReverse(bool newReverse);
    int RestoreDefaultFrameDelay();
    void PlayPause();

signals:
    void LoadedNewPic();                                                                                                    //Rozgłoszenie otrzymania nowego obrazu
    void ShowNewPic(QPixmap* newPic, bool resetZoom);                                                                       //Sygnał do KWZoomer ze wskaźnikiem do obrazu

private:
    KWPic* picPtr;                                                                                                          //Wskaźnik do KWPic z którego będą pobierane obrazy
    QTimer* timer;                                                                                                          //Timer do przełączania klatek

    int frames;                                                                                                             //Liczba klatek
    int currFrame;                                                                                                          //Aktualna klatka
    int delay;                                                                                                              //Aktualne opóźnienie klatki
    int defDelay;                                                                                                           //Domyślne opóźnienie klatki
    bool cicle;                                                                                                             //Flaga odtwarzania cyklicznego
    bool right;                                                                                                             //Kierunek odtwarzania cyklicznego
    bool reverse;                                                                                                           //Odwrócony kierunek odtwarzania standardowego

    void Halt();                                                                                                            //Reset parametrów obrazu
};

#endif // KWPLAYER_H
