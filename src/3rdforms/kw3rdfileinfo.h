#ifndef KW3RDFILEINFO_H
#define KW3RDFILEINFO_H

#include <QObject>
#include <QDialog>
#include <QLineEdit>
#include <QFileInfo>
#include <QPixmap>

class KW3rdFileInfo : public QDialog
{
    Q_OBJECT
public:
    KW3rdFileInfo(QWidget* parent);                                                                                         //Konstruktor

    void GenInfo(QFileInfo fi, QPixmap ob, int kl);                                                                         //Generuje informacje o pliku

private:
    QLineEdit* nazwaPliku;
    QLineEdit* katalog;
    QLineEdit* sciezkaPliku;
    QLineEdit* rozmiarObrazu;

    const int constSzer = 120;
    const int constSzerT = 150;
    const int constWys = 23;

    void InitWidgets();                                                                                                     //Inicjalizacja widgetów
};

#endif // KW3RDFILEINFO_H
