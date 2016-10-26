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

class KWZoomer : public QObject
{
    Q_OBJECT
public:
    KWZoomer(QGraphicsScene* ngView);

    static const int MIN_ZOOM = 5;
    static const int MAX_ZOOM = 500;

    enum defTypes
    {
        noZoom,
        limScreen,
        limWidth,
        limHeight
    };

    void SetDefaultMode(defTypes newType, int newWidth=-1, int newHeight=-1);                                               //Ustawia domyślny trub powiększenia
    void SetLimits(int newWidth, int newHeight);                                                                            //Ustawia nowe limity startowe obrazu
    void ResetZoom();                                                                                                       //Resetuje powiększenie
    void NewPic(QPixmap* newPic, bool resetZoom=true);                                                                      //Ustawia nowy obraz oraz resetuje powiększenie

    void SetZoom(float newZoom);                                                                                            //Ustawia nowe powiększenie

private:
    QGraphicsScene* gView;                                                                                                  //Wskaźnik na wyświetlacz

    QPixmap* lastPic;                                                                                                       //Wskaźnik na ostatnio używany obraz
    float currZoom;                                                                                                         //Aktualne powiększenie
    defTypes defMode;                                                                                                       //Domyślny tryb powiększania
    int limitWidth;                                                                                                         //Początkowy limit szerokości obrazu
    int limitHeight;                                                                                                        //Początkowy limit wysokoście obrazu

    Qt::TransformationMode tranMode;                                                                                        //Metoda skalowania

    void sendZoom();                                                                                                        //Wysyła przeskalowany obraz

signals:
    void ReZoomed(int newZoom);                                                                                             //Sygnał zmiany powiększenia
};

#endif // KWZOOMER_H
