#include <vector>

#include "geom_graphics_view.hpp"
#include "crtanje.hpp"
#include "../Cons/geom.hpp"
#include "../Cons/util.hpp"

geom_graphics_view::geom_graphics_view(QWidget* parent)
    : QGraphicsView(parent)
{
   QGraphicsScene* scena = new QGraphicsScene(this);

   scena->setSceneRect(X_BOUND_GRAPHIC_SCENE, Y_BOUND_GRAPHIC_SCENE,
                          WIDTH_GRAPHIC_SCENE, HEIGHT_GRAPHIC_SCENE);

   setScene(scena);

   setRenderHint(QPainter::Antialiasing);

   /***** Ovde samo proveravamo nase metode *****/
   nacrtaj_tacku();
   nacrtaj_tacku(geom::tacka (70,80));
   /***** ********************************** *****/
}

void geom_graphics_view::drawBackground(QPainter* painter, const QRectF& pozadina_)
{
    Q_UNUSED(pozadina_);

    QRectF pozadina = this->sceneRect();
    painter->setOpacity(0.8);
    painter->setBrush(QColor(159,189,223));

//    nacrtaj_koordinatne_ose();
    painter->drawRect(pozadina);
    nacrtaj_mrezu();

}

void geom_graphics_view::nacrtaj_tacku()
{
    crtanje::ctacka* nova_tacka = new crtanje::ctacka();
    nova_tacka->setPos(0, 0);
    scene()->addItem(nova_tacka);
    sve_tacke.push_back(geom::PodTip (0,0));
}

void geom_graphics_view::nacrtaj_tacku(const geom::tacka& t) {
    crtanje::ctacka* nova_tacka = new crtanje::ctacka(t);
    nova_tacka->setPos(t[0], t[1]);
    scene()->addItem(nova_tacka);
    sve_tacke.push_back(geom::PodTip (0,0));
}

void geom_graphics_view::nacrtaj_liniju (const geom::tacka& pocetak, const geom::tacka& kraj) {
    crtanje::clinija* linija = new crtanje::clinija(pocetak[0], pocetak[1],
                             kraj[0], kraj[1]);
    scene()->addItem(linija);

}


/*
void geom_graphics_view::nacrtaj_koordinatne_ose() {

}*/

// FIXME: Vidljivost mreze
void geom_graphics_view::nacrtaj_mrezu() {
    for (auto i= X_BOUND_GRAPHIC_SCENE; i<= (X_BOUND_GRAPHIC_SCENE + WIDTH_GRAPHIC_SCENE); i+= 20)
    {
        geom::tacka pocetak_ (i,Y_BOUND_GRAPHIC_SCENE);
        geom::tacka kraj_   (i, (Y_BOUND_GRAPHIC_SCENE + HEIGHT_GRAPHIC_SCENE));
        nacrtaj_liniju(pocetak_, kraj_);
    }

    for (auto i= Y_BOUND_GRAPHIC_SCENE; i<= (Y_BOUND_GRAPHIC_SCENE + HEIGHT_GRAPHIC_SCENE); i+= 20)
    {
        geom::tacka pocetak_ (X_BOUND_GRAPHIC_SCENE, i);
        geom::tacka kraj_   ((X_BOUND_GRAPHIC_SCENE + WIDTH_GRAPHIC_SCENE), i);
        nacrtaj_liniju(pocetak_, kraj_);
    }
}
