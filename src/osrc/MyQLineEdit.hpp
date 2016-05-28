/*  Klasa ma tylko przechwycić i nie przepuścić sygnału ENTER (aby zmiana obrazu po wpisaniu numeru i zatwierdzeniu nie powodowała
 *  przejścia do trybu fullscreen).
 */

#ifndef MYQLINEEDIT_HPP
#define MYQLINEEDIT_HPP

#include <QLineEdit>
#include <QKeyEvent>
#include <QObject>

class MyQLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    MyQLineEdit(QString t):QLineEdit(t){}
private:
    virtual void keyPressEvent(QKeyEvent *event)
    {
        if((event->key()==Qt::Key_Enter)||(event->key()==Qt::Key_Return))
            emit editingFinished();
        else
            QLineEdit::keyPressEvent(event);
    }
};

#endif
