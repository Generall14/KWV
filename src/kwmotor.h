#ifndef KWMOTOR_H
#define KWMOTOR_H

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

    QString Adres();                                                                                                        //Zwraca adres aktualnie otwartego pliku<REF>
    bool isOpened();                                                                                                        //Zwraca true jeżeli jest wczytany jakiś plik<REF>
    int DlugoscListy();                                                                                                     //Zwraca liczbę zdjęć na liście<REF>

    long getTimeUs();                                                                                                       //Zwraca czas otwierania ostatniego pliku<REF>

    QFileInfo fileInfo() const;                                                                                             //Zwraca informację o pliku<REF>
    QPixmap fileSample() const;                                                                                             //Zwraca jeden obraz<REF>
    int fileFrames() const;                                                                                                 //Zwraca liczbę klatek<REF>

    QString Filters();                                                                                                      //Zwraca filtry plików

private:
    KWGraphicsView* GV;                                                                                                     //Wskaźnik na obiekt wyświetlający obrazy<REF>

    QStringList obslugiwane;                                                                                                //Lista obsługiwanych rozszerzeń
    QStringList filtry;                                                                                                     //FIltry otwierania olików
    QString filtr;                                                                                                          //Ostateczny filtr

    //QPixmap obraz;                                                                                                          //Aktualny obraz
    QVector<QPixmap> gifVec;                                                                                                //Obrazy z animacji<REF>

    KWPic obraz;                                                                                                            //Obiekt obrazu<REF>

    QString lastRequestPath;                                                                                                //Po zleceniu odczytu poprzez wskazanie adresu
    int lastRequestNumber;                                                                                                  //Po zleceniu odczytu next/back

    //-----------------------------------------------Zmienne opisujące zestaw plików----------------------------------------
    QDir katalog;                                                                                                           //Aktualny katalog
    QStringList pliki;                                                                                                      //Lista dostępnych plików w katalogu
    QFileInfo plik;                                                                                                         //Dane o aktualnym pliku
    int aktualny;                                                                                                           //Aktualny plik na liście

    std::chrono::steady_clock::time_point czas_start, czas_end;                                                             //Pomiar czasu<REF>

    void Sygnaly();                                                                                                         //Rozsyła sygnały związane ze zmianą pliku<REF>

    bool OtworzGif(QString adr);                                                                                            //Otwiera animację gif<REF>
    bool OtworzImg(QString adr);                                                                                            //Otwiera obraz<REF>
    bool OtworzPlik(QString adr);                                                                                           //Otwiera plik<REF>

    void CalcFilesParams(QString adres);                                                                                    //Oblicza parametry plików w folderze

signals:
    void Rozdzielczosc(int w, int h, int k, int d);                                                                         //Zmiana rozdzielczości otwartego obrazu<REF>
    void Licznik(int c, int a);                                                                                             //Zmiana wartości licznika plików<REF>
    void Data(QDateTime);                                                                                                   //Zmiana daty<REF>
    void Rozmiar(int KiB);                                                                                                  //Zmiana rozmiaru<REF>
    void Plik(QString p);                                                                                                   //Zmiana pliku<REF>
    //void NewOpened(QString ad);                                                                                             //Nowo otwarty plik (tylko poprzez Plik -> Otwórz)
    void FileOn();                                                                                                          //Otwarto plik<REF>
    void FileOff();                                                                                                         //Zamknięto plik<REF>

    void Error(QString);                                                                                                    //Zgłoszenie błędu

    void LoadRequest(QString adres, int orderId);                                                                                        //Żądanie odczytu
    void NewLoaded(const KWPicInfo *pi, int curr, int total);

public slots:
    void Otworz(QString adres) throw(QString);                                                                              //Otwiera nowy plik
    //void Otworz();                                                                                                          //Wywołuje procedurę otwierania pliku
    void Clear();                                                                                                           //Zamyka plik i czyści pamięć<REF>
    void Next();                                                                                                            //Następny plik na liście
    void Back();                                                                                                            //Poprzedni plik na liście
    void Otworz(int nr);                                                                                                    //Otwiera plik na podstawie numeru na liście
    void RandImg();                                                                                                         //Otwiera losowy obraz

    void PicDone(const KWPicInfo *pi, int orderId);                                                                                      //Odebrano potwierdzenie otwarcia pliku
    void PicError(QString errorMsg, int orderId);                                                                                        //Odebrano błąd otwarcia pliku
};

#endif // KWMOTOR_H
