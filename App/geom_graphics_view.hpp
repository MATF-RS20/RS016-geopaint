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
#include "../Cons/geom.hpp"
#include "../Cons/util.hpp"

#define X_BOUND_GRAPHIC_SCENE (-2000)
#define Y_BOUND_GRAPHIC_SCENE (-2000)
#define WIDTH_GRAPHIC_SCENE   (4000)
#define HEIGHT_GRAPHIC_SCENE  (4000)
#define MESH_CELL (50)
#define POINT_ON_AXIS_SIZE (3)
#define PENCIL_WEIGHT_AXIS (2)

// Nasa podklasa grafickog pogleda na scenu
class geom_graphics_view : public QGraphicsView
{
public:
    geom_graphics_view(QWidget* );
    // TODO: Dodaj i konstruktore sa realnim argumentima

    /***** CRTANJE PRIMITIVA *****/
    void nacrtaj_tacku();
    void nacrtaj_tacku(const geom::tacka& );

    void nacrtaj_liniju(const geom::tacka& , const geom::tacka&);
    void nacrtaj_x_osu();
    void nacrtaj_y_osu();
    void nacrtaj_koordinatne_ose() {
        nacrtaj_x_osu();
        nacrtaj_y_osu();
    }

    /***** CRTANJE OBLIKA *****/
    void nacrtaj_elipsu(geom::elipsa& e);
    void nacrtaj_krug(geom::krug& k);
    void nacrtaj_poligon(geom::poly& p);

protected:
    void drawBackground(QPainter *, const QRectF &) override;
private:
    std::vector<geom::PodTip> sve_tacke;
    void nacrtaj_mrezu();
};

#endif // GEOM_GRAPHICS_VIEW_H
