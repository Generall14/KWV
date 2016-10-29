#ifndef KW3RDFILEINFO_H
#define KW3RDFILEINFO_H

#include <QObject>
#include <QDialog>
#include <QLineEdit>
#include <QFileInfo>
#include <QPixmap>
#include "../usrc/kwpicinfo.h"

class KW3rdFileInfo : public QDialog
{
    Q_OBJECT
public:
    KW3rdFileInfo(QWidget* parent);                                                                                         //Konstruktor

    void GenInfo(const KWPicInfo *pi);                                                                                      //Generuje informacje o pliku

private:
    QLineEdit* nazwaPliku;
    QLineEdit* katalog;
    QLineEdit* sciezkaPliku;
    QLineEdit* rozmiarObrazu;
    QLineEdit* glebiaKolorow;
    QLineEdit* rozmiarNaDysku;
    QLineEdit* czasLadowania;
    QLineEdit* dataEdycji;

    const int constSzer = 155;
    const int constSzerT = 230;
    const int constWys = 23;

    void InitWidgets();                                                                                                     //Inicjalizacja widgetów
};

#endif // KW3RDFILEINFO_H
