#include "crtanje.hpp"
#include "../Cons/geom.hpp"
#include "Boje.hpp"
#include "geom_graphics_view.hpp"

#include <QPointF>

crtanje::cyosa* crtanje::cyosa::osa_ = nullptr;

crtanje::cyosa* crtanje::cyosa::osa(){
    if (osa_ == nullptr){
        osa_ = new cyosa();
        osa_->setZValue(50);
    }

    return osa_;
}
//FIXME
void crtanje::cyosa::paint(QPainter* painter, const QStyleOptionGraphicsItem*  option, QWidget* widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QPen pencil(KLIKNUTO_BOJA);
    pencil.setWidth(PENCIL_WEIGHT_AXIS);
    painter->setPen(pencil);
    painter->drawLine(QPointF(0, Y_BOUND_GRAPHIC_SCENE),
                      QPointF(0, Y_BOUND_GRAPHIC_SCENE + HEIGHT_GRAPHIC_SCENE));

    for (auto i = Y_BOUND_GRAPHIC_SCENE; i <=Y_BOUND_GRAPHIC_SCENE + HEIGHT_GRAPHIC_SCENE; i += MESH_CELL)
        painter->drawLine(QPointF(POINT_ON_AXIS_SIZE, i),
                          QPointF(-POINT_ON_AXIS_SIZE, i));
}
