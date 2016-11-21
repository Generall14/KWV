#include "kwpic.h"
#include <QDebug>

KWPic::KWPic(QObject *parent):
    QObject(parent)
{

}

KWPic::~KWPic()
{
    ClearGifVec();
}

void KWPic::ClearGifVec()
{
    for(int i=0;i<gifVec.length();++i)
    {
        if(gifVec.at(i)!=NULL)
            delete gifVec[i];
    }
    gifVec.clear();
}

void KWPic::LoadFile(QString adres, int orderId)
{
//    qDebug() << "KWPic: LoadFile slot";
    if(OtworzPlik(adres))
    {
        this->CalcPicInfo(adres);
        emit Error(&picInfo, tr("Nie można otworzyć pliku ") + adres + ".", orderId);
        return;
    }

    this->CalcPicInfo(adres);
    emit Done(&picInfo, orderId);
}

void KWPic::ClearData(int orderId)
{
    ClearGifVec();
    this->CalcPicInfo("");
    emit Done(&picInfo, orderId);
}

QPixmap* KWPic::GetFrame(int nrFrame)
{
    if((nrFrame<gifVec.length()&&(nrFrame>=0)))
        return gifVec[nrFrame];
    return NULL;
}

const KWPicInfo* KWPic::GetPicInfo()
{
    return &picInfo;
}

bool KWPic::OtworzPlik(QString adr)
{
    czas_start = std::chrono::steady_clock::now();                                  //Pomiar czasu

    ClearGifVec();
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

bool KWPic::OtworzImg(QString adr)
{
    QPixmap* tpm = new QPixmap(adr);                                                //Odczytywanie obrazu
    //ClearGifVec();

    if(tpm->isNull())
    {
        delete tpm;
        return 666;
    }

    gifVec.push_back(tpm);
    tv = 0;
    return 0;
}

bool KWPic::OtworzGif(QString adr)
{
    QMovie* mv = new QMovie(adr);                                                   //Otwieranie pliku gif za pomocą QMovie

    if(mv->isValid())                                                               //Poprawny obiekt?
    {
        //ClearGifVec();

        gifVec.reserve(mv->frameCount());
        for(int i=0;i<mv->frameCount();++i)                                         //Zapisywanie klatek do wektora
        {
            mv->jumpToFrame(i);
            gifVec.push_back(new QPixmap(mv->currentPixmap()));
        }
        delete mv;                                                                  //Niszczenie obiektu QMovie
        tv= mv->nextFrameDelay();                                                   //Odczytywanie czasu klatki
        return 0;
    }
    delete mv;
    return 666;
}

void KWPic::CalcPicInfo(QString adres)
{
    picInfo.Clear();
    picInfo.fileInfo.setFile(adres);

    if(gifVec.isEmpty())
        return;

    picInfo.isOk = true;
    if(gifVec.length()>1)
    {
        picInfo.isGif = true;
        picInfo.frames = gifVec.length();
        picInfo.frameDelay = tv;
    }
    picInfo.resHeight = gifVec[0]->height();
    picInfo.resWidth = gifVec[0]->width();
    picInfo.picDepth = gifVec[0]->defaultDepth();
    picInfo.loadTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(czas_end - czas_start).count();
}
