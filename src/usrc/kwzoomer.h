#ifndef KWZOOMER_H
#define KWZOOMER_H

/**
 * \class KWZoomer
 * @brief Klasa odpowiadająca za powiększanie obrazu.
 *
 * Pośredniczy pomiędzy KWPlayer a QGraphicsScene. Otrzymuje wskaźnik na ktualnie wyświetlaną klatkę, następnie ustawia w QGraphicsScene obiektu KWGraphicsView odpowiednio dopasowany obraz.
 *
 * Powiększenie ustawiane jest za pomocą funkcji KWZoomer::SetZoom, KWZoomer::ZoomIn, KWZoomer::ZoomOut oraz funkcji KWZoomer::ResetZoom która automatycznie dobiera powiększenie na
 * podstawie strategi opisanej w KWZoomer::defTypes.
 */

#include <QObject>
#include <QPixmap>
#include <QGraphicsScene>
#include <QSize>

class KWZoomer : public QObject
{
    Q_OBJECT
public:
    KWZoomer(QGraphicsScene* ngView, QObject *parent = 0);

    static constexpr float MIN_ZOOM = .1;                                                                                   //Minimalne powiększenie
    static constexpr float MAX_ZOOM = 5;                                                                                    //Maksymalne powiększenie
    static constexpr float DELTA_ZOOM = .05;                                                                                //Inkrementacja/dekrementacja powiększenia

    enum defTypes
    {
        noZoom,
        limScreen,
        limWidth,
        limHeight
    };

    void SetDefaultMode(defTypes newType, QSize newLimit=QSize(-1,-1));                                                     //Ustawia domyślny trub powiększenia


public slots:
    void NewPic(QPixmap* newPic, bool resetZoom=true);                                                                      //Ustawia nowy obraz oraz resetuje powiększenie
    void ReCalcZoom();                                                                                                      //Ponownie dopasowuje rozmiar
    void ResetZoom();                                                                                                       //Resetuje powiększenie
    void SetZoom(float newZoom);                                                                                            //Ustawia nowe powiększenie
    void ZoomIn();                                                                                                          //Powiększenie o jeden krok
    void ZoomOut();                                                                                                         //Pomniejszenie o jeden krok

    void SetLimits(QSize newLimit);                                                                                         //Ustawia nowe limity startowe obrazu

signals:
    void ReZoomed(int newZoom, QSize newSize);                                                                              //Sygnał zmiany powiększenia

private:
    QGraphicsScene* gView;                                                                                                  //Wskaźnik na wyświetlacz

    QPixmap* lastPic;                                                                                                       //Wskaźnik na ostatnio używany obraz
    float currZoom;                                                                                                         //Aktualne powiększenie
    defTypes defMode;                                                                                                       //Domyślny tryb powiększania
    QSize startLimit;                                                                                                       //Początkowy limit rozmiaru obrazu

    Qt::TransformationMode tranMode;                                                                                        //Metoda skalowania

    void sendZoom(bool sendSig=false);                                                                                      //Wysyła przeskalowany obraz

    bool touched;                                                                                                           //flaga określa czy rozmiar był zmieniany przez użytkownika
};

#endif // KWZOOMER_H
