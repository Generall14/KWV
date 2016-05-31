#include "kwmotor.h"
#include "kwgraphicsview.h"

#include <QDebug>

KWMotor::KWMotor(KWGraphicsView *gv, QObject *parent):
    QObject(parent),
    GV(gv)
{
    filtry << tr("Pliki jpg") + " (*.jpg)"                                          //Filtry obsługiwanych plików
           << tr("Pliki jpeg") + " (*.jpeg)"
           << tr("Pliki gif") + " (*.gif)"
           << tr("Pliki bmp") + " (*.bmp)"
           << tr("Pliki png") + " (*.png)"
           << tr("Pliki pbm") + " (*.pbm)"
           << tr("Pliki pgm") + " (*.pgm)"
           << tr("Pliki ppm") + " (*.ppm)"
           << tr("Pliki xbm") + " (*.xbm)"
           << tr("Pliki xpm") + " (*.xpm)"
           << tr("Pliki ico") + " (*.ico)";

    for(int i=0;i<filtry.length();i++)                                              //Tworzenie listy obsługiwanych rozszerzeń
    {
        QStringList tempList = filtry.at(i).split(".");
        QString tempString = tempList.last();
        obslugiwane << tempString.left(tempString.length()-1);
    }

    filtr = tr("Wszystkie obsługiwane pliki")+" (";                                 //Tworzenie całkowitego filtra
    for(int i=0;i<obslugiwane.length();i++)
        filtr += "*." + obslugiwane.at(i) + " ";
    filtr += ")";
    for(int i=0;i<filtry.length();i++)
        filtr += ";;" + filtry.at(i);

    katalog.setFilter(QDir::Files | QDir::Hidden);                                  //Ustawienia ładowania katalogów
}

void KWMotor::Otworz(QString adres) throw(QString)
{
    plik.setFile(adres);                                                            //Ładowanie opisu pliku

    katalog.setPath(plik.path());                                                   //Ładowanie katalogu

    pliki.clear();                                                                  //Czyszczenie listy plików

    QFileInfoList lista = katalog.entryInfoList();                                  //Lista wszystkich plików w folderze
    for(int i=0;i<lista.size();++i)                                                 //Wybieranie plików z obsługiwanymi rozszerzeniami i tworzenie z nich nowej listy plików
    {
        QFileInfo fileInfo = lista.at(i);
        if(obslugiwane.contains(fileInfo.suffix(), Qt::CaseInsensitive))
            pliki.push_back(fileInfo.fileName());
    }

    for(int i=0;i<pliki.length();++i)                                               //Ustalanie numeru aktualnego pliku
    {
        if(pliki[i]==plik.fileName())
        {
            aktualny = i;
            break;
        }
    }

    OtworzPlik(adres);                                                              //Otwieranie pliku

    Sygnaly();                                                                      //Rozsyłanie sygnałów
}

bool KWMotor::OtworzPlik(QString adr)
{
    czas_start = std::chrono::steady_clock::now();                                  //Pomiar czasu
    if(adr.split('.').last()=="gif")                                                //Odczytywanie rozszerzenia
    {
        if(OtworzGif(adr))                                                          //Jeżeli plik ma rozszerzenie *.gif
            return 666;
    }
    else
    {
        if(OtworzImg(adr))                                                          //Dla pozostałych plików
            return 666;
    }
    czas_end = std::chrono::steady_clock::now();                                    //Pomiar czasu
    return 0;
}

bool KWMotor::OtworzGif(QString adr)
{
    QMovie* mv = new QMovie(adr);                                                   //Otwieranie pliku gif za pomocą QMovie

    if(mv->isValid())                                                               //Poprawny obiekt?
    {
        gifVec.clear();                                                             //Czyszczenie wektora
        for(int i=0;i<mv->frameCount();++i)                                         //Zapisywanie klatek do wektora
        {
            mv->jumpToFrame(i);
            gifVec.push_back(mv->currentPixmap());
        }
        delete mv;                                                                  //Niszczenie obiektu QMovie
        int tv= mv->nextFrameDelay();                                               //Odczytywanie czasu klatki
        if(tv>0)
        {
            GV->Wyswietl(gifVec, tv);                                               //Wyświetlanie animacji
            return 0;
        }
        else return 666;
    }
    GV->Wyswietl(gifVec);
    emit Error("Nie można otworzyć pliku " + adr + ".");                            //Zgłoszenie błędu
    delete mv;
    return 666;
}

bool KWMotor::OtworzImg(QString adr)
{
    gifVec.clear();                                                                 //Czyszczenie wektora
    gifVec.push_back(QPixmap(adr));                                                 //Odczytywanie obrazu
    if(gifVec.at(0).isNull())                                                       //Sprawdzanie poprawności
    {
        GV->Wyswietl(gifVec);
        emit Error("Nie można otworzyć pliku " + adr + ".");                        //Zgłaszanie błędu
        return 666;
    }
    GV->Wyswietl(gifVec);                                                           //Wyświetlanie obrazu
    return 0;
}

void KWMotor::Otworz()
{
    QString adres = QFileDialog::getOpenFileName(0, tr("Otwórz plik"), "", filtr);  //Wyświetlanie okna wybierania pliku
    if(!adres.isEmpty())
        this->Otworz(adres);
}

void KWMotor::Sygnaly()
{
    int bpp = 0;
    if(!gifVec.at(0).hasAlpha())
        bpp = gifVec.at(0).depth()*75/100;
    else
        bpp = gifVec.at(0).depth();
    emit Rozdzielczosc(gifVec.at(0).width(), gifVec.at(0).height(), gifVec.length()>1?gifVec.length():0, bpp);//Zmiana rozdzielczości
    emit Licznik(aktualny+1, pliki.length());                                       //Zmiana listy plików
    emit Data(plik.created());                                                      //Zmiana daty
    emit Rozmiar(plik.size()/1024);                                                 //Zmiana rozmiaru
    emit Plik(plik.fileName());                                                     //ZMiana nazwy pliku
}

void KWMotor::Next()
{
    if(pliki.length()<=1)                                                           //Opcja wyłączona dla list jednoelementowych i pustych
        return;

    int next = aktualny + 1;                                                        //Nowy indeks
    if(next>=pliki.length())                                                        //Sprawdzanie czy indeks nie przekroczył zakresu
        next = 0;

    QString adres = katalog.absolutePath() + "/" + pliki[next];                     //Pełen adres nowego pliku

    OtworzPlik(adres);                                                              //Otwieranie pliku

    aktualny = next;                                                                //Ustawianie znacznika

    plik.setFile(adres);                                                            //Pobieranie informacji o nowym pliku

    Sygnaly();                                                                      //Rozsyłanie sygnałów
}

void KWMotor::Back()
{
    if(pliki.length()<=1)                                                           //Opcja wyłączona dla list jednoelementowych i pustych
        return;

    int next = aktualny - 1;                                                        //Nowy indeks
    if(next<0)                                                                      //Sprawdzanie czy indeks nie przekroczył zakresu
        next = pliki.length()-1;

    QString adres = katalog.absolutePath() + "/" + pliki[next];                     //Pełen adres nowego pliku

    OtworzPlik(adres);                                                              //Otwieranie pliku

    aktualny = next;                                                                //Ustawianie znacznika

    plik.setFile(adres);                                                            //Pobieranie informacji o nowym pliku

    Sygnaly();                                                                      //Rozsyłanie sygnałów
}

void KWMotor::Otworz(int nr)
{
    if((nr<0)||(nr>=pliki.length())||(nr==aktualny))                                //Sprawdzanie warunków
        return;

    QString adres = katalog.absolutePath() + "/" + pliki[nr];                       //Pełen adres nowego pliku

    OtworzPlik(adres);                                                              //Otwieranie pliku

    aktualny = nr;                                                                  //Ustawianie znacznika

    plik.setFile(adres);                                                            //Pobieranie informacji o nowym pliku

    Sygnaly();                                                                      //Rozsyłanie sygnałów
}

void KWMotor::RandImg()
{
    this->Otworz((aktualny+qrand())%pliki.length());
}

QString KWMotor::Adres()
{
    if(plik.exists())
        return plik.filePath();
    return QString();
}

bool KWMotor::isOpened()
{
    return plik.exists();
}

int KWMotor::DlugoscListy()
{
    return pliki.length();
}

long KWMotor::getTimeUs()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(czas_end - czas_start).count();
}
