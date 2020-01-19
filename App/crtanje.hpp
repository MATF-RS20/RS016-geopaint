#ifndef CRTANJE_H
#define CRTANJE_H

#include <QList>
#include <QPainter>
#include <QColor>
#include <QGraphicsItem>

#include "/home/milena/Learning_Cetvrta_godina/Razvoj_softvera/RS_Projekat/RS016-geopaint/Cons/geom.hpp"

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
}


#endif // CRTANJE_H
