#ifndef KWPICINFO_H
#define KWPICINFO_H

#include <QFileInfo>

class KWPicInfo
{
public:
    KWPicInfo();

    void Clear();                                                                                                           //Czyści informacje

    QFileInfo fileInfo;                                                                                                     //Informacje o pliku
    bool isGif;                                                                                                             //Plik jest animacją
    int frames;                                                                                                             //Liczba klatek animacji
    int frameDelay;                                                                                                         //Opóźnienie klatki
    int resWidth;                                                                                                           //Szerokość obrazu
    int resHeight;                                                                                                          //Wysokość obrazu
    int picDepth;                                                                                                           //Głębia obrazu
    int loadTimeUs;                                                                                                         //Czas ładowania obrazu w us
};

#endif // KWPICINFO_H