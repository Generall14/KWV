#include "kwgraphicsview.h"
#include "../mainwindow.h"
#include <QDebug>
#include <assert.h>

KWGraphicsView::KWGraphicsView(MainWindow *mw):
    QGraphicsView(mw),
    MWPtr(mw)
{
    scena = new QGraphicsScene(this);                                               //Tworzenie sceny
    this->setScene(scena);                                                          //Dodawanie sceny do widoku

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);                       //Wyłączenie pasków
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(GifUpd()));

    setMouseTracking(true);                                                         //Ustawianie śledzenia myszy

#ifdef TEST
    scena->setBackgroundBrush(QBrush(QColor(100, 200, 150)));                       //Ustawianie tła
#else
    scena->setBackgroundBrush(QBrush(QColor(0, 0, 0)));                             //Ustawianie tła
#endif
}

KWGraphicsView::~KWGraphicsView()
{
    delete timer;
}

void KWGraphicsView::Wyswietl(QVector<QPixmap> nowyObraz, int tv)
{
    czas_start = std::chrono::steady_clock::now();                                  //Pomiar czasu
    if(nowyObraz.isEmpty())                                                         //Jeżeli obraz jest nieprawidłowy
        return;

    timer->stop();                                                                  //Zatrzymanie timera
    gifVec.clear();                                                                 //Czyszczenie wektora
    currFrame=0;                                                                    //Ustawianie klatki
    forward = true;

    if((nowyObraz.length()==1)||(tv==0))                                            //Zwykły obraz
    {
        animacja = false;                                                           //Ustawianie flagi
        gifVec.push_back(nowyObraz.at(0));                                          //Kopiowanie obrazu
    }
    else                                                                            //Animacja
    {
        animacja = true;                                                            //Ustawianie flagi
        gifVec = nowyObraz;                                                         //Kopiowanie wektora
        totalFrame = gifVec.length();                                               //Ustalanie długości animacji
        defDelay = tv;                                                              //Ustawianie czasku klatki
        timer->start(defDelay);                                                     //Uruchamianie timera
    }

    scena->clear();                                                                 //Czyszczenie sceny
    scena->addPixmap(gifVec.at(0));                                                 //Dodawanie obrazu do sceny
    scena->setSceneRect(gifVec.at(0).rect());                                       //Ustawianie rozmiaru sceny
    zum = 1;                                                                        //Zerowanie powiększenia
    DopasujOkno(gifVec.at(0).rect());                                               //Dopasuj okno

    emit Zoomed(zum*100);                                                           //Wysyłanie sygnału
    emit Nowy();
    czas_end = std::chrono::steady_clock::now();                                    //Pomiar czasu
}

void KWGraphicsView::GifNext()
{
    ++currFrame;                                                                    //Inkrementacja klatki
    if(currFrame>=totalFrame)                                                       //Sprawdzanie zakresu klatki
        currFrame=0;

    if(zum!=1.00)
        Zoom(zum, gifVec.at(currFrame));                                            //Wyświetlanie przeskalowanego obrazu
    else
    {
        scena->clear();                                                             //Czyszczenie sceny
        scena->addPixmap(gifVec.at(currFrame));                                     //Dodawanie obrazu
        scena->setSceneRect(gifVec.at(currFrame).rect());
    }
}

void KWGraphicsView::GifBack()
{
    --currFrame;                                                                    //Dekrementacja
    if(currFrame<0)                                                                 //Sprawdzanie zakresu klatki
        currFrame = totalFrame-1;

    if(zum!=1.00)
        Zoom(zum, gifVec.at(currFrame));                                            //Wyświetlanie przeskalowanego obrazu
    else
    {
        scena->clear();                                                             //Czyszczenie sceny
        scena->addPixmap(gifVec.at(currFrame));                                     //Dodawanie obrazu
        scena->setSceneRect(gifVec.at(currFrame).rect());
    }
}

void KWGraphicsView::GifUpd()
{
    if(!trybKol)                                                                    //Tryb zwykły
        GifNext();
    else
    {
        if(forward)                                                                 //Odtwarzanie w przód
        {
            if(currFrame>=totalFrame-1)                                             //Zawrót
            {
                forward = false;
                GifBack();
            }
            else
                GifNext();
        }
        else                                                                        //Odtwarzanie w tył
        {
            if(currFrame<=0)                                                        //Zawrót
            {
                forward = true;
                GifNext();
            }
            else
                GifBack();
        }
    }
}

void KWGraphicsView::GifSetSpeed(int s)
{
    timer->stop();                                                                  //Zatrzymanie timera
    if(animacja)
        timer->start(s);                                                            //Uruchomienie z nowym interwałem
}

void KWGraphicsView::DopasujOkno(QRect trect)
{
    if((trect.height()>maxObraz.height())||(trect.width()>maxObraz.width()))        //Sprawdzanie czy obraz przekracza dopuszczalny rozmiar
        DopasujObrazDoOkna(trect);                                                  //Dopasowuje obraz do okna
    else
    {
        if((!MWw->isMaximized())&&(!MWw->isFullScreen()))                           //Tylko jeżeli okno jest w trybie normalnym
            DopasujOknoDoObrazu(trect);                                             //Dopasowuje okno do obrazu
    }
}

void KWGraphicsView::Odswiez()
{
    if(gifVec.isEmpty())
        return;
    DopasujOkno(gifVec.at(currFrame).rect());
}

void KWGraphicsView::DopasujOknoDoObrazu(QRect trect)
{
    if(MWw->isMaximized()||MWw->isFullScreen())                                     //Opuszczanie funkcji gdy okno jest zmaksymalizowane lub w trybie fullscreen
        return;

    trect.setHeight(trect.height()+paskiGora+paskiDol+marDOl);                      //Dodanie marginesów
    trect.setWidth(trect.width()+marSzer);

    if(trect.height()>maxOkno.height())                                             //Ograniczenie wysokości okna do maksymalnej wartości
        trect.setHeight(maxOkno.height());
    if(trect.width()>maxOkno.width())                                               //Ograniczenie szerokości okna do maksymalnej wrtości
        trect.setWidth(maxOkno.width());

    MWw->setGeometry(DodajMar(trect));                                              //Ustawienie rozmiaru okna
}

void KWGraphicsView::DopasujObrazDoOkna(QRect trect)
{
    QRect temp = maxOkno;
    QRect max;

    if(MWw->isMaximized())                                                          //Dobranie odpowiedniego odniesienia
        max = maxObrazMax;
    else if(MWw->isFullScreen())
        max = avFull;
    else
        max = maxObraz;

    float sx = ((float)trect.width())/((float)max.width());                         //Obliczanie krotności przekroczenia szerokości
    float sy = ((float)trect.height())/((float)max.height());                       //Obliczanie krotności przekroczenia wysokości

    if(sx>sy)                                                                       //Wybór większego współczynnika
    {
        Zoom(1/sx);                                                                 //Przeskalowanie do szerokości
        temp.setHeight(gifVec.at(currFrame).height()*zum+1);                        //Przeskalowanie rozmiaru do wysokości obrazu
    }
    else
    {
        Zoom(1/sy);                                                                 //Przeskalowanie do wysokości
        temp.setWidth(gifVec.at(currFrame).width()*zum+5);                          //Przeskalowanie rozmiaru do szerokości obrazu
    }

    if((!MWw->isMaximized())&&(!MWw->isFullScreen()))                               //Tylko jeżeli okno jest w trybie normalnym
        MWw->setGeometry(DodajMar(temp));                                           //Ustawienie rozmiaru okna
}

void KWGraphicsView::Zoom(float z, QPixmap px)
{
    if((z<minZoom)||(z>maxZoom))                                                    //Sprawdzania czy z przekracza wartości dopuszczalne
        return;

    if(px.isNull())
        px = gifVec.at(currFrame);                                                  //Jeżeli nie podano obrazu, użyj aktualnego

    QPixmap temp = px.scaledToHeight((int)(px.height()*z), Qt::SmoothTransformation);//Przeskalowanie obrazu

    scena->clear();                                                                 //Czyszczenie sceny
    scena->addPixmap(temp);                                                         //Dodawanie przeskalowanego obrazu
    scena->setSceneRect(temp.rect());                                               //Dopasowanie okna do nowego obrazu
    zum = z;                                                                        //Ustawienie powiększenia
    emit Zoomed(z*100);                                                             //Wysłanie sygnału o zmianie powiększenia
}

void KWGraphicsView::ResetZoom()
{
    SetZoom(1.0);
}

void KWGraphicsView::InitGeometry()
{
    paskiGora = MWw->menuBar()->height();                                           //Pobieranie wysokości paska menu
    paskiDol = MWw->statusBar()->height();                                          //Pobieranie wartości paska statusu

    QDesktopWidget desktop;
    QRect av = desktop.availableGeometry();                                         //Pobieranie dostępnego rozmiaru pulpitu
    QRect fgeT = MWw->frameGeometry();                                              //Pobieranie rozmiaru zewnętrznego okna
    QRect geT = MWw->geometry();                                                    //Pobieranie rozmiaru wewnętrznego okna
    avFull = desktop.screenGeometry();                                              //Pobieranie rozdzielczości ekranu

    marSzer = (fgeT.width()-geT.width())*.5;                                        //Obliczanie marginesów bocznych
    marGora = geT.y()-fgeT.y();                                                     //Obliczanie marginesu górnego
    marDOl = (fgeT.height()-geT.height())-marGora;                                  //Obliczanie marginesu dolnego

    maxOkno = av;
    maxOkno.setHeight(maxOkno.height()-marGora-marDOl);                             //Obliczanie maksymalnego rozmiaru okna
    maxOkno.setWidth(maxOkno.width()-2*marSzer);

    maxObraz = maxOkno;                                                             //Obliczanie maksymalnego rozmiaru obrazu
    maxObraz.setWidth(maxObraz.width()-marSzer);
    maxObraz.setHeight(maxObraz.height()-paskiDol-paskiGora-marDOl);

    maxObrazMax = av;                                                               //Obliczanie rozmiaru obrazu dla zmaksymalizowanego okna
    maxObrazMax.setHeight(maxObrazMax.height()-paskiDol-paskiGora-marGora);

    disconnect(qApp, SIGNAL(applicationStateChanged(Qt::ApplicationState)), this, SLOT(InitGeometry()));//Odłączanie sygnału

    if(!MWw->startOpen.isNull())
        MWw->Otworz(MWw->startOpen);
}

QRect KWGraphicsView::DodajMar(QRect rwe)
{
    rwe.setX(rwe.x()+marSzer);                                                      //Dodanie marginesu szerokości do punktu początkowego
    rwe.setY(rwe.y()+marGora);                                                      //Dodanie marginesu wysokości do punktu początkowego
    rwe.setWidth(rwe.width()+marSzer);                                              //Dodanie marginesu do szerokości obrazu
    rwe.setHeight(rwe.height()+marGora);                                            //Dodanie marginesu do wysokości obrazu

    return rwe;
}

void KWGraphicsView::ZoomIn()
{
    SetZoom(floor(zum*(1/deltaZoom))*deltaZoom+deltaZoom);                          //Dopasowanie wartości do szerego 1 + x*0,1 i powiększenie o jeden stopień
}

void KWGraphicsView::ZoomOut()
{
    SetZoom(ceil(zum*(1/deltaZoom)-deltaZoom)*deltaZoom-deltaZoom);                 //Dopasowanie wartości do szerego 1 + x*0,1 i zmniejszenie o jeden stopień
}

void KWGraphicsView::SetZoom(float z)
{
    if(gifVec.isEmpty())
        return;

    Zoom(z);                                                                        //Przeskalowanie obrazu

    QRect temp = scena->sceneRect().toRect();                                       //Pobranie rozmiaru sceny
    DopasujOknoDoObrazu(temp);                                                      //Dopasowanie okna do obrazu
}

int KWGraphicsView::DefDelay()
{
    return defDelay;
}

bool KWGraphicsView::GifTryb()
{
    return trybKol;
}

void KWGraphicsView::GifSetTryb(bool b)
{
    trybKol = b;
}

bool KWGraphicsView::Animacja()
{
    return animacja;
}

void KWGraphicsView::GifPlayPause()
{
    if(animacja)                                                                    //Jeżeli obrazem jest animacja
    {
        if(timer->isActive())
            timer->stop();
        else
            timer->start();
    }
}

long KWGraphicsView::getTimeUs()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(czas_end - czas_start).count();
}


void KWGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton)                                            //Prawy przycisk myszy
    {
        przeciaganie = true;                                                        //Ustawienie flagi przeciągania
        setCursor(QCursor(Qt::ClosedHandCursor));                                   //Ustawianie kursora
    }
    else
        QGraphicsView::mousePressEvent(event);                                      //Wywoływanie standardowej obsługi zdarzeń

    pozycja = event->globalPos();                                                   //Zapisanie pozycji kursora
}

void KWGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    przeciaganie = false;                                                           //Resetowanie flagi przeciągania

    setCursor(QCursor(Qt::ArrowCursor));                                            //Resetowanie kursora

    QGraphicsView::mouseReleaseEvent(event);                                        //Wywoływanie standardowej obsługi zdarzeń
}

void KWGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if(MWw->isFullScreen())                                                         //Wykrywanie trybu fullscreen
    {
        MWw->cursorTimer->start();                                                  //Restart timera
        QApplication::restoreOverrideCursor();                                      //Przywracanie kursora
    }

    if(przeciaganie)                                                                //Jeżeli ustawiony tryb przeciągania
    {
        QPoint temp = event->globalPos();                                           //Pobranie pozycji kursora
        int dx = temp.x() - pozycja.x();                                            //Obliczanie przesunięć
        int dy = temp.y() - pozycja.y();
        verticalScrollBar()->setValue(verticalScrollBar()->value()-dy);             //Przesunięcie obrazu
        horizontalScrollBar()->setValue(horizontalScrollBar()->value()-dx);
        pozycja = temp;                                                             //Nadpisanie pozycji
    }
    else
        QGraphicsView::mouseMoveEvent(event);                                       //Wywoływanie standardowej obsługi zdarzeń
}

void KWGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)                                             //Lewy przycisk myszy
        emit ToggleFullscreen();                                                    //Żądanie przełączenia trybu fullscreen
}

void KWGraphicsView::keyPressEvent(QKeyEvent *event)
{
    if(gifVec.isEmpty())
    {
        QGraphicsView::keyPressEvent(event);                                        //Wywołanie standardowej obsługi klawiszy
        return;
    }

    QRect trect = gifVec.at(currFrame).rect();
    QRect max;

    if(MWw->isMaximized())                                                          //Dobranie odpowiedniego odniesienia
        max = maxObrazMax;
    else if(MWw->isFullScreen())
        max = avFull;
    else
        max = maxObraz;

    bool przeskalowaneW = false, przeskalowaneH = false;                            //Sprawdzanie czy obraz jest powiększony ponad okno
    if(trect.width()*zum>(max.width()))
        przeskalowaneW = true;
    if(trect.height()*zum>(max.height()))
        przeskalowaneH = true;

    if(event->key()==Qt::Key_Right)                                                 //Strzałka w prawo
    {
        if(!przeskalowaneW)                                                         //Obraz przeskalowany -> przesuwanie obrazu
            emit Next();
        else                                                                        //W przeciwnym wypadku -> zmiana obrazu
            QGraphicsView::keyPressEvent(event);
    }
    else if(event->key()==Qt::Key_Left)                                             //Strzałka w lewo
    {
        if(!przeskalowaneW)
            emit Back();
        else
            QGraphicsView::keyPressEvent(event);
    }
    else if(event->key()==Qt::Key_Up)                                               //Strzałka w górę
    {
        if(!przeskalowaneH)                                                         //Obraz przeskalowany -> przesuwanie obrazu
            emit Next();
        else                                                                        //W przeciwnym wypadku -> zmiana obrazu
            QGraphicsView::keyPressEvent(event);
    }
    else if(event->key()==Qt::Key_Down)                                             //Strzałka w dół
    {
        if(!przeskalowaneH)
            emit Back();
        else
            QGraphicsView::keyPressEvent(event);
    }
    else
        QGraphicsView::keyPressEvent(event);                                        //Wywołanie standardowej obsługi klawiszy
}

void KWGraphicsView::wheelEvent(QWheelEvent* event)
{
    if((event->modifiers()==Qt::ControlModifier)||(event->buttons()==Qt::MidButton))//Sprawdzanie czy wciśnięto CTRL lub ŚPM
    {
        if(event->angleDelta().y()>0)                                               //Skalowanie obrazu
            this->ZoomIn();
        else
            this->ZoomOut();
    }
    else
    {
        if(event->angleDelta().y()>0)                                               //Przełączanie obrazów
            emit Next();
        else
            emit Back();
    }
}
