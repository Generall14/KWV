#include "kwmotor.h"
#include "kwgraphicsview.h"

#include <QDebug>

KWMotor::KWMotor(KWPic *kp, QObject *parent):
    QObject(parent),
    GV(NULL)
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

    connect(this, SIGNAL(LoadRequest(QString, int)), kp, SLOT(LoadFile(QString, int)));
    connect(this, SIGNAL(ClearRequest(int)), kp, SLOT(ClearData(int)));
    connect(kp, SIGNAL(Done(const KWPicInfo*, int)), this, SLOT(PicDone(const KWPicInfo*, int)));
    connect(kp, SIGNAL(Error(const KWPicInfo*, QString, int)), this, SLOT(PicError(const KWPicInfo*, QString, int)));
}

void KWMotor::CalcFilesParams(QString adres)
{
    plik.setFile(adres);                                                            //Ładowanie opisu pliku

    katalog.setPath(plik.path());                                                   //Ładowanie katalogu

    pliki.clear();                                                                  //Czyszczenie listy plików

    if(adres.isEmpty())
        return;

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
}

void KWMotor::Otworz(QString adres) throw(QString)
{
    lastRequestPath = adres;
    lastRequestNumber = -1;
    emit LoadRequest(adres, 0);
}

void KWMotor::Clear()
{
    lastRequestPath.clear();
    lastRequestNumber = -1;
    emit ClearRequest(0);
}

void KWMotor::Next()
{
    if(pliki.length()<=1)                                                           //Opcja wyłączona dla list jednoelementowych i pustych
        return;

    int next = aktualny + 1;                                                        //Nowy indeks
    if(next>=pliki.length())                                                        //Sprawdzanie czy indeks nie przekroczył zakresu
        next = 0;

    QString adres = katalog.absolutePath() + "/" + pliki[next];                     //Pełen adres nowego pliku

    lastRequestPath.clear();
    lastRequestNumber = next;
    emit LoadRequest(adres, 0);
}

void KWMotor::Back()
{
    if(pliki.length()<=1)                                                           //Opcja wyłączona dla list jednoelementowych i pustych
        return;

    int next = aktualny - 1;                                                        //Nowy indeks
    if(next<0)                                                                      //Sprawdzanie czy indeks nie przekroczył zakresu
        next = pliki.length()-1;

    QString adres = katalog.absolutePath() + "/" + pliki[next];                     //Pełen adres nowego pliku

    lastRequestPath.clear();
    lastRequestNumber = next;
    emit LoadRequest(adres, 0);
}

void KWMotor::Otworz(int nr)
{
    if((nr<0)||(nr>=pliki.length())||(nr==aktualny))                                //Sprawdzanie warunków
        return;

    QString adres = katalog.absolutePath() + "/" + pliki[nr];                       //Pełen adres nowego pliku

    lastRequestPath.clear();
    lastRequestNumber = nr;
    emit LoadRequest(adres, 0);
}

void KWMotor::RandImg()
{
    if(pliki.length()<2)
        return;
    this->Otworz((aktualny+qrand())%pliki.length());
}

int KWMotor::DlugoscListy()
{
    return pliki.length();
}

QString KWMotor::Filters()
{
    return filtr;
}

void KWMotor::PicDone(const KWPicInfo *pi, int orderId)
{
    if(lastRequestNumber>=0)
    {
        aktualny = lastRequestNumber;
    }
    else if(!lastRequestPath.isEmpty())
    {
        CalcFilesParams(lastRequestPath);
    }
    else
        CalcFilesParams("");

    emit NewLoaded(pi, aktualny+1, pliki.length());
    if(pi->isOk)
    {
        emit FileOn();
    }
    else
        emit FileOff();

    lastRequestNumber = -1;
    lastRequestPath.clear();
}

void KWMotor::PicError(const KWPicInfo *pi, QString errorMsg, int orderId)
{
    if(lastRequestNumber>=0)
    {
        aktualny = lastRequestNumber;
        emit NewLoaded(pi, aktualny+1, pliki.length());
        emit FileOff();
    }
    emit Error(errorMsg);
    emit FileOff();
}
