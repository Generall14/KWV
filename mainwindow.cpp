#include "mainwindow.h"

#include <QDebug>

#include <QProcess>
#include <QTextStream>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 100, 100);                                              //Ustawianie w początkowej pozycji (górna lewa strona ekranu)

    setMinimumSize(QSize(minSzerokosc, minWysokosc));                               //Ustawianie minimalnego rozmiaru okna

    QStringList lista =  qApp->arguments();                                         //Pobieranie listy argumentów
    for(int i=0;i<lista.length();++i)                                               //Przeszukiwanie argumentów
    {
        if((!lista.at(i).compare("-t", Qt::CaseInsensitive))||(!lista.at(i).compare("--test", Qt::CaseInsensitive)))
        {
            testRun = true;
            lista.removeAt(i);
        }
    }
    if(lista.length()>1)                                                            //Jeżeli zostały dodatkowe argumenty
        startOpen = lista[1];                                                       //Przypisywanie drugiego argumentu jako adresu do otwarcia

    QFileInfo fff(qApp->arguments().at(0));                                         //Pobieranie katalogu bazowego
    baseDirectory = fff.path()+"/";

    cursorTimer = new QTimer();                                                     //Tworzenie nowego timera
    cursorTimer->setInterval(2000);                                                 //Ustawianie czasu ukrywania
    connect(cursorTimer, SIGNAL(timeout()), this, SLOT(HideCursor()));              //Tworzenie połączenia

    InitWidgets();                                                                  //Tworzenie obiektów
    InitConnections();                                                              //Tworzenie połączeń
    InitShortcuts();                                                                //Tworzenie skrótów

    setWindowIcon(QIcon(":/ics/ikona.ico"));                                        //Ustawianie ikony na belce okna
    LoadRecentFIles();                                                              //Czytanie listy ostatnich plików
    LoadRecentPlaces();
    AddToRec(startOpen);

    if(testRun)                                                                     //Wersja testowa
    {
        this->setWindowTitle("KWView <Tryb testowy>");

        QPushButton* pba = new QPushButton("A", this);
        pba->setGeometry(10, 30, 30, 30);
        connect(pba, SIGNAL(clicked(bool)), this, SLOT(testOpenA()));

        QPushButton* pbback = new QPushButton("ta", this);
        pbback->setGeometry(40, 30, 30, 30);
        connect(pbback, SIGNAL(clicked(bool)), this, SLOT(FileInfo()));

        QPushButton* pbnext = new QPushButton(">", this);
        pbnext->setGeometry(70, 30, 30, 30);
        connect(pbnext, SIGNAL(clicked(bool)), motor, SLOT(Next()));

        QPushButton* pbminus = new QPushButton("-", this);
        pbminus->setGeometry(100, 30, 30, 30);
        connect(pbminus, SIGNAL(clicked(bool)), zoomerObj, SLOT(ZoomOut()));

        QPushButton* pbplus = new QPushButton("+", this);
        pbplus->setGeometry(130, 30, 30, 30);
        connect(pbplus, SIGNAL(clicked(bool)), zoomerObj, SLOT(ZoomIn()));

        QPushButton* pbtest = new QPushButton("t", this);
        pbtest->setGeometry(160, 30, 30, 30);
        connect(pbtest, SIGNAL(clicked(bool)), this, SLOT(publicTest()));
    }
    else
        this->setWindowTitle("KWView");                                             //Główny pasek okna
}

MainWindow::~MainWindow()
{
    SaveRecentFIles();
    SaveRecentPlaces();

    for(int i=0;i<rep.length();++i)
        delete rep[i];
    rep.clear();

    delete okno;
    delete fileInfo;

    delete cursorTimer;
}

void MainWindow::InitWidgets()
{
    wyswietlacz = new KWGraphicsView(this);                                         //Tworzenie wyświetlacza obrazów
    this->setCentralWidget(wyswietlacz);

    picObj = new KWPic(this);
    zoomerObj = new KWZoomer(wyswietlacz->Scena(), this);
    playerObj = new KWPlayer(picObj, zoomerObj, this);

    motor = new KWMotor(picObj, this);                                         //Tworzenie silnika

    menu = new KWMenu(this);                                                        //Tworzenie menu

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

    //connect(wyswietlacz, SIGNAL(Zoomed(int)), pasekDolny, SLOT(UstawZoom(int)));                                //Sygnały z wyświetlacza
    connect(wyswietlacz, SIGNAL(Next()), motor, SLOT(Next()));
    connect(wyswietlacz, SIGNAL(Back()), motor, SLOT(Back()));
    connect(wyswietlacz, SIGNAL(ToggleFullscreen()), this, SLOT(ToggleFullscreen()));


    //connect(motor, SIGNAL(Rozdzielczosc(int,int,int,int)), pasekDolny, SLOT(UstawRozdzielczosc(int,int,int,int)));  //Sygnały po otworzeniu nowego pliku
    //connect(motor, SIGNAL(Rozmiar(int)), pasekDolny, SLOT(UstawRozmiar(int)));
    //connect(motor, SIGNAL(Licznik(int,int)), pasekDolny, SLOT(UstawLicznik(int,int)));
    //connect(motor, SIGNAL(Data(QDateTime)), pasekDolny, SLOT(UstawDate(QDateTime)));
//    connect(motor, SIGNAL(Plik(QString)), this, SLOT(TitleBar(QString)));
    //connect(motor, SIGNAL(Plik(QString)), pasekDolny, SLOT(UstawPlik(QString)));
    connect(motor, SIGNAL(Error(QString)), this, SLOT(Error(QString)));
    //connect(motor, SIGNAL(NewOpened(QString)), this, SLOT(AddToRec(QString)));
    connect(motor, SIGNAL(FileOn()), menu, SLOT(FileOn()));
    connect(motor, SIGNAL(FileOff()), menu, SLOT(FileOff()));
    connect(motor, SIGNAL(NewLoaded(const KWPicInfo*,int,int)), pasekDolny, SLOT(SetNewData(const KWPicInfo*,int,int)));

    connect(menu, SIGNAL(About()), this, SLOT(About()));                                                        //Połączenia z menu
    connect(menu, SIGNAL(Otworz()), this, SLOT(Otworz()));
    connect(menu, SIGNAL(OpenRec(int)), this, SLOT(OpenRec(int)));
    connect(menu, SIGNAL(CloseFile()), motor, SLOT(Clear()));

    connect(pasekDolny, SIGNAL(Zmiana(int)), motor, SLOT(Otworz(int)));                                         //Sygnały z paska stanu
    connect(pasekDolny, SIGNAL(ResetZoom()), zoomerObj, SLOT(ResetZoom()));
    connect(pasekDolny, SIGNAL(Back()), motor, SLOT(Back()));
    connect(pasekDolny, SIGNAL(Next()), motor, SLOT(Next()));

    connect(motor, SIGNAL(NewLoaded(const KWPicInfo*,int,int)), playerObj, SLOT(NewPic(const KWPicInfo*)));
    connect(motor, SIGNAL(NewLoaded(const KWPicInfo*,int,int)), this, SLOT(TitleBar(const KWPicInfo*)));
    connect(zoomerObj, SIGNAL(ReZoomed(int)), pasekDolny, SLOT(UstawZoom(int)));
    connect(wyswietlacz, SIGNAL(ZoomInReq()), zoomerObj, SLOT(ZoomIn()));
    connect(wyswietlacz, SIGNAL(ZoomOutReq()), zoomerObj, SLOT(ZoomOut()));
}

void MainWindow::InitShortcuts()
{
    QShortcut* s =  new QShortcut(QKeySequence(Qt::Key_Plus), this);                //Powiększenie obrazu
    connect(s, SIGNAL(activated()), zoomerObj, SLOT(ZoomIn()));

    s =  new QShortcut(QKeySequence(Qt::Key_Minus), this);                          //Pomniejszenie obrazu
    connect(s, SIGNAL(activated()), zoomerObj, SLOT(ZoomOut()));

    s =  new QShortcut(QKeySequence(Qt::Key_Delete), this);                         //Usuń
    connect(s, SIGNAL(activated()), this, SLOT(Usun()));

    s =  new QShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_N), this);        //Nowe okno
    connect(s, SIGNAL(activated()), this, SLOT(NoweOkno()));
    s->setEnabled(false);
    rep.push_back(s);

    s =  new QShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_O), this);        //Otwieranie nowego obrazu
    connect(s, SIGNAL(activated()), this, SLOT(Otworz()));
    s->setEnabled(false);
    rep.push_back(s);

    s =  new QShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_H), this);        //Zoom reset
    connect(s, SIGNAL(activated()), zoomerObj, SLOT(ResetZoom()));
    s->setEnabled(false);
    rep.push_back(s);

    s =  new QShortcut(QKeySequence(Qt::ControlModifier | Qt::Key_M), this);        //Losowy obraz
    connect(s, SIGNAL(activated()), motor, SLOT(RandImg()));
    s->setEnabled(false);
    rep.push_back(s);

    s =  new QShortcut(QKeySequence(Qt::Key_I), this);                              //Informacje o pliku
    connect(s, SIGNAL(activated()), this, SLOT(FileInfo()));
    s->setEnabled(false);
    rep.push_back(s);

    s =  new QShortcut(QKeySequence(Qt::Key_F2), this);                             //Zmiana nazwy
    connect(s, SIGNAL(activated()), this, SLOT(Rename()));
    s->setEnabled(false);
    rep.push_back(s);
}

void MainWindow::testOpenA()
{
    motor->Otworz("imga.jpg");
}

void MainWindow::TitleBar(const KWPicInfo *pi)
{
    QString name = pi->fileInfo.fileName();
    if(testRun)                                                                     //Wersja testowa
        this->setWindowTitle("KWView <Tryb testowy> - " + name);
    else
        this->setWindowTitle("KWView - " + name);                                   //Główny pasek okna
}

void MainWindow::About()
{
    QString t;
    about ab;
    t = "KWView\n\nAutor: " + ab.autor + "\n\nWersja programu: " + ab.wersja + "\nZ dnia ";
    t += ab.data;

    if(testRun)
        t += "\n\nTryb testowy.";

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
//    long long total=0, open=0, show=0, zoom=0;
//    std::chrono::steady_clock::time_point czas_start, czas_end;
//    motor->Otworz("bench/imga.jpg");

//    for(int i=0;i<43;i++)
//    {
//        czas_start = std::chrono::steady_clock::now();
//        motor->Next();
//        czas_end = std::chrono::steady_clock::now();
//        total += std::chrono::duration_cast<std::chrono::microseconds>(czas_end - czas_start).count();
//        open += motor->getTimeUs();
//        show += wyswietlacz->getTimeUs();

//        wyswietlacz->SetZoom(1.95);
//        zoom += std::chrono::duration_cast<std::chrono::microseconds>(czas_end - czas_start).count();
//        total += std::chrono::duration_cast<std::chrono::microseconds>(czas_end - czas_start).count();
//    }
//    printf("Wyniki pomiaru:\nCałkowity czas: %d ms\n", (int)(total/1000));
//    printf("Czas otwierania:: %d ms\n", (int)(open/1000));
//    printf("Czas wyświetlania:: %d ms\n", (int)(show/1000));
//    printf("Czas skalowania:: %d ms\n", (int)(zoom/1000));
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
        QMainWindow::event(event);            //wyswietlacz->Odswiez();
    else
        QMainWindow::event(event);                                                  //Przekazanie nieprzechwyconych eventów
    return 0;
}

void MainWindow::Otworz(QString adres)
{
    motor->Otworz(adres);
}

void MainWindow::Otworz()
{
    QString adres = QFileDialog::getOpenFileName(0, tr("Otwórz plik"), lastOpenedDir, motor->Filters());  //Wyświetlanie okna wybierania pliku
    if(!adres.isEmpty())
    {
        motor->Otworz(adres);
        this->AddToRec(adres);
        lastOpenedDir = adres;
    }
}

void MainWindow::Reopen()
{
    motor->Otworz(picObj->GetPicInfo()->fileInfo.filePath());
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

void MainWindow::FileInfo()
{
    if(!fileInfo)
        fileInfo = new KW3rdFileInfo(this);
//    if(motor->isOpened())
        fileInfo->GenInfo(picObj->GetPicInfo());
//    else
//        fileInfo->GenInfo(QFileInfo(), QPixmap(), 0, 0);
    fileInfo->show();
}

void MainWindow::AddToRec(QString rec)
{
    if(!rec.isEmpty())
        recentFiles.push_back(rec);                                                 //Dodawanie adresu
    while(recentFiles.length()>maxRecentFiles)
        recentFiles.removeFirst();                                                  //Usuwanie najstarszych elementów po przekroczeniu limitu

    menu->UpdateRecent(recentFiles);                                                //Uaktualnianie menu
}

void MainWindow::OpenRec(int i)
{
    if((i>=0)&&(i<recentFiles.length()))
        motor->Otworz(recentFiles.at(i));
}

void MainWindow::Rename()
{
    bool ok;
    QString oldName = picObj->GetPicInfo()->fileInfo.filePath();
    QFile plik(oldName);                                                            //Obiekt pliku
    QFileInfo fi(plik);

    QString newName = QInputDialog::getText(this, tr("Zmiana nazwy"), tr("Podaj nową nazwę"), QLineEdit::Normal, fi.baseName(), &ok);
    if (ok && !newName.isEmpty())
    {
        if(!newName.endsWith("."+fi.suffix()))
            newName+="."+fi.suffix();                                               //Dodawanie rozszerzenia
        if(plik.rename(newName))                                                    //Zmiana nazwy
        {
            QFileInfo fi2(plik);
            motor->Otworz(fi2.absoluteFilePath());                                  //Ponowne otwieranie pliku
        }
    }
}

void MainWindow::LoadRecentFIles()
{
    QFile plik(baseDirectory+"recent.kwv");
    if(!plik.open(QIODevice::ReadOnly))                                             //Otwieranie pliku z danymi
        return;

    QTextStream ts(&plik);
    while(!ts.atEnd())
        recentFiles.push_back(ts.readLine());

    plik.close();                                                                   //Zamykanie pliku
}

void MainWindow::SaveRecentFIles()
{
    QFile plik(baseDirectory+"recent.kwv");
    if(!plik.open(QIODevice::WriteOnly | QIODevice::Truncate))                      //Otwieranie pliku z danymi
        return;

    QTextStream ts(&plik);
    for(int i=0;i<recentFiles.length();++i)
        ts << recentFiles.at(i)+"\r\n";

    plik.close();
}

void MainWindow::LoadRecentPlaces()
{
    lastCopyDir = "./";                                                             //Domyślne wartości
    lastOpenedDir = "./";
    lastMoveDir = "./";

    QFile plik(baseDirectory+"lastDirs.kwv");
    if(!plik.open(QIODevice::ReadOnly))                                             //Otwieranie pliku z danymi
        return;

    QTextStream ts(&plik);
    QStringList linie;
    while(!ts.atEnd())
        linie << ts.readLine();

    plik.close();

    for(int i=0;i<linie.length();++i)                                               //Czytanie wartości
    {
        if(linie.at(i).mid(0, 3)=="-CD")                                            //Ostatni folder kopiowania
        {
            QFileInfo td(linie.at(i).mid(3));
            lastCopyDir = td.path();
        }
        if(linie.at(i).mid(0, 3)=="-MD")                                            //Ostatni folder kopiowania
        {
            QFileInfo td(linie.at(i).mid(3));
            lastMoveDir = td.path();
        }
        if(linie.at(i).mid(0, 3)=="-OD")                                            //Ostatni folder otwierania
        {
            QFileInfo td(linie.at(i).mid(3));
            lastOpenedDir = td.absoluteFilePath();
        }
    }
}

void MainWindow::SaveRecentPlaces()
{
    QFile plik(baseDirectory+"lastDirs.kwv");
    if(!plik.open(QIODevice::WriteOnly | QIODevice::Truncate))                      //Otwieranie pliku z danymi
        return;

    QTextStream ts(&plik);
    ts << "-CD"+lastCopyDir+QString(lastCopyDir.endsWith('/')?"":"/")+"\n";
    ts << "-MD"+lastMoveDir+QString(lastMoveDir.endsWith('/')?"":"/")+"\n";
    ts << "-OD"+lastOpenedDir+"\n";

    plik.close();
}

void MainWindow::Usun()
{
    if(!picObj->GetPicInfo()->fileInfo.isFile())                                    //Jeżeli żaden obraz nie jest wczytany
        return;

    QMessageBox msgBox(QMessageBox::Question, tr("Potwierdź"), tr("Czy jesteś pewien, że chcesz usunąć obraz ") + picObj->GetPicInfo()->fileInfo.filePath() + "?",
                       QMessageBox::Yes | QMessageBox::No, this);
    msgBox.setButtonText(QMessageBox::Yes, tr("Tak"));
    msgBox.setButtonText(QMessageBox::No, tr("Anuluj"));
    msgBox.setIcon(QMessageBox::Question);

    if(msgBox.exec() == QMessageBox::No)                                            //Anuluj -> wyjście
        return;

    QFile plik(picObj->GetPicInfo()->fileInfo.filePath());                          //Usuwanie pliku
    plik.remove();

    if(motor->DlugoscListy()>1)                                                     //Jeżeli na liście były inne pliki
    {
        motor->Next();                                                              //Otwórz kolejny obraz
//        motor->Otworz(motor->Adres());
    }
}

void MainWindow::Kopiuj()
{
    if(!picObj->GetPicInfo()->fileInfo.isFile())                                    //Brak pliku do skopiowania
        return;
    QFile startPlik(picObj->GetPicInfo()->fileInfo.filePath());

    QString startString = lastCopyDir+"/"+startPlik.fileName();                     //Początkowy adres
    QString suffixes = "Pliki " + QFileInfo(startPlik).suffix() + " (*." + QFileInfo(startPlik).suffix() + ")";

    QString fileName = QFileDialog::getSaveFileName(this, tr("Wskaż adres docelowego pliku"), startString, suffixes, 0, QFileDialog::ReadOnly);

    if(!fileName.isEmpty())
    {
        QFileInfo endFile(fileName);
        lastCopyDir = endFile.path();                                               //Zapisywanie użytego adresu

        startPlik.copy(fileName);                                                   //Kopiowanie pliku
    }
}

void MainWindow::Przenies()
{
    if(!picObj->GetPicInfo()->fileInfo.isFile())                                    //Brak pliku
        return;
    QFile startPlik(picObj->GetPicInfo()->fileInfo.filePath());

    QString startString = lastMoveDir+"/"+startPlik.fileName();                     //Początkowy adres
    QString suffixes = "Pliki " + QFileInfo(startPlik).suffix() + " (*." + QFileInfo(startPlik).suffix() + ")";

    QString fileName = QFileDialog::getSaveFileName(this, tr("Wskaż adres docelowego pliku"), startString, suffixes, 0);

    if(fileName.isEmpty())
        return;
    if(!QFileInfo(fileName).absoluteFilePath().compare(QFileInfo(startPlik).absoluteFilePath()))
        return;

    bool rem = false;

    QFileInfo endFile(fileName);
    lastMoveDir = endFile.path();                                                   //Zapisywanie użytego adresu

    if(startPlik.copy(fileName))                                                    //Kopiowanie pliku
    {
        if(startPlik.remove())                                                      //Usuwanie poprzedniego pliku
            rem = true;
    }

    if(!rem)
    {
        QFile remFile(fileName);
        remFile.remove();                                                           //Usuwanie skopiowanego pliku w przypadku niepowodzenia operacji

        QMessageBox::information(this, tr("Błąd przenoszenia"), tr("Nie można przenieść pliku ")+startPlik.fileName()+".");
    }
}

void MainWindow::NoweOkno()
{
    QStringList arguments;
    if(picObj->GetPicInfo()->fileInfo.isFile())
        arguments << picObj->GetPicInfo()->fileInfo.filePath();                     //Tworzenie argumentów programu

    if(testRun)
        arguments << "--test";

    QProcess *myProcess = new QProcess(NULL);
    myProcess->start(qApp->arguments()[0], arguments);                              //Uruchamianie nowego procesu
}
