#ifndef CRTANJE_H
#define CRTANJE_H

#include <QList>
#include <QPainter>
#include <QColor>
#include <QGraphicsItem>

#include "../Cons/geom.hpp"
#include "../Cons/tacka.hpp"
#include "../Cons/oblik.hpp"
#include "geom_graphics_view.hpp"

namespace crtanje {


// Graficki objekat na sceni koji ce nam predstavljati tacku.
class ctacka : public QGraphicsItem
{
public:
    ctacka();
    ctacka(const geom::tacka& t);
    ctacka(const geom::Element x, const geom::Element y);

    /***** OVO SAMO AKO BAS BUDE VREMENA! *****
     crtanje::ctacka::ctacka(const geom::tacka& , QColor );
     crtanje::ctacka::ctacka(const geom::Elem , const geom::Elem , QColor )
     *********************************************/

    QRectF boundingRect() const Q_DECL_OVERRIDE;

    void paint(QPainter* , const QStyleOptionGraphicsItem* , QWidget* ) Q_DECL_OVERRIDE;

    void setColor(QColor , QColor );

    QPainterPath shape() const Q_DECL_OVERRIDE;

private:
    geom::tacka odgovarajuca_tacka;
    QColor boja;
    QColor kliknuto;
};



class clinija : public QGraphicsItem
{
public:
    clinija(const geom::Element x_pocetak, const geom::Element y_pocetak,
            const geom::Element x_kraj,    const geom::Element y_kraj);

    clinija(const geom::Element x_pocetak, const geom::Element y_pocetak,
                const geom::Element x_kraj,    const geom::Element y_kraj, QColor boja_);

    QRectF boundingRect() const Q_DECL_OVERRIDE;

    void paint(QPainter* , const QStyleOptionGraphicsItem* , QWidget* ) Q_DECL_OVERRIDE;

    void setColor(QColor );

    QPainterPath shape() const Q_DECL_OVERRIDE;
private:
    geom::tacka pocetak;
    geom::tacka kraj;
    QColor boja;
};

// TODO: Singletoni: Jer, nema smisla da se pravi vise osa
class cxosa : public clinija {
private:
    cxosa() : clinija(X_BOUND_GRAPHIC_SCENE, 0,
                      X_BOUND_GRAPHIC_SCENE + WIDTH_GRAPHIC_SCENE, 0) {}

    ~cxosa() override = default;
    static cxosa* osa_;

public:
    static cxosa* osa();
    void paint(QPainter* , const QStyleOptionGraphicsItem* , QWidget* ) override;

};

class cyosa : public clinija {
private:
    cyosa() : clinija(0, Y_BOUND_GRAPHIC_SCENE,
                      0, Y_BOUND_GRAPHIC_SCENE + HEIGHT_GRAPHIC_SCENE) {}

    ~cyosa() override = default;
    static cyosa* osa_;

public:
    static cyosa* osa();
    void paint(QPainter* , const QStyleOptionGraphicsItem* , QWidget* ) override;

};

/***** OBLICI *****/
/*****************/

// Element na grafickoj sceni koji predstavlja poligon
class cpoligon : public QGraphicsItem {
public:
    cpoligon(geom::poly& );

    QRectF boundingRect() const Q_DECL_OVERRIDE;

    void paint(QPainter* , const QStyleOptionGraphicsItem* , QWidget* ) Q_DECL_OVERRIDE;

    QPainterPath shape() const Q_DECL_OVERRIDE;

    geom::poly odgovarajuci_poligon;
};

// Element na grafickoj sceni koji predstavlja elipsa
class celipsa : public QGraphicsItem {
public:
    celipsa(geom::elipsa& );

    QRectF boundingRect() const Q_DECL_OVERRIDE;

    void paint(QPainter* , const QStyleOptionGraphicsItem* , QWidget* ) Q_DECL_OVERRIDE;

    QPainterPath shape() const Q_DECL_OVERRIDE;

    geom::elipsa odgovarajuca_elipsa;
};

// Element na grafickoj sceni koji predstavlja krug
class ckrug : public QGraphicsItem {
public:
    ckrug(geom::krug& );

    QRectF boundingRect() const Q_DECL_OVERRIDE;

    void paint(QPainter* , const QStyleOptionGraphicsItem* , QWidget* ) Q_DECL_OVERRIDE;

    QPainterPath shape() const Q_DECL_OVERRIDE;

    geom::krug odgovarajuci_krug;
};
}
#endif // CRTANJE_H
