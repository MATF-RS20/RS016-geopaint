#ifndef OBLIK_HPP
#define OBLIK_HPP

#include <vector>

// Imenski prostor za geometriju
namespace geom {

// Neophodne obacavajuce deklaracije
class geom;
class tacka;

// Alijas za kolekciju tacaka
using Matrica = std::vector<std::vector<double>>;
using Vektor = Matrica::value_type;
using Element = Vektor::value_type;
using Velicina = Matrica::size_type;
using NizTacaka = std::vector<tacka>;

// Maksimalno svedena reprezentacija oblika
// koje je moguce nacrtati na platnu; mogli
// bi se implementirati razni metodi po ugledu
// na prethodne klase, ali to nije neophodno,
// pa je stoga i izostavljeno u radu
class oblik
{
public:
    // Podrazumevani virtuelni destruktor
    virtual ~oblik() = default;

    // Dohvatac za tacke
    const NizTacaka& tacke() const;

    // Transformacija oblika
    void transformisi(const geom&);

protected:
    // Konstruktori od vektora
    oblik(const NizTacaka&);
    oblik(NizTacaka&& = {});

    // Uredjeni niz tacaka oblika
    NizTacaka _tacke;
};

// Mnogougao ili poligonska linija
class poly : public oblik
{
public:
    // Konstruktori od vektora
    poly(const NizTacaka&);
    poly(NizTacaka&& = {});

    // Konstruktor od tacaka
    poly(const std::initializer_list<tacka>);

    // Dohvatac za zatvorenost
    bool zatvoren() const;

    // Zatvaranje i otvaranje
    void zatvori();
    void otvori();

    // Dodavanje tacke
    void dodaj(const tacka&);
    void dodaj(tacka&&);

    // Zamena konveksnim omotacem
    void omot();

private:
    // Indikator zatvorenosti
    bool _zatvoren = false;
};

// Pravilan mnogougao
class ppoly : public oblik
{
public:
    // Konstruktor sa centrom i velicinama
    ppoly(const tacka&, const Velicina = 3, const Element = 3);

};

// Elipsa
class elipsa : public oblik
{
public:
    // Konstruktor sa centrom i poluprecnicima
    elipsa(const tacka&, const Element = 3, const Element = 6);

    // Konstruktor sa trima glavnim tackama
    elipsa(const tacka&, const tacka&, const tacka&);

};

// Krug
class krug : public oblik
{
public:
    // Konstruktor sa centrom i poluprecnikom
    krug(const tacka&, const Element = 5);

    // Konstruktor sa dvema glavnim tackama
    krug(const tacka&, const tacka&);

};

/***** OVO SAMO AKO BAS BUDE VREMENA ******************
// Slika kao oblik
class slika : public oblik
{
public:
    // Konstruktor sa putanjom i centrom
    slika(const std::string&, const tacka& = {0, 0});

    // Dohvatac za putanju
    std::string putanja() const;

private:
    // Putanja (tj. QPixmap ili sta vec)
    std::string _putanja;
};

// Tekstualni oblik
class tekst : public oblik
{
public:
    // Konstruktor sa tekstom i centrom
    tekst(const std::string& = "", const tacka& = {0, 0});

    // Dohvatac za sadrzaj
    std::string sadrzaj() const;

private:
    // Sadrzaj teksta
    std::string _sadrzaj;
};
*****************************************************/

}

#endif // OBLIK_HPP
