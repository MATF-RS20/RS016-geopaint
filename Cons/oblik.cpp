#include "geom.hpp"

// Imenski prostor za geometriju;
// u ovoj datoteci su oblici
namespace geom{

// Dohvatac za tacke
const OTip& oblik::tacke() const
{
    return _tacke;
}

// Transformacija oblika
void oblik::transformisi(const geom& g)
{
    std::for_each(std::begin(_tacke),
                  std::end(_tacke),
                  std::bind(&tacka::primeni,
                            std::placeholders::_1,
                            g));
}

// Konstruktor od l-vektora
oblik::oblik(const OTip& t)
    : _tacke(t)
{}

// Konstruktor od r-vektora
oblik::oblik(OTip&& t)
    : _tacke(std::move(t))
{}

// Konstruktor od l-vektora
poly::poly(const OTip& t)
    : oblik(t)
{}

// Konstruktor od r-vektora
poly::poly(OTip&& t)
    : oblik(std::move(t))
{}

// Konstruktor od tacaka
poly::poly(const std::initializer_list<tacka> ts)
{
    _tacke.assign(ts);
}

// Dohvatac za zatvorenost
bool poly::zatvoren() const
{
    return _zatvoren;
}

// Zatvaranje poligona
void poly::zatvori()
{
    _zatvoren = true;
}

// Otvaranje poligona
void poly::otvori()
{
    _zatvoren = false;
}

// Dodavanje l-tacke
void poly::dodaj(const tacka& t)
{
    _tacke.push_back(t);
}

// Dodavanje r-tacke
void poly::dodaj(tacka&& t)
{
    _tacke.push_back(std::move(t));
}

// Zamena konveksnim omotacem
void poly::omot()
{
    _tacke = util::konv_omot(_tacke);
}

// Konstruktor sa centrom i velicinama
ppoly::ppoly(const tacka& t, const Vel i, const Elem x)
{
    // Konstanta pi kao arkuskosinus od -1
    const auto pi = acos(-1);

    // Racunanje poluprecnika opisanog kruga
    // pravilnog mnogougla na osnovu prosledjene
    // zeljene velicine stranice i broja tacaka
    const auto r = x/(2*sin(pi/i));

    // Ako je neparan broj tacaka, prva
    // je na vrhu opisanog kruga; a inace
    // (paran broj) prva i druga grade
    // najgornji 1/i isecak opisanog kruga
    const auto phi = i%2 ? pi/2 : pi/i;

    // Petlja u kojoj se racunaju sve potrebne tacke
    const auto kraj = phi+2*pi;
    const auto incr = 2*pi/i;
    for (auto i = phi; i < kraj; i += incr){
        _tacke.emplace_back(t[0]+r*cos(i), t[1]+r*sin(i));
    }
}

// Konstruktor sa centrom i poluprecnicima
elipsa::elipsa(const tacka& t, const Elem e1, const Elem e2)
{
    // Dodavanje centra
    _tacke.push_back(t);

    // Dodavanje tacke manje ose
    _tacke.emplace_back(t[0]+e1, t[1]);

    // Dodavanje tacke vece ose
    _tacke.emplace_back(t[0], t[1]+e2);
}

// Konstruktor sa trima glavnim tackama
elipsa::elipsa(const tacka& t1, const tacka& t2, const tacka& t3)
{
    _tacke.push_back(t1);
    _tacke.push_back(t2);
    _tacke.push_back(t3);
}

// Konstruktor sa centrom i poluprecnikom
krug::krug(const tacka& t, const Elem e)
{
    // Dodavanje centra
    _tacke.push_back(t);

    // Dodavanje tacke ose
    _tacke.emplace_back(t[0]+e, t[1]);
}

// Konstruktor sa dvema glavnim tackama
krug::krug(const tacka& t1, const tacka& t2)
{
    _tacke.push_back(t1);
    _tacke.push_back(t2);
}

/***** OVO SAMO AKO BAS BUDE VREMENA ******************
// Konstruktor sa putanjom i centrom
slika::slika(const std::string& put, const tacka& t)
{
    _tacke.push_back(t);
    _putanja = put;
}

// Dohvatac za putanju
std::string slika::putanja() const
{
    return _putanja;
}

// Konstruktor sa tekstom i centrom
tekst::tekst(const std::string& sad, const tacka& t)
{
    _tacke.push_back(t);
    _sadrzaj = sad;
}

// Dohvatac za sadrzaj
std::string tekst::sadrzaj() const
{
    return _sadrzaj;
}
*****************************************************/

}
