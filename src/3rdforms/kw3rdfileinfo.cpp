#include "kw3rdfileinfo.h"

#include <QLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QLabel>
#include <QDir>
#include <algorithm>

KW3rdFileInfo::KW3rdFileInfo(QWidget *parent):
    QDialog(parent)
{
    InitWidgets();
    this->setWindowTitle(tr("Informacje o obrazie"));
}

void KW3rdFileInfo::GenInfo(QFileInfo fi, QPixmap ob, int kl)
{
    if(kl>0)
    {
        nazwaPliku->setText(fi.fileName());
        katalog->setText(fi.dir().absolutePath());
        sciezkaPliku->setText(fi.dir().absoluteFilePath(fi.fileName()));

        int w = ob.width();
        int h = ob.height();
        int gcd = std::__gcd(w, h);
        rozmiarObrazu->setText(QString::number(w)+" x "+QString::number(h)+tr(" pikseli")+" ("+QString::number(w/gcd)+":"+QString::number(h/gcd)+")");

        nazwaPliku->setText(QString::number(ob.defaultDepth()));
    }
    else
    {
        nazwaPliku->setText("-");
        katalog->setText("-");
        sciezkaPliku->setText("-");
        rozmiarObrazu->setText("-");
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

    QHBoxLayout* butLayout = new QHBoxLayout();                                     //Przycisk "OK"
    butLayout->addSpacerItem(new QSpacerItem(5, 5, QSizePolicy::Expanding));
    QPushButton* but = new QPushButton("OK", this);
    connect(but, SIGNAL(clicked(bool)), this, SLOT(close()));
    butLayout->addWidget(but);
    mainLay->addLayout(butLayout);
}
