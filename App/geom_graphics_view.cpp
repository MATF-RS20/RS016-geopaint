#include <vector>

#include "geom_graphics_view.hpp"
#include "crtanje.hpp"
#include "../Cons/geom.hpp"
#include "../Cons/util.hpp"

geom_graphics_view::geom_graphics_view(QWidget* parent)
    : QGraphicsView(parent)
{
   QGraphicsScene* scena = new QGraphicsScene(this);

   scena->setSceneRect(-2000, -2000, 4000, 4000);

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
/*  TODO:
    nacrtaj_koordinatne_ose();
    nacrtaj_mrezu();*/
    painter->drawRect(pozadina);
}

void geom_graphics_view::nacrtaj_tacku()
{
    crtanje::ctacka* nova_tacka = new crtanje::ctacka();
    nova_tacka->setPos(0, 0);
    scene()->addItem(nova_tacka);
    sve_tacke.push_back(geom::tacka(0,0));
}

void geom_graphics_view::nacrtaj_tacku(const geom::tacka& t) {
    crtanje::ctacka* nova_tacka = new crtanje::ctacka(t);
    nova_tacka->setPos(t[0], t[1]);
    scene()->addItem(nova_tacka);
    sve_tacke.push_back(t);
}

/*
void geom_graphics_view::nacrtaj_koordinatne_ose() {

}*/
