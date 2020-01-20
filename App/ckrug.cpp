#include "../Cons/geom.hpp"
#include "crtanje.hpp"

#include "Boje.hpp"
#include <cstdlib>

// klasa ckrug : Element graficke scene koji predstavlja elipsu
crtanje::ckrug::ckrug(geom::krug& k)
    : odgovarajuci_krug(k) {};

// Crtanje elementa klase ckrug (nasledjena iz klase elementa graficke scene)
void crtanje::ckrug::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    auto centar = odgovarajuci_krug.tacke()[0];
    auto tacka_na_krugu = odgovarajuci_krug.tacke()[1];
    auto r = tacka_na_krugu[0] - centar[0];

    QPen pen;

    pen.setColor(OBLIK_BOJA);
    pen.setWidthF(0.5);
    painter->setPen(pen);

    painter->drawEllipse(centar[0] - r, centar[1] -r, 2*r, 2*r);
}

// Pomocna funkcija za detekciju kolizije (nasledjena iz klase elementa graficke scene)
QPainterPath crtanje::ckrug::shape() const
{
    QPainterPath path;
    auto centar = odgovarajuci_krug.tacke()[0];
    auto tacka_na_krugu = odgovarajuci_krug.tacke()[1];
    auto r = tacka_na_krugu[0] - centar[0];

    path.addEllipse(centar[0] - 1.5 * r, centar[1] - 1.5 * r, 4 * r, 4 * r);
    return path;
}

// Pomocna funkcija za iscrtavanje (nasledjena iz klase elementa graficke scene)
QRectF crtanje::ckrug::boundingRect() const
{
    auto centar = odgovarajuci_krug.tacke()[0];
    auto tacka_na_krugu = odgovarajuci_krug.tacke()[1];
    auto r = tacka_na_krugu[0] - centar[0];

    return QRectF(centar[0] - 2 * r, centar[1] - 2 * r, 4 * r, 4 * r);
}

