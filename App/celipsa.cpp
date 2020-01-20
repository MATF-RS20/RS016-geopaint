#include "../Cons/geom.hpp"
#include "crtanje.hpp"

#include "Boje.hpp"
#include <cstdlib>

// klasa celipsa : Element graficke scene koji predstavlja elipsu
crtanje::celipsa::celipsa(geom::elipsa& e)
    : odgovarajuca_elipsa(e) {};

// Crtanje elementa klase celipsa (nasledjena iz klase elementa graficke scene)
void crtanje::celipsa::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    auto centar = odgovarajuca_elipsa.tacke()[0];
    auto a = odgovarajuca_elipsa.tacke()[1][0] - centar[0];
    auto b = odgovarajuca_elipsa.tacke()[2][1] - centar[1];

    if(a < 0)
        a = (-a);
    if(b < 0)
        b = (-b);

    QPen pen;

    pen.setColor(OBLIK_BOJA);
    pen.setWidthF(0.5);
    painter->setPen(pen);

    painter->drawEllipse(centar[0] - a, centar[1] - b, 2 * a, 2 * b);
}

// Pomocna funkcija za detekciju kolizije (nasledjena iz klase elementa graficke scene)
QPainterPath crtanje::celipsa::shape() const
{
    QPainterPath path;
    auto centar = odgovarajuca_elipsa.tacke()[0];
    auto a = odgovarajuca_elipsa.tacke()[1][0] - centar[0];
    auto b = odgovarajuca_elipsa.tacke()[2][1] - centar[1];

    if(a < 0)
        a = (-a);
    if(b < 0)
        b = (-b);

    path.addEllipse(centar[0] - 1.5 * a, centar[1] - 1.5 * b, 4 * a, 4 * b);
    return path;
}

// Pomocna funkcija za iscrtavanje (nasledjena iz klase elementa graficke scene)
QRectF crtanje::celipsa::boundingRect() const
{
    auto centar = odgovarajuca_elipsa.tacke()[0];
    auto a = odgovarajuca_elipsa.tacke()[1][0] - centar[0];
    auto b = odgovarajuca_elipsa.tacke()[2][1] - centar[1];

    if(a < 0)
        a = (-a);
    if(b < 0)
        b = (-b);

    return QRectF(centar[0] - 2 * a, centar[1] - 2 * b, 4 * a, 4 * b);
}


