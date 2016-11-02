#include "kwstatusbar.h"
#include "mainwindow.h"

KWStatusBar::KWStatusBar(QStatusBar *bar, MainWindow *mw):
    QObject(bar),
    MWPtr(mw),
    pasek(bar)
{
    Init();
}

void KWStatusBar::Init()
{
    res = new QLabel("-");                                                          //Nowy obiekt do wyświetlania rozdzielczości
    res->setMinimumWidth(130);                                                      //Minimalna szerokość
    res->setFrameShape(QFrame::StyledPanel);                                        //Styl ramki

    zoom = new QLabel("-");                                                         //Nowy obiekt do wyświetlania powiększenia
    zoom->setMinimumWidth(60);
    zoom->setFrameShape(QFrame::StyledPanel);

    resetZoom = new QToolButton();                                                  //Nowy przycisk resetu powiększenia
    int ms = pasek->height()*.6;
    resetZoom->setMinimumSize(ms, ms);
    resetZoom->setMaximumSize(ms, ms);
    resetZoom->setToolTip(tr("Oryginalny rozmiar"));
    resetZoom->setIcon(QIcon(":/ics/resetZoom.ico"));
    resetZoom->setIconSize(QSize(12, 12));
    connect(resetZoom, SIGNAL(clicked(bool)), this, SIGNAL(ResetZoom()));

    btnBack = new QToolButton();                                                    //Nowy przycisk "Back"
    btnBack->setMinimumSize(ms, ms);
    btnBack->setMaximumSize(ms, ms);
    btnBack->setToolTip(tr("Poprzedni obraz"));
    btnBack->setIcon(QIcon(":/ics/prevImage.ico"));
    btnBack->setIconSize(QSize(12, 12));
    connect(btnBack, SIGNAL(clicked(bool)), this, SIGNAL(Back()));

    plikia = new MyQLineEdit("0");                                                  //Nowy obiekt do wyświetlania aktualnego pliku
    plikia->setMinimumWidth(40);
    plikia->setMaximumWidth(40);
    plikia->setAlignment(Qt::AlignRight);
    connect(plikia, SIGNAL(editingFinished()), this, SLOT(ZmienPlik()));
    //pliki->setFrameShape(QFrame::StyledPanel);

    plikib = new QLabel("/0");                                                      //Nowy obiekt do wyświetlania liczby plików
    plikib->setMinimumWidth(50);
    plikib->setFrameShape(QFrame::StyledPanel);

    btnNext = new QToolButton();                                                    //Nowy przycisk "Next"
    btnNext->setMinimumSize(ms, ms);
    btnNext->setMaximumSize(ms, ms);
    btnNext->setToolTip(tr("Kolejny obraz"));
    btnNext->setIcon(QIcon(":/ics/nextImage.ico"));
    btnNext->setIconSize(QSize(12, 12));
    connect(btnNext, SIGNAL(clicked(bool)), this, SIGNAL(Next()));

    data = new QLabel("-");                                                         //Nowy obiekt do wyświetlania daty
    data->setMinimumWidth(100);
    data->setFrameShape(QFrame::StyledPanel);

    rozmiar = new QLabel("-");                                                      //Nowy obiekt do wyświetlania rozmiaru
    rozmiar->setMinimumWidth(80);
    rozmiar->setFrameShape(QFrame::StyledPanel);

    pasek->addWidget(res);                                                          //Dodawanie obiektów do paska stanu
    pasek->addWidget(zoom);
    pasek->addWidget(resetZoom);
    pasek->addWidget(btnBack);
    pasek->addWidget(plikia);
    pasek->addWidget(plikib);
    pasek->addWidget(btnNext);
    pasek->addWidget(data);
    pasek->addWidget(rozmiar);

    plikiF = new QLabel("0/0", MWw);                                                //Pliki w trybie pełnoekranowym
    plikiF->setGeometry(5, 5, 90, 16);
    plikiF->setMinimumWidth(90);
    plikiF->hide();

    zoomF = new QLabel("-", MWw);                                                   //Zoom w trybie pełnoekranowym
    zoomF->setGeometry(100, 5, 60, 16);
    zoomF->setMinimumWidth(60);
    zoomF->hide();

    plikF = new QLabel("-", MWw);                                                   //Nazwa pliku w trybie pełnoekranowym
    plikF->setGeometry(165, 5, 400, 16);
    plikF->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    plikF->hide();
}

void KWStatusBar::SetNoPicData()
{
    res->setText("-");
    zoom->setText("-");
    plikia->setText("0");
    plikib->setText("/0");
    data->setText("-");
    rozmiar->setText("-");
    plikiF->setText("0/0");
    zoomF->setText("-");
    plikF->setText("-");
}

void KWStatusBar::fullscreenOn()
{
    plikF->show();                                                                  //Wyświetlanie elementów
    plikiF->show();
    zoomF->show();
}

void KWStatusBar::fullscreenOff()
{
    plikiF->hide();                                                                 //Ukrywanie elementów
    plikF->hide();
    zoomF->hide();
}

void KWStatusBar::UstawZoom(int z)
{
    zoom->setText(QString::number(z)+"%");
    zoomF->setText(QString::number(z)+"%");
}

void KWStatusBar::ZmienPlik()
{
    bool ok = false;
    int temp = plikia->text().toInt(&ok);

    if(ok)
    {
        if((temp>0)&&(temp<all)&&(temp!=akt))
        {
            emit Zmiana(temp-1);
            return;
        }
    }

    plikia->setText(QString::number(akt));
}

void KWStatusBar::SetNewData(const KWPicInfo *pi, int curr, int total)
{
    if(!pi->fileInfo.isFile())
    {
        SetNoPicData();
        return;
    }
    //Rozdzielczość
    QString temp = QString::number(pi->resWidth)+"x"+QString::number(pi->resHeight);//Podstawowy ciąg
    if(pi->isGif)
        temp += " (" + QString::number(pi->frames) + "kl)";                         //Dodawanie informacji o liczbie klatek
    else if(pi->picDepth)
        temp += " (" + QString::number(pi->picDepth) + "BPP)";                      //Dodawanie informacji o głębi
    res->setText(temp);
    //Licznik obrazów
    akt = curr;
    all = total;
    plikia->setText(QString::number(curr));
    plikib->setText("/"+QString::number(total));
    plikiF->setText(QString::number(curr)+"/"+QString::number(total));
    //Data
    data->setText(pi->fileInfo.lastModified().toString("yyyy-MM-dd, H:mm:ss"));
    //Rozmiar
    int KiB = pi->fileInfo.size()/1024;
    if(int(KiB/1024))                                                               //Sprawdza czy wyświetlić wartość w KiB czy konwertować na MiB
        rozmiar->setText(QString::number(float((float)KiB/1024.0), 'g', 2)+" MiB");
    else
        rozmiar->setText(QString::number(KiB)+" KiB");
    //Nazwa pliku
    plikF->setText(pi->fileInfo.fileName());
}
