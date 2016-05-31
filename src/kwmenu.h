#ifndef KWMENU_H
#define KWMENU_H

#include <QObject>
#include <QMenu>
#include <QAction>
#include "mwptr.h"

class MainWindow;

class KWMenu : public QObject, public MWPtr
{
    Q_OBJECT
public:
    KWMenu(MainWindow* mw);
    ~KWMenu();

private:
    QMenu* menuPlik;
    QAction* akcjaOtworz;
    QAction* akcjaZamknij;
    QAction* akcjaNoweOkno;

    QMenu* menuEdycja;
    QAction* akcjaUsun;
    QAction* akcjaGifManager;

    QMenu* menuWidok;
    QAction* akcjaResetZoom;
    QAction* akcjaLosowy;

    QMenu* menuPomoc;
    QAction* akcjaOProgramie;

    void InitMenus();

signals:
    void Otworz();
    void About();

public slots:
};

#endif // KWMENU_H
