#ifndef GEOM_GRAPHICS_VIEW_H
#define GEOM_GRAPHICS_VIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QWheelEvent>
#include <qmath.h>
#include <QVector>
#include <QPointF>
#include <QPoint>
#include <QGraphicsItem>

#include <vector>
#include "/home/milena/Learning_Cetvrta_godina/Razvoj_softvera/RS_Projekat/RS016-geopaint/Cons/geom.hpp"
#include "/home/milena/Learning_Cetvrta_godina/Razvoj_softvera/RS_Projekat/RS016-geopaint/Cons/util.hpp"


class geom_graphics_view : public QGraphicsView
{
public:
    geom_graphics_view(QWidget* );

    void nacrtaj_tacku();
    void nacrtaj_tacku(const geom::tacka& );
/*
    void nacrtajOblik(const Shape& );
*/

protected:
    void drawBackground(QPainter *, const QRectF &) override;
private:
    std::vector<geom::tacka> sve_tacke;
/*
    void nacrtaj_koordinatne_ose();
    void nacrtaj_mrezu();*/
};

#endif // GEOM_GRAPHICS_VIEW_H
