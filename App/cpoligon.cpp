#include "crtanje.hpp"
#include "../Cons/geom.hpp"

#include "Boje.hpp"

/*crtanje::cpoligon::cpoligon(geom::poly& p)
    : odgovarajuci_poligon(p) {};*/

void crtanje::cpoligon::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget)

    auto tacke = odgovarajuci_poligon.tacke();

    auto prva_tacka = tacke[0];
    auto pocetna_tacka = tacke[0];
    auto druga_tacka = tacke[1];

    ctacka c_pocetna_tacka (tacke[0]);
    ctacka c_druga_tacka (tacke[1]);
    c_pocetna_tacka.paint(painter,option,widget);
    c_druga_tacka.paint(painter,option,widget);

    QPen p(OBLIK_BOJA);
    p.setWidth(1);
    painter->setPen(p);
    painter->drawLine(pocetna_tacka[0], pocetna_tacka[1],
                      druga_tacka[0], druga_tacka[1]);

    for (auto i = 1; i < tacke.size() - 1 ; i++)
    {
        pocetna_tacka = tacke[i];
        druga_tacka   = tacke[i+1];

        ctacka c_druga_tacka (tacke[i+1]);
        c_druga_tacka.paint(painter,option,widget);
        painter->drawLine(pocetna_tacka[0], pocetna_tacka[1],
                          druga_tacka[0], druga_tacka[1]);
    }

    if (odgovarajuci_poligon.zatvoren())
    {
        painter->drawLine(prva_tacka[0], prva_tacka[1],
                          druga_tacka[0], druga_tacka[1]);
    }

}
/*
QRectF crtanje::cpoligon::boundingRect() const {

}
*/
