#ifndef KWMOTOR_H
#define KWMOTOR_H

/**
 * \class KWMotor
 */
//Automatycznie tworzy połączenia z KWPic podanym w konstruktorze

#include "usrc/kwpic.h"
#include <QObject>
#include <QDir>
#include <QStringList>
#include <QFile>
#include <QPixmap>
#include <QFileInfo>
#include <QDateTime>
#include <QFileDialog>
#include <QVector>
#include <QMovie>
#include <chrono>

class KWGraphicsView;

class KWMotor : public QObject
{
    Q_OBJECT
public:
    KWMotor(KWPic* kp, QObject* parent=0);

    int DlugoscListy();                                                                                                     //Zwraca liczbę zdjęć na liście

    QString Filters();                                                                                                      //Zwraca filtry plików

private:
    KWGraphicsView* GV;                                                                                                     //Wskaźnik na obiekt wyświetlający obrazy

    QStringList obslugiwane;                                                                                                //Lista obsługiwanych rozszerzeń
    QStringList filtry;                                                                                                     //FIltry otwierania olików
    QString filtr;                                                                                                          //Ostateczny filtr

    QString lastRequestPath;                                                                                                //Po zleceniu odczytu poprzez wskazanie adresu
    int lastRequestNumber;                                                                                                  //Po zleceniu odczytu next/back

    //-----------------------------------------------Zmienne opisujące zestaw plików----------------------------------------
    QDir katalog;                                                                                                           //Aktualny katalog
    QStringList pliki;                                                                                                      //Lista dostępnych plików w katalogu
    QFileInfo plik;                                                                                                         //Dane o aktualnym pliku
    int aktualny;                                                                                                           //Aktualny plik na liście

    void CalcFilesParams(QString adres);                                                                                    //Oblicza parametry plików w folderze

signals:
    void FileOn();                                                                                                          //Otwarto plik<OK>
    void FileOff();                                                                                                         //Zamknięto plik<OK>

    void Error(QString);                                                                                                    //Zgłoszenie błędu<OK>

    void LoadRequest(QString adres, int orderId);                                                                           //Żądanie odczytu (do KWPic)
    void ClearRequest(int orderId);                                                                                         //Żądanie zamknięcia pliów (do KWPic)
    void NewLoaded(const KWPicInfo *pi, int curr, int total);                                                               //Rozsyła informacje o nowo otawrtym pliku

public slots:
    void Otworz(QString adres) throw(QString);                                                                              //Otwiera nowy plik<OK>
    void Clear();                                                                                                           //Zamyka plik i czyści pamięć
    void Next();                                                                                                            //Następny plik na liście<OK>
    void Back();                                                                                                            //Poprzedni plik na liście<OK>
    void Otworz(int nr);                                                                                                    //Otwiera plik na podstawie numeru na liście<OK>
    void RandImg();                                                                                                         //Otwiera losowy obraz<OK>

    void PicDone(const KWPicInfo *pi, int orderId);                                                                         //Odebrano potwierdzenie otwarcia pliku (z KWPic)
    void PicError(QString errorMsg, int orderId);                                                                           //Odebrano błąd otwarcia pliku (z KWPic)
};

#endif // KWMOTOR_H
