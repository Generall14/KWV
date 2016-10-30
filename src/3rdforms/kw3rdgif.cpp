#include "kw3rdgif.h"

#include <QDebug>
#include <QLayout>
#include <QBoxLayout>
#include <QLabel>
#include <QLine>
#include <QSpacerItem>

KW3rdGif::KW3rdGif(KWGraphicsView* gg):
    QDialog(gg),
    GV(gg)
{
    InitWidgets();                                                                  //Tworzenie widgetów
    InitConnections();                                                              //Tworzenie połączeń
    Update();                                                                       //Uaktualnianie parametrów

    setWindowTitle("Gif manager");                                                  //Ustawianie napisu na belce okna
}

KW3rdGif::~KW3rdGif()
{

}

void KW3rdGif::InitWidgets()
{
    QVBoxLayout* mainLay = new QVBoxLayout(this);                                   //Główny layout

    //Sposób odtwarzania
    cycCheck = new QCheckBox(tr("Odtwarzanie cykliczne / kołowe"), this);           //Zmiana trybu odtwarzania
    mainLay->addWidget(cycCheck);

    //Prędkość odtwarzania
    QVBoxLayout* speedLay = new QVBoxLayout();
    mainLay->addLayout(speedLay);

    QHBoxLayout* speedSliderLay2 = new QHBoxLayout();
    speedLay->addLayout(speedSliderLay2);

    QLabel* speedLab = new QLabel(tr("Czas wyświetlania jednej klatki:"), this);
    speedSliderLay2->addWidget(speedLab);

    speedButtonDef = new QToolButton(this);                                         //Domyślna prędkość odtwarzania
    speedButtonDef->setIcon(QIcon(":/ics/defSpeed.ico"));
    speedSliderLay2->addWidget(speedButtonDef);

    QHBoxLayout* speedSliderLay = new QHBoxLayout();
    speedLay->addLayout(speedSliderLay);

    speedSlider = new QSlider(Qt::Horizontal, this);                                //Zmiana prędkości odtwarzania
    speedSlider->setInvertedAppearance(false);
    speedSlider->setMinimum(1);//<REF>
    speedSlider->setMaximum(500);//<REF>
    speedSliderLay->addWidget(speedSlider);

    speedLabel = new QLabel("- ms", this);                                          //Wyświetlanie prędkości odtwarzania
    speedLabel->setMinimumWidth(53);
    speedLabel->setAlignment(Qt::AlignRight);
    speedSliderLay->addWidget(speedLabel);

    //Sterowanie
    QHBoxLayout* sterLay = new QHBoxLayout();
    mainLay->addLayout(sterLay);

    QSpacerItem* spacer = new QSpacerItem(2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum);
    sterLay->addItem(spacer);

    sterBack = new QToolButton(this);                                               //Poprzednia klatka
    sterBack->setIcon(QIcon(":/ics/prevFrame.ico"));
    sterLay->addWidget(sterBack);

    sterPlay = new QToolButton(this);                                               //Odtwarzanie / zatrzymanie
    sterPlay->setIcon(QIcon(":/ics/pause.ico"));
    sterLay->addWidget(sterPlay);

    sterNext = new QToolButton(this);                                               //Kolejna klatka
    sterNext->setIcon(QIcon(":/ics/nextFrame.ico"));
    sterLay->addWidget(sterNext);

    spacer = new QSpacerItem(2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum);
    sterLay->addItem(spacer);
}

void KW3rdGif::InitConnections()
{
    connect(speedSlider, SIGNAL(valueChanged(int)), GV, SLOT(GifSetSpeed(int)));    //Zmiana prędkości
    connect(speedSlider, SIGNAL(valueChanged(int)), this, SLOT(SpeedLabel(int)));

    connect(cycCheck, SIGNAL(toggled(bool)), GV, SLOT(GifSetTryb(bool)));           //Zmiana trybu

    connect(speedButtonDef, SIGNAL(clicked(bool)), this, SLOT(SpeedDef()));         //Domyslna prędkość

    connect(sterPlay, SIGNAL(clicked(bool)), GV, SLOT(GifPlayPause()));             //Sterowanie odtwarzaniem
    connect(sterBack, SIGNAL(clicked(bool)), GV, SLOT(GifBack()));
    connect(sterNext, SIGNAL(clicked(bool)), GV, SLOT(GifNext()));

    connect(GV, SIGNAL(Nowy()), this, SLOT(Update()));                              //Nowy obraz
}

void KW3rdGif::SpeedDef()
{
    speedSlider->setValue(100);//<REF>
}

void KW3rdGif::Update()
{
//    speedSlider->setValue(GV->DefDelay());
//    cycCheck->setChecked(GV->GifTryb());
//    this->setEnabled(GV->Animacja());
}

void KW3rdGif::SpeedLabel(int s)
{
    speedLabel->setText(QString::number(s)+" ms");
}
