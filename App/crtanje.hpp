#ifndef CRTANJE_H
#define CRTANJE_H

#include <QList>
#include <QPainter>
#include <QColor>
#include <QGraphicsItem>

#include "../Cons/geom.hpp"
#include "geom_graphics_view.hpp"

namespace crtanje {


// Graficki objekat na sceni koji ce nam predstavljati tacku.
class ctacka : public QGraphicsItem
{
public:
    ctacka();
    ctacka(const geom::tacka& t);
    ctacka(const geom::Elem x, const geom::Elem y);

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
    clinija(const geom::Elem x_pocetak, const geom::Elem y_pocetak,
            const geom::Elem x_kraj,    const geom::Elem y_kraj);

    clinija(const geom::Elem x_pocetak, const geom::Elem y_pocetak,
                const geom::Elem x_kraj,    const geom::Elem y_kraj, QColor boja_);

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

    ~cxosa() = default;
    static cxosa* osa_;

public:
    static cxosa* osa();
    void paint(QPainter* , const QStyleOptionGraphicsItem* , QWidget* ) override;

};

class cyosa : public clinija {
private:
    cyosa() : clinija(0, Y_BOUND_GRAPHIC_SCENE,
                      0, Y_BOUND_GRAPHIC_SCENE + HEIGHT_GRAPHIC_SCENE) {}

    ~cyosa() = default;
    static cyosa* osa_;

public:
    static cyosa* osa();
    void paint(QPainter* , const QStyleOptionGraphicsItem* , QWidget* ) override;

};
}


#endif // CRTANJE_H
