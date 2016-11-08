#include "kw3rdfileinfo.h"

#include <QLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QLabel>
#include <QDir>
#include <algorithm>
#include <QDateTime>

KW3rdFileInfo::KW3rdFileInfo(QWidget *parent):
    QDialog(parent)
{
    InitWidgets();
    this->setWindowTitle(tr("Informacje o obrazie"));
}

void KW3rdFileInfo::GenInfo(const KWPicInfo* pi)
{
    //QFileInfo fi, QPixmap ob, int kl, int ms
    if(pi->isOk)
    {
        nazwaPliku->setText(pi->fileInfo.fileName());
        katalog->setText(pi->fileInfo.dir().absolutePath());
        sciezkaPliku->setText(pi->fileInfo.dir().absoluteFilePath(pi->fileInfo.fileName()));

        int w = pi->resWidth;
        int h = pi->resHeight;
        int gcd = std::__gcd(w, h);
        rozmiarObrazu->setText(QString::number(w)+" x "+QString::number(h)+tr(" pikseli")+" ("+QString::number(w/gcd)+":"+QString::number(h/gcd)+")");

        glebiaKolorow->setText(QString("%1").arg(pow(2, pi->picDepth), 0, 'E', 2)+" ("+QString::number(pi->picDepth)+" bits per pixel)");

        long long B = pi->fileInfo.size();
        int KiB = B/1024;
        if(int(KiB/1024))                                                               //Sprawdza czy wyświetlić wartość w KiB czy konwertować na MiB
            rozmiarNaDysku->setText(QString::number(float((float)KiB/1024.0), 'g', 2)+" MiB ("+QString("%L1").arg(B)+" B)");
        else
            rozmiarNaDysku->setText(QString::number(KiB)+" KiB ("+QString("%L1").arg(B)+" B)");

        dataEdycji->setText(pi->fileInfo.lastModified().toString("yyyy-MM-dd, HH:mm:ss"));
        czasLadowania->setText(QString("%L1").arg(pi->loadTimeUs/1000)+" ms");
    }
    else
    {
        nazwaPliku->setText("-");
        katalog->setText("-");
        sciezkaPliku->setText("-");
        rozmiarObrazu->setText("-");
        glebiaKolorow->setText("-");
        rozmiarNaDysku->setText("-");
        dataEdycji->setText("-");
        czasLadowania->setText("-");
    }
}

void KW3rdFileInfo::InitWidgets()
{
    QVBoxLayout* mainLay = new QVBoxLayout(this);                                   //Główny layout

    QHBoxLayout* fileLayout = new QHBoxLayout();                                    //Nazwa pliku
    QLabel* fileLabel = new QLabel(tr("Nazwa pliku:"), this);
    fileLabel->setMinimumSize(constSzer, constWys);
    fileLayout->addWidget(fileLabel);
    nazwaPliku = new QLineEdit("-", this);
    nazwaPliku->setReadOnly(true);
    nazwaPliku->setMinimumSize(constSzerT, constWys);
    fileLayout->addWidget(nazwaPliku);
    mainLay->addLayout(fileLayout);

    QHBoxLayout* katalogLayout = new QHBoxLayout();                                 //Katalog pliku
    QLabel* katalogLabel = new QLabel(tr("Katalog:"), this);
    katalogLabel->setMinimumSize(constSzer, constWys);
    katalogLayout->addWidget(katalogLabel);
    katalog = new QLineEdit("-", this);
    katalog->setReadOnly(true);
    katalog->setMinimumSize(constSzerT, constWys);
    katalogLayout->addWidget(katalog);
    mainLay->addLayout(katalogLayout);

    QHBoxLayout* sciezkaLayout = new QHBoxLayout();                                 //Pełna ścieżka
    QLabel* sciezkaLabel = new QLabel(tr("Pełen adres pliku:"), this);
    sciezkaLabel->setMinimumSize(constSzer, constWys);
    sciezkaLayout->addWidget(sciezkaLabel);
    sciezkaPliku = new QLineEdit("-", this);
    sciezkaPliku->setReadOnly(true);
    sciezkaPliku->setMinimumSize(constSzerT, constWys);
    sciezkaLayout->addWidget(sciezkaPliku);
    mainLay->addLayout(sciezkaLayout);

    QHBoxLayout* rozLayout = new QHBoxLayout();                                     //Rozmiar obrazu
    QLabel* rozLabel = new QLabel(tr("Rozmiar obrazu:"), this);
    rozLabel->setMinimumSize(constSzer, constWys);
    rozLayout->addWidget(rozLabel);
    rozmiarObrazu = new QLineEdit("-", this);
    rozmiarObrazu->setReadOnly(true);
    rozmiarObrazu->setMinimumSize(constSzerT, constWys);
    rozLayout->addWidget(rozmiarObrazu);
    mainLay->addLayout(rozLayout);

    QHBoxLayout* glebiaLayout = new QHBoxLayout();                                  //Głębia
    QLabel* glebiaLabel = new QLabel(tr("Liczba kolorów:"), this);
    glebiaLabel->setMinimumSize(constSzer, constWys);
    glebiaLayout->addWidget(glebiaLabel);
    glebiaKolorow = new QLineEdit("-", this);
    glebiaKolorow->setReadOnly(true);
    glebiaKolorow->setMinimumSize(constSzerT, constWys);
    glebiaLayout->addWidget(glebiaKolorow);
    mainLay->addLayout(glebiaLayout);

    QHBoxLayout* dyskLayout = new QHBoxLayout();                                    //Rozmiar pliku
    QLabel* dyskLabel = new QLabel(tr("Rozmiar pliku na dysku:"), this);
    dyskLabel->setMinimumSize(constSzer, constWys);
    dyskLayout->addWidget(dyskLabel);
    rozmiarNaDysku = new QLineEdit("-", this);
    rozmiarNaDysku->setReadOnly(true);
    rozmiarNaDysku->setMinimumSize(constSzerT, constWys);
    dyskLayout->addWidget(rozmiarNaDysku);
    mainLay->addLayout(dyskLayout);

    QHBoxLayout* edycjaLayout = new QHBoxLayout();                                    //Data ostatniej edycji
    QLabel* edycjaLabel = new QLabel(tr("Ostatnio modyfikowany:"), this);
    edycjaLabel->setMinimumSize(constSzer, constWys);
    edycjaLayout->addWidget(edycjaLabel);
    dataEdycji = new QLineEdit("-", this);
    dataEdycji->setReadOnly(true);
    dataEdycji->setMinimumSize(constSzerT, constWys);
    edycjaLayout->addWidget(dataEdycji);
    mainLay->addLayout(edycjaLayout);

    QHBoxLayout* czasLayout = new QHBoxLayout();                                    //Czas ładowania
    QLabel* czasLabel = new QLabel(tr("Czas ładowania:"), this);
    czasLabel->setMinimumSize(constSzer, constWys);
    czasLayout->addWidget(czasLabel);
    czasLadowania = new QLineEdit("-", this);
    czasLadowania->setReadOnly(true);
    czasLadowania->setMinimumSize(constSzerT, constWys);
    czasLayout->addWidget(czasLadowania);
    mainLay->addLayout(czasLayout);

    QHBoxLayout* butLayout = new QHBoxLayout();                                     //Przycisk "OK"
    butLayout->addSpacerItem(new QSpacerItem(5, 5, QSizePolicy::Expanding));
    QPushButton* but = new QPushButton("OK", this);
    connect(but, SIGNAL(clicked(bool)), this, SLOT(close()));
    butLayout->addWidget(but);
    mainLay->addLayout(butLayout);
}
