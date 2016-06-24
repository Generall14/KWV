#ifndef KWMOTOR_H
#define KWMOTOR_H

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
    KWMotor(KWGraphicsView* gv, QObject* parent=0);

    QString Adres();                                                                                                        //Zwraca adres aktualnie otwartego pliku
    bool isOpened();                                                                                                        //Zwraca true jeżeli jest wczytany jakiś plik
    int DlugoscListy();                                                                                                     //Zwraca liczbę zdjęć na liście

    long getTimeUs();                                                                                                       //Zwraca czas otwierania ostatniego pliku

    QFileInfo fileInfo() const;                                                                                             //Zwraca informację o pliku
    QPixmap fileSample() const;                                                                                             //Zwraca jeden obraz
    int fileFrames() const;                                                                                                 //Zwraca liczbę klatek

private:
    KWGraphicsView* GV;                                                                                                     //Wskaźnik na obiekt wyświetlający obrazy

    QStringList obslugiwane;                                                                                                //Lista obsługiwanych rozszerzeń
    QStringList filtry;                                                                                                     //FIltry otwierania olików
    QString filtr;                                                                                                          //Ostateczny filtr

    //QPixmap obraz;                                                                                                          //Aktualny obraz
    QVector<QPixmap> gifVec;                                                                                                //Obrazy z animacji

    //-----------------------------------------------Zmienne opisujące zestaw plików----------------------------------------
    QDir katalog;                                                                                                           //Aktualny katalog
    QStringList pliki;                                                                                                      //Lista dostępnych plików w katalogu
    QFileInfo plik;                                                                                                         //Dane o aktualnym pliku
    int aktualny;                                                                                                           //Aktualny plik na liście

    std::chrono::steady_clock::time_point czas_start, czas_end;                                                             //Pomiar czasu

    void Sygnaly();                                                                                                         //Rozsyła sygnały związane ze zmianą pliku

    bool OtworzGif(QString adr);                                                                                            //Otwiera animację gif
    bool OtworzImg(QString adr);                                                                                            //Otwiera obraz
    bool OtworzPlik(QString adr);                                                                                           //Otwiera plik

signals:
    void Rozdzielczosc(int w, int h, int k, int d);                                                                         //Zmiana rozdzielczości otwartego obrazu
    void Licznik(int c, int a);                                                                                             //Zmiana wartości licznika plików
    void Data(QDateTime);                                                                                                   //Zmiana daty
    void Rozmiar(int KiB);                                                                                                  //Zmiana rozmiaru
    void Plik(QString p);                                                                                                   //Zmiana pliku
    void NewOpened(QString ad);                                                                                             //Nowo otwarty plik (tylko poprzez Plik -> Otwórz)
    void FileOn();                                                                                                          //Otwarto plik
    void FileOff();                                                                                                         //Zamknięto plik

    void Error(QString);                                                                                                    //Zgłoszenie błędu

public slots:
    void Otworz(QString adres) throw(QString);                                                                              //Otwiera nowy plik
    void Otworz();                                                                                                          //Wywołuje procedurę otwierania pliku
    void Next();                                                                                                            //Następny plik na liście
    void Back();                                                                                                            //Poprzedni plik na liście
    void Otworz(int nr);                                                                                                    //Otwiera plik na podstawie numeru na liście
    void RandImg();                                                                                                         //Otwiera losowy obraz
};

#endif // KWMOTOR_H
