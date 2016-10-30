#ifndef KWWINDOWZOOMER_H
#define KWWINDOWZOOMER_H

#include <QObject>
#include <QRect>
#include <QPoint>
#include <QSize>
#include "../osrc/mwptr.h"

class MainWindow;

class KWWindowZoomer : public QObject, public MWPtr
{
    Q_OBJECT
public:
    explicit KWWindowZoomer(MainWindow* mw);

public slots:
    void InitGeometry();

    void setNormalMode();
    void setMaximizeMode();
    void setFullscreenMode();

signals:
    void setMaximumPicSize(QSize size);

private:
    QSize fullResolution;                                                                                                   //Dostępna rozdzielczość ekranu
    QSize maxWindowSize;                                                                                                    //Maksymalny rozmiar okna (belka nie jest częścią okna)
    QSize maxPicSize;                                                                                                       //Maksymalny rozmiar obrazu
    QSize maxPicSizeMaximized;                                                                                              //Maksymalny rozmiar obrazu w trybie zmaksymalizowanym
    int deltaHeight;                                                                                                        //Straty w wysokości
    int deltaWidth;                                                                                                         //Straty w szerokośco
    QPoint zeroPoint;                                                                                                       //Punkt początkowy okna

    enum screenMode{normalMode, maximizedMode, fullscreenMode};

    screenMode currentMode;
};

#endif // KWWINDOWZOOMER_H
