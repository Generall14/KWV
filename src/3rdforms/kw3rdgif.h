#ifndef KW3RDGIF_H
#define KW3RDGIF_H

#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QToolButton>
#include <QCheckBox>

#include "../kwgraphicsview.h"

class KW3rdGif : public QDialog
{
    Q_OBJECT
public:
    KW3rdGif(KWGraphicsView* gg);                                                                                           //Konstruktor
    ~KW3rdGif();                                                                                                            //Destruktor

private:
    KWGraphicsView* GV;                                                                                                     //Wskaźnik na wyświetlacz obrazów

    QSlider* speedSlider = NULL;                                                                                            //Slider prędkości odtwarzania
    QLabel* speedLabel = NULL;                                                                                              //Wyświetlanie prędkości odtwarzania
    QToolButton* speedButtonDef = NULL;                                                                                     //Resetowanie prędkości odtwarzania

    QCheckBox* cycCheck = NULL;                                                                                             //Ustawianie trybu odtwarzania

    QToolButton* sterBack = NULL;                                                                                           //Poprzednia klatka
    QToolButton* sterNext = NULL;                                                                                           //Następna klatka
    QToolButton* sterPlay = NULL;                                                                                           //Pauza

    void InitWidgets();                                                                                                     //Tworzenie widgetów<REF>
    void InitConnections();                                                                                                 //Tworzenie połączeń

private slots:
    void SpeedLabel(int s);                                                                                                 //Wyświetlanie napisu z prędkością odtwarzania

public slots:
    void Update();                                                                                                          //Uaktualnienie danych<REF>
    void SpeedDef();                                                                                                        //Domyślna prędkość<REF>
};

#endif // KW3RDGIF_H
