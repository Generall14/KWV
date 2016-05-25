#include "mainwindow.h"

#include <QDebug>

#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 100, 100);                                              //Ustawianie w początkowej pozycji (górna lewa strona ekranu)

    setMinimumSize(QSize(minSzerokosc, minWysokosc));                               //Ustawianie minimalnego rozmiaru okna

    QStringList tempList = qApp->arguments();                                       //Pobieranie listy argumentów
    if(tempList.length()>1)                                                         //Jeżeli podano dodatkowe argumenty
        startOpen = tempList[1];                                                    //Przypisywanie drugiego argumentu jako adresu do otwarcia

    cursorTimer = new QTimer();                                                     //Tworzenie nowego timera
    cursorTimer->setInterval(2000);                                                 //Ustawianie czasu ukrywania
    connect(cursorTimer, SIGNAL(timeout()), this, SLOT(HideCursor()));              //Tworzenie połączenia

    InitWidgets();                                                                  //Tworzenie obiektów
    InitConnections();                                                              //Tworzenie połączeń
    InitShortcuts();                                                                //Tworzenie skrótów

    setWindowIcon(QIcon("ics/ikona.ico"));                                          //Ustawianie ikony na belce okna

#ifdef TEST                                                                         //Wersja testowa
    this->setWindowTitle("KWView (Wersja testowa)");
#else
    this->setWindowTitle("KWView");                                                 //Główny pasek okna
#endif

#ifdef TEST                                                                         //testy

    QPushButton* pba = new QPushButton("A", this);
    pba->setGeometry(10, 30, 30, 30);
    connect(pba, SIGNAL(clicked(bool)), this, SLOT(testOpenA()));

    QPushButton* pbback = new QPushButton("<", this);
    pbback->setGeometry(40, 30, 30, 30);
    connect(pbback, SIGNAL(clicked(bool)), motor, SLOT(Back()));

    QPushButton* pbnext = new QPushButton(">", this);
    pbnext->setGeometry(70, 30, 30, 30);
    connect(pbnext, SIGNAL(clicked(bool)), motor, SLOT(Next()));

    QPushButton* pbminus = new QPushButton("-", this);
    pbminus->setGeometry(100, 30, 30, 30);
    connect(pbminus, SIGNAL(clicked(bool)), wyswietlacz, SLOT(ZoomOut()));

    QPushButton* pbplus = new QPushButton("+", this);
    pbplus->setGeometry(130, 30, 30, 30);
    connect(pbplus, SIGNAL(clicked(bool)), wyswietlacz, SLOT(ZoomIn()));

    QPushButton* pbtest = new QPushButton("t", this);
    pbtest->setGeometry(160, 30, 30, 30);
    connect(pbtest, SIGNAL(clicked(bool)), this, SLOT(publicTest()));
#endif                                                                              //koniec testów
}

MainWindow::~MainWindow()
{
    for(int i=0;i<rep.length();++i)
        delete rep[i];
    rep.clear();

    delete cursorTimer;
}

void MainWindow::InitWidgets()
{
    wyswietlacz = new KWGraphicsView(this);                                         //Tworzenie wyświetlacza obrazów
    this->setCentralWidget(wyswietlacz);

    menu = new KWMenu(this);                                                        //Tworzenie menu

    motor = new KWMotor(wyswietlacz, this);                                         //Tworzenie silnika

    pasekDolny = new KWStatusBar(QMainWindow::statusBar(), this);                   //Tworzenie paska stanu

    invisibleCloser = new QPushButton("", this);                                    //Tworzenie przycisku do zamykania trybu fullscreen
    QBrush tb(Qt::transparent);                                                     //Czerń
    invisibleCloser->setPalette(QPalette(tb, tb, tb, tb, tb, tb, tb, tb, tb));
    invisibleCloser->setFlat(true);                                                 //Niewidzialność
    invisibleCloser->setCursor(Qt::PointingHandCursor);
    invisibleCloser->hide();
}

void MainWindow::InitConnections()
{
    connect(qApp, SIGNAL(applicationStateChanged(Qt::ApplicationState)), wyswietlacz, SLOT(InitGeometry()));    //Tylko do inicjalizacji

    connect(invisibleCloser, SIGNAL(clicked(bool)), this, SLOT(resetFullscreen()));                             //Zamykanie trybu pełnoekranowego

    connect(wyswietlacz, SIGNAL(Zoomed(int)), pasekDolny, SLOT(UstawZoom(int)));                                //Sygnały z wyświetlacza
    connect(wyswietlacz, SIGNAL(Next()), motor, SLOT(Next()));
    connect(wyswietlacz, SIGNAL(Back()), motor, SLOT(Back()));
    connect(wyswietlacz, SIGNAL(ToggleFullscreen()), this, SLOT(ToggleFullscreen()));

    connect(motor, SIGNAL(Rozdzielczosc(int,int,int)), pasekDolny, SLOT(UstawRozdzielczosc(int,int,int)));      //Sygnały po otworzeniu nowego pliku
    connect(motor, SIGNAL(Rozmiar(int)), pasekDolny, SLOT(UstawRozmiar(int)));
    connect(motor, SIGNAL(Licznik(int,int)), pasekDolny, SLOT(UstawLicznik(int,int)));
    connect(motor, SIGNAL(Data(QDateTime)), pasekDolny, SLOT(UstawDate(QDateTime)));
    connect(motor, SIGNAL(Plik(QString)), this, SLOT(TitleBar(QString)));
    connect(motor, SIGNAL(Plik(QString)), pasekDolny, SLOT(UstawPlik(QString)));
    connect(motor, SIGNAL(Error(QString)), this, SLOT(Error(QString)));

    connect(menu, SIGNAL(About()), this, SLOT(About()));                                                        //Połączenia z menu
    connect(menu, SIGNAL(Otworz()), motor, SLOT(Otworz()));

    connect(pasekDolny, SIGNAL(Zmiana(int)), motor, SLOT(Otworz(int)));                                         //Sygnały z paska stanu
    connect(pasekDolny, SIGNAL(ResetZoom()), wyswietlacz, SLOT(ResetZoom()));
    connect(pasekDolny, SIGNAL(Back()), motor, SLOT(Back()));
    connect(pasekDolny, SIGNAL(Next()), motor, SLOT(Next()));
}

void MainWindow::InitShortcuts()
{
    QShortcut* s =  new QShortcut(QKeySequence(Qt::Key_Plus), this);                //Powiększenie obrazu
    connect(s, SIGNAL(activated()), wyswietlacz, SLOT(ZoomIn()));

    s =  new QShortcut(QKeySequence(Qt::Key_Minus), this);                          //Pomniejszenie obrazu
    connect(s, SIGNAL(activated()), wyswietlacz, SLOT(ZoomOut()));

    s =  new QShortcut(QKeySequence(Qt::Key_Delete), this);                         //Usuń
    connect(s, SIGNAL(activated()), this, SLOT(Usun()));

    s =  new QShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_N), this);        //Nowe okno
    connect(s, SIGNAL(activated()), this, SLOT(NoweOkno()));
    s->setEnabled(false);
    rep.push_back(s);

    s =  new QShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_O), this);        //Otwieranie nowego obrazu
    connect(s, SIGNAL(activated()), motor, SLOT(Otworz()));
    s->setEnabled(false);
    rep.push_back(s);

    s =  new QShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_H), this);        //Zoom reset
    connect(s, SIGNAL(activated()), wyswietlacz, SLOT(ResetZoom()));
    s->setEnabled(false);
    rep.push_back(s);
}

void MainWindow::testOpenA()
{
    motor->Otworz("imga.jpg");
}

void MainWindow::TitleBar(QString name)
{
#ifdef TEST                                                                         //Wersja testowa
    this->setWindowTitle("KWView (Wersja testowa) - " + name);
#else
    this->setWindowTitle("KWView - " + name);                                       //Główny pasek okna
#endif
}

void MainWindow::About()
{
    QString t;
    about ab;
    t = "KWView\n\nAutor: " + ab.autor + "\n\nWersja programu: " + ab.wersja + "\nZ dnia ";
    t += ab.data + "\n\nWersja ";
#ifdef TEST
    t += "testowa.";
#else
    t += "docelowa.";
#endif

    t += "\n\nStan programu: " + ab.stan;
    QMessageBox msgBox;
    msgBox.setWindowTitle("O programie");
    msgBox.setText(t);
    msgBox.setStandardButtons(QMessageBox::Ok );
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void MainWindow::publicTest()
{
//    std::chrono::steady_clock::time_point czas_start, czas_end;
//    //ToggleFullscreen();
//    //GifManager();
//    czas_start = std::chrono::steady_clock::now();                                  //Pomiar czasu
//    motor->Next();
//    czas_end = std::chrono::steady_clock::now();                                  //Pomiar czasu
//    long open = motor->getTimeUs();
//    long show = wyswietlacz->getTimeUs();
//    long total = std::chrono::duration_cast<std::chrono::microseconds>(czas_end - czas_start).count();
//    long narzut = total - show - open;
//    qDebug() << "total: " << total << ", open: " << open << ", show: " << show << ", narzut: " << narzut;
    Benchmark();
}

void MainWindow::Benchmark()
{
    long long total=0, open=0, show=0, zoom=0;
    std::chrono::steady_clock::time_point czas_start, czas_end;
    motor->Otworz("bench/imga.jpg");

    for(int i=0;i<43;i++)
    {
        czas_start = std::chrono::steady_clock::now();
        motor->Next();
        czas_end = std::chrono::steady_clock::now();
        total += std::chrono::duration_cast<std::chrono::microseconds>(czas_end - czas_start).count();
        open += motor->getTimeUs();
        show += wyswietlacz->getTimeUs();

        wyswietlacz->SetZoom(1.95);
        zoom += std::chrono::duration_cast<std::chrono::microseconds>(czas_end - czas_start).count();
        total += std::chrono::duration_cast<std::chrono::microseconds>(czas_end - czas_start).count();
    }
    printf("Wyniki pomiaru:\nCałkowity czas: %d ms\n", (int)(total/1000));
    printf("Czas otwierania:: %d ms\n", (int)(open/1000));
    printf("Czas wyświetlania:: %d ms\n", (int)(show/1000));
    printf("Czas skalowania:: %d ms\n", (int)(zoom/1000));
}

void MainWindow::ToggleFullscreen()
{
    if(!isFullScreen())
        setFullsscreen();
    else
        resetFullscreen();
}

void MainWindow::setFullsscreen()
{
    if(isMaximized())                                                               //Zapisuje poprzedni stan okna
        prev = Zmaksymalizowany;
    else
        prev = Normalny;

    QMainWindow::statusBar()->hide();                                               //Ukrywa statusbar
    menuBar()->hide();                                                              //Ukrywa pasek menu

    showFullScreen();                                                               //Uruchamia tryb fullscreen

    QDesktopWidget desktop;
    invisibleCloser->show();                                                        //Uruchamia przycisk wyjście z trybu
    invisibleCloser->setGeometry(desktop.screenGeometry().width()-30, 0, 30, 30);

    pasekDolny->fullscreenOn();                                                     //Pasek dolny przechodzi w tryb fullscreen (wyświetla sam napis w górnym, lewym rogu ekranu)

    for(int i=0;i<rep.length();++i)                                                 //Aktywacja skrótów
        rep[i]->setEnabled(true);

    cursorTimer->start();                                                           //Uruchamianie timera

    //wyswietlacz->ResetZoom();
}

void MainWindow::resetFullscreen()
{
    QMainWindow::statusBar()->show();                                               //Wyświetla statusbar
    menuBar()->show();                                                              //Wyświetla menu

    if(prev==Zmaksymalizowany)                                                      //Przywraca poprzedni stan okna
        showMaximized();
    else
        showNormal();

    invisibleCloser->setGeometry(0, 20, 30, 30);                                    //Ukrywa przycisk zamykania
    invisibleCloser->hide();

    pasekDolny->fullscreenOff();                                                    //Pasek dolny przechodzi w standardowy typ

    for(int i=0;i<rep.length();++i)                                                 //Deaktywacja skrótów
        rep[i]->setEnabled(false);

    cursorTimer->stop();                                                            //Zatrzymywanie timera
    QApplication::restoreOverrideCursor();                                          //Przywracanie kursora

    //wyswietlacz->ResetZoom();
}

void MainWindow::HideCursor()
{
    cursorTimer->stop();                                                            //Zatrzymywanie timera
    QApplication::setOverrideCursor(Qt::BlankCursor);                               //Ukrywanie kursora
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Escape)                                                //Przycisk ESC
    {
        if(isFullScreen())                                                          //Jeżeli tryb fullscreen jest aktywny, przejdź do zwykłego
            resetFullscreen();
        else                                                                        //W przeciwnym razie wyłącz program
            this->close();
    }
    else if((event->key()==Qt::Key_Enter)||(event->key()==Qt::Key_Return))          //Przycisk Enter
        this->ToggleFullscreen();                                                   //Przełączenie trybu
    else
        QMainWindow::keyPressEvent(event);                                          //Przekazanie nieprzechwyconych eventów
}

bool MainWindow::event(QEvent* event)
{
    if(event->type()==QEvent::WindowStateChange)                                    //Zmiana stanu okna
        wyswietlacz->Odswiez();
    else
        QMainWindow::event(event);                                                  //Przekazanie nieprzechwyconych eventów
    return 0;
}

void MainWindow::Otworz(QString adres)
{
    motor->Otworz(adres);
}

void MainWindow::Error(QString er)
{
    QMessageBox msgBox(QMessageBox::Warning, tr("Błąd"), er, QMessageBox::Ok, this);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

void MainWindow::GifManager()
{
    if(!okno)
        okno = new KW3rdGif(wyswietlacz);
    okno->show();
}

void MainWindow::Usun()
{
    if(!motor->isOpened())                                                          //Jeżeli żaden obraz nie jest wczytany
        return;

    QMessageBox msgBox(QMessageBox::Question, tr("Potwierdź"), tr("Czy jesteś pewien, że chcesz usunąć obraz ") + motor->Adres() + "?",
                       QMessageBox::Yes | QMessageBox::No, this);
    msgBox.setButtonText(QMessageBox::Yes, tr("Tak"));
    msgBox.setButtonText(QMessageBox::No, tr("Anuluj"));
    msgBox.setIcon(QMessageBox::Question);

    if(msgBox.exec() == QMessageBox::No)                                            //Anuluj -> wyjście
        return;

    QFile plik(motor->Adres());                                                     //Usuwanie pliku
    plik.remove();

    if(motor->DlugoscListy()>1)                                                     //Jeżeli na liście były inne pliki
    {
        motor->Next();                                                              //Otwórz kolejny obraz
        motor->Otworz(motor->Adres());
    }
}

void MainWindow::NoweOkno()
{
    QStringList arguments;
    if(motor->isOpened())
        arguments << motor->Adres();                                                //Tworzenie argumentów programu

    QProcess *myProcess = new QProcess(NULL);
    myProcess->start(qApp->arguments()[0], arguments);                              //Uruchamianie nowego procesu
}
