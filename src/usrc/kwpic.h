#ifndef KWPIC_H
#define KWPIC_H

/**
 * \class KWPic
 * @brief Klasa zarządzająca otwartymi plikami obrazów.
 *
 * Przechowuje obrazy w wektorze KWPic::gifVec (obraz lub klatki animacji), zwraca obrazy poprzez funkcję KWPic::GetFrame.
 *
 * Zlecenie otwarcia pliku odbywa się poprzez funkcje KWPic::LoadFile, po zakończeniu operacji jest wysyłany sygnał KWPic::Done lub KWPic::Error w zależności od wyniku.
 */

#include <QObject>
#include <QVector>
#include <QPixmap>
#include <QMovie>
#include <chrono>
#include <QFileInfo>
#include "kwpicinfo.h"

class KWPic : public QObject
{
    Q_OBJECT
public:
    KWPic(QObject *parent = 0);
    ~KWPic();

    QPixmap* GetFrame(int nrFrame=0);                                                                                       //Zwraca wskaźnik na klatkę
    const KWPicInfo* GetPicInfo();                                                                                          //Zwraca informacje o obrazie

public slots:
    void LoadFile(QString adres, int orderId);                                                                              //Załaduj plik z adresu
    void ClearData(int orderId);                                                                                            //Czyści dane

signals:
    void Done(const KWPicInfo *pi, int orderId);                                                                            //Prawidłowe odczytanie pliku
    void Error(QString errorMsg, int orderId);                                                                              //Błędne odczytanie pliku

private:
    QVector<QPixmap*> gifVec;                                                                                               //Obraz lub klatki animacji
    int tv;                                                                                                                 //Czas klatki
    KWPicInfo picInfo;                                                                                                      //Informacje o obrazie

    std::chrono::steady_clock::time_point czas_start, czas_end;                                                             //Pomiar czasu

    void ClearGifVec();                                                                                                     //Czyści gifVec

    bool OtworzPlik(QString adr);                                                                                           //Rozpoznanie i otwieranie pliku
    bool OtworzImg(QString adr);                                                                                            //Otwieranie plików obrazu
    bool OtworzGif(QString adr);                                                                                            //Otwieranie pliku gif

    void CalcPicInfo(QString adres);                                                                                        //Oblicza informacje o pliku
};

#endif // KWPIC_H
