#include "crtanje.hpp"
#include "../Cons/geom.hpp"
#include "Boje.hpp"

#include <QPointF>

crtanje::clinija::clinija(const geom::Elem x_pocetak, const geom::Elem y_pocetak,
                 const geom::Elem x_kraj,    const geom::Elem y_kraj)
    : pocetak(geom::tacka(x_pocetak, y_pocetak)) , kraj(geom::tacka(x_kraj, y_kraj)), boja(PODRAZUMEVANA_BOJA) {this->setZValue(10);};

crtanje::clinija::clinija(const geom::Elem x_pocetak, const geom::Elem y_pocetak,
                 const geom::Elem x_kraj,    const geom::Elem y_kraj, QColor boja_)

 : pocetak(geom::tacka(x_pocetak, y_pocetak)) , kraj(geom::tacka(x_kraj, y_kraj)), boja(boja_) {this->setZValue(10);};

QRectF crtanje::clinija::boundingRect() const  {
    auto min_x = (pocetak[0] < kraj[0]) ? 2*pocetak[0] : 2*kraj[0];
    auto min_y = (pocetak[1] < kraj[1]) ? 2*pocetak[1] : 2*kraj[1];

    return QRectF(min_x, min_y, 2 * abs(pocetak[0] - kraj[0]), 2 * abs(pocetak[1] - kraj[1]));
}

void crtanje::clinija::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QPen pencil(boja);
    pencil.setWidth(1);
    painter->setPen(pencil);
    painter->drawLine(QPointF(pocetak[0], pocetak[1]), QPointF(kraj[0], kraj[1]));
}

QPainterPath crtanje::clinija::shape() const {
    auto min_x = (pocetak[0] < kraj[0]) ? 2*pocetak[0] : 2*kraj[0];
    auto min_y = (pocetak[1] < kraj[1]) ? 2*pocetak[1] : 2*kraj[1];

    QPainterPath path;
    path.addRect(min_x, min_y, abs(pocetak[0] - kraj[0]), pocetak[1] - kraj[1]);
    return path;
}

void crtanje::clinija::setColor(QColor nova_boja) {
    boja = nova_boja;
}
