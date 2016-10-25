#ifndef MWPTR_H
#define MWPTR_H

class MainWindow;

class MWPtr
{
public:
    MWPtr(MainWindow* mw);

protected:
    MainWindow* MWw;
};

#endif // MWPTR_H
