#include "crtanje.hpp"
#include "../Cons/geom.hpp"

#include "Boje.hpp"

crtanje::cpoligon::cpoligon(geom::poly& p)
    : odgovarajuci_poligon(p) {};

void crtanje::cpoligon::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget)

    QPen p;
    p.setWidth(1);
    painter->setPen(p);
    auto tacke = odgovarajuci_poligon.tacke();

    auto prva_tacka = tacke[0];
    auto pocetna_tacka = tacke[0];
    auto druga_tacka = tacke[1];

    ctacka c_pocetna_tacka (tacke[0]);
    ctacka c_druga_tacka (tacke[1]);


    painter->drawLine(pocetna_tacka[0], pocetna_tacka[1],
                      druga_tacka[0], druga_tacka[1]);

    for (auto i = 1; i < tacke.size() - 1 ; i++)
    {

        pocetna_tacka = tacke[i];
        druga_tacka   = tacke[i+1];

        ctacka c_druga_tacka (tacke[i+1]);
        c_druga_tacka.paint(painter,option,widget);
        painter->drawLine(pocetna_tacka[0], pocetna_tacka[1],
                          druga_tacka[0], druga_tacka[1]);
    }

    if (odgovarajuci_poligon.zatvoren())
    {
        painter->drawLine(prva_tacka[0], prva_tacka[1],
                          druga_tacka[0], druga_tacka[1]);
    }

}

QRectF crtanje::cpoligon::boundingRect() const {
       const auto [min_x, max_x] = std::minmax_element(std::cbegin(odgovarajuci_poligon.tacke()),
                                                        std::cend(odgovarajuci_poligon.tacke()),
                                                        [](const auto& a,
                                                           const auto& b)
                                                        {return a[0] < b[0];});

        const auto [min_y, max_y] = std::minmax_element(std::cbegin(odgovarajuci_poligon.tacke()),
                                                        std::cend(odgovarajuci_poligon.tacke()),
                                                        [](const auto& a,
                                                           const auto& b)
                                                        {return a[1] < b[1];});

        const auto mini_x = (*min_x)[0];
        const auto maxi_x = (*max_x)[0];

        const auto mini_y = (*min_y)[1];
        const auto maxi_y = (*max_y)[1];

        return QRectF(mini_x, mini_y, (maxi_x - mini_x), (maxi_y - mini_y));
}

QPainterPath crtanje::cpoligon::shape() const {
    QPainterPath path;
    const auto [min_x, max_x] = std::minmax_element(std::cbegin(odgovarajuci_poligon.tacke()),
                                                     std::cend(odgovarajuci_poligon.tacke()),
                                                     [](const auto& a,
                                                        const auto& b)
                                                     {return a[0] < b[0];});

     const auto [min_y, max_y] = std::minmax_element(std::cbegin(odgovarajuci_poligon.tacke()),
                                                     std::cend(odgovarajuci_poligon.tacke()),
                                                     [](const auto& a,
                                                        const auto& b)
                                                     {return a[1] < b[1];});

     const auto mini_x = (*min_x)[0];
     const auto maxi_x = (*max_x)[0];

     const auto mini_y = (*min_y)[1];
     const auto maxi_y = (*max_y)[1];

     path.addRect(mini_x, mini_y, (maxi_x - mini_x), (maxi_y - mini_y));
     return path;
}



