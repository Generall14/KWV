#ifndef KWMENU_H
#define KWMENU_H

#include <QObject>
#include <QMenu>
#include <QAction>
#include "osrc/mwptr.h"

class MainWindow;

class KWMenu : public QObject, public MWPtr
{
    Q_OBJECT
public:
    KWMenu(MainWindow* mw);
    ~KWMenu();

    void UpdateRecent(QVector<QString> recVec);                                                                             //Wyświetlanie listy poprzednich plików

private:
    QVector<QAction*> fileDependent;

    QMenu* menuPlik;
    QAction* akcjaOtworz;
    QAction* akcjaOtworzPonownie;
    QMenu* menuRecent;
    QAction* akcjaZamknijPlik;
    QAction* akcjaRename;
    QAction* akcjaKopiujPlik;
    QAction* akcjaPrzeniesPlik;
    QAction* akcjaNoweOkno;
    QAction* akcjaZamknij;

    QMenu* menuEdycja;
    QAction* akcjaUsun;
    QAction* akcjaGifManager;

    QMenu* menuObraz;
    QAction* akcjaFileInfo;

    QMenu* menuWidok;
    QAction* akcjaResetZoom;
    QAction* akcjaLosowy;

    QMenu* menuPomoc;
    QAction* akcjaOProgramie;

    QVector<QAction*> recentActions;

    void InitMenus();

signals:
    void Otworz();
    void About();
    void OpenRec(int nr);                                                                                                   //Otwieranie poprzednich plików
    void CloseFile();

public slots:
    void FileOn();
    void FileOff();
};

#endif // KWMENU_H
