#include "crtanje.hpp"
#include "../Cons/geom.hpp"
#include "Boje.hpp"
#include "geom_graphics_view.hpp"

#include <QPointF>

// Element na grafickoj sceni koji predstavlja x-osu.
// Implementiran je koriscenjem principa UNIKAT obrasca
// za projektovanje (eng. SINGLETON), jer nema smisla da
// imamo vise primeraka x-ose.
crtanje::cxosa* crtanje::cxosa::osa_ = nullptr;

crtanje::cxosa* crtanje::cxosa::osa(){
    if (osa_ == nullptr){
        osa_ = new cxosa();
        osa_->setZValue(50);
    }

    return osa_;
}

void crtanje::cxosa::paint(QPainter* painter, const QStyleOptionGraphicsItem*  option, QWidget* widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QPen pencil(KLIKNUTO_BOJA);
    pencil.setWidth(PENCIL_WEIGHT_AXIS);
    painter->setPen(pencil);
    painter->drawLine(QPointF(X_BOUND_GRAPHIC_SCENE,0),
                      QPointF(X_BOUND_GRAPHIC_SCENE + WIDTH_GRAPHIC_SCENE, 0));

    for (auto i = X_BOUND_GRAPHIC_SCENE; i <= X_BOUND_GRAPHIC_SCENE + WIDTH_GRAPHIC_SCENE; i += MESH_CELL)
        painter->drawLine(QPointF(i, POINT_ON_AXIS_SIZE),
                          QPointF(i, -POINT_ON_AXIS_SIZE));
}
