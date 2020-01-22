#ifndef GEOM_HPP
#define GEOM_HPP

#include <iostream>
#include <vector>

#include "util.hpp"

// Imenski prostor za geometriju; namenjen je
// za implementaciju klasa i povezanih metoda
namespace geom
{

/////////////////////////////////////////////
// GEOM /////////////////////////////////////
/////////////////////////////////////////////

// Izdvajanje imena tipova,
// zapravo njihovih alijasa
using Matrica = std::vector<std::vector<double>>;
using Vektor = Matrica::value_type;
using Element = Vektor::value_type;
using Velicina = Matrica::size_type;

// Poseban izuzetak
class Exc : std::exception
{
public:
    // Eksplicitni konstruktori sa porukom,
    // sto znaci da se ne moze konstruisati
    // izuzetak osim prilikom ispaljivanja
    explicit Exc(const char*);
    explicit Exc(const std::string&);

    // Uobicajeni dohvatac poruke,
    // prevazidjen iz bazne klase
    const char* what() const noexcept override;

protected:
    // Poruka o izuzetku
    std::string poruka;
};

// Preliminarno deklarisanje klase za tacku,
// kako bi prevodilac znao za nju i pre pune
// deklaracije, date u nastavku zaglavlja
class tacka;

// Afino preslikavanje ravni
class geom
{
public:
    // Konstruktor bez argumenata
    geom();

    // Destruktor; ne radi nista jer
    // klasa nema dinamickih objekata
    virtual ~geom() = default;

    // Konstruktor kopije; uzima se podrazumevani
    // sablon zasnovan na idiomu zamene (copy-swap)
    geom(const geom&) = default;

    // Konstruktor kopije sa semantikom pomeranja;
    // uzima se podrazumevani sablon pomeranja (move)
    geom(geom&&) noexcept = default;

    // Konstruktori od vektora
    geom(const Matrica&);
    geom(Matrica&&);
    geom(const Vektor&, const Vektor&, const Vektor&);
    geom(Vektor&&, Vektor&&, Vektor&&);
    geom(const Vektor&, const Vektor&);
    geom(Vektor&&, Vektor&&);

    // Dohvatac za matricu
    const Matrica& mat() const;

    // Dohvatac za velicinu
    Velicina size() const;

    // Dohvatac za toleranciju
    Element tol() const;

    // Kolekcijski metodi
    auto begin() const noexcept;
    auto cbegin() const noexcept;
    auto end() const noexcept;
    auto cend() const noexcept;

    // Inverz preslikavanja
    geom inv(const bool = false);

    // Stepen preslikavanja
    geom pow(const int, const bool = false);

    // Centriranje transformacije
    geom pomeri(const double, const double, const bool = false);
    geom pomeri(tacka&, const bool = false);
    geom pomeri(Vektor&, const bool = false);
    geom pomeri(Vektor&&, const bool = false);

    // Pretvaranje preslikavanja u nisku
    std::string str() const;

    // Operator mnozenja
    geom operator*(const geom&) const;

    // Mnozenje numerickom vrednoscu zdesna
    geom operator*(const double) const;

    // Operator mnozenja sa dodelom
    geom& operator*=(const geom&);

    // Mnozenje numerickom vrednoscu zdesna sa dodelom
    geom& operator*=(const double);

    // Operator stepenovanja
    geom operator^(const int) const;

    // Operator stepenovanja sa dodelom
    geom& operator^=(const int);

    // Operator jednakosti
    bool operator==(const geom&) const;

    // Operator nejednakosti
    bool operator!=(const geom&) const;

    // Operator dodele; podrazumevani
    // sablon idioma zamene (copy-swap)
    geom& operator=(const geom&) = default;

    // Operator dodele sa semantikom pomeranja;
    // podrazumevani sablon pomeranja (move)
    geom& operator=(geom&&) noexcept = default;

    // Operatori dodele vektora
    geom& operator=(const Matrica&);
    geom& operator=(Matrica&&);

    // Operator indeksiranja za dohvatanje; eksplicitno
    // oznacen da se ne sme odbaciti, kako bi se dodatno
    // istaklo da je iskljucivo za dohvatanje, ne izmenu
    [[nodiscard]]
    const Vektor& operator[](const Velicina) const;

    // Struktura koja sluzi za citanje
    // toka razdvojenog zapetama
    struct Citac{
        // Objekat citaca
        geom& g;

        // Indeks citaca
        Velicina i;

        // Konstruktor
        Citac(geom&, Velicina);

        // Zapeta za upis
        Citac operator,(const Element) const;
    };

    // Pocetni upis
    Citac operator<<(const Element);

protected:
    // Transformacija ravni je predstavljena
    // homogenom 3x3 matricom, a podrazumevano
    // je u pitanju jedinicna transformacija;
    // nije const zbog pomeranja (move)
    Matrica _mat;

    // Matrica preslikavanja ravni fiksno
    // je velicine 3, ali slicno nije const
    Velicina _size = 3;

    // Tolerancija u slucaju greske u racunu;
    // nije const iz istog razloga kao dosad
    Element _tol = 1e-5;

private:
    // Provera korektnosti preslikavanja
    void proveri();

    // Operator citanja sa ulaznog toka; mora
    // prijateljski kako bi pristupao matrici
    friend std::istream& operator>>(std::istream&, geom&);
};

// Staticka verzija fje za inverz
geom inv(geom&, const bool = false);

// Staticko centriranje transformacije
geom pomeri(geom&, const double, const double, const bool = false);
geom pomeri(geom&, tacka&, const bool = false);
geom pomeri(geom&, Vektor&, const bool = false);
geom pomeri(geom&, Vektor&&, const bool = false);

// Operator ispisa na izlazni tok
std::ostream& operator<<(std::ostream&, const geom&);

// Operator citanja sa ulaznog toka
std::istream& operator>>(std::istream&, geom&);

// Mnozenje numerickom vrednoscu sleva
geom operator*(const double, const geom&);

/////////////////////////////////////////////
// !GEOM ////////////////////////////////////
/////////////////////////////////////////////

/////////////////////////////////////////////
// TACKA ////////////////////////////////////
/////////////////////////////////////////////

// Tacka afine ravni; dekorisana u smislu
// da implementira potpuno uredjenje
class tacka : public util::total_order<tacka>
{
public:
    // Konstruktor bez argumenata
    tacka();

    // Destruktor; ne radi nista jer
    // klasa nema dinamickih objekata
    ~tacka() = default;

    // Konstruktor kopije; uzima se podrazumevani
    // sablon zasnovan na idiomu zamene (copy-swap)
    tacka(const tacka&) = default;

    // Konstruktor kopije sa semantikom pomeranja;
    // uzima se podrazumevani sablon pomeranja (move)
    tacka(tacka&&) noexcept = default;

    // Konstruktori od brojeva
    tacka(Element, Element);
    tacka(Element, Element, Element);

    // Konstruktori od vektora
    tacka(const Matrica&);
    tacka(Matrica&&);
    tacka(const Vektor&);
    tacka(Vektor&&);

    // Dohvatac za vektor
    const Vektor& mat() const;

    // Dohvatac za velicinu
    Velicina size() const;

    // Dohvatac za toleranciju
    Element tol() const;

    // Kolekcijski metodi
    auto begin() const noexcept;
    auto cbegin() const noexcept;
    auto end() const noexcept;
    auto cend() const noexcept;

    // Pretvaranje tacke u nisku
    std::string str() const;

    // Primena transformacije na tacku
    void primeni(const geom&);

    // Unarna potvrda
    tacka operator+() const;

    // Operator sabiranja
    tacka operator+(const tacka&) const;

    // Sabiranje sa numerickom vrednoscu
    tacka operator+(const double) const;

    // Operator sabiranja sa dodelom
    tacka& operator+=(const tacka&);

    // Sabiranje sa numerickom vrednoscu sa dodelom
    tacka& operator+=(const double);

    // Unarna negacija
    tacka operator-() const;

    // Operator oduzimanja
    tacka operator-(const tacka&) const;

    // Oduzimanje numericke vrednosti
    tacka operator-(const double) const;

    // Operator oduzimanja sa dodelom
    tacka& operator-=(const tacka&);

    // Oduzimanje numericke vrednosti sa dodelom
    tacka& operator-=(const double);

    // Mnozenje numerickom vrednoscu
    tacka operator*(const double) const;

    // Mnozenje numerickom vrednoscu sa dodelom
    tacka& operator*=(const double);

    // Deljenje numerickom vrednoscu
    tacka operator/(const double) const;

    // Deljenje numerickom vrednoscu sa dodelom
    tacka& operator/=(const double);

    // Operator jednakosti
    bool operator==(const tacka&) const;

    // Operator poretka
    bool operator<(const tacka&) const;

    // Operator dodele; podrazumevani
    // sablon idioma zamene (copy-swap)
    tacka& operator=(const tacka&) = default;

    // Operator dodele sa semantikom pomeranja;
    // podrazumevani sablon pomeranja (move)
    tacka& operator=(tacka&&) noexcept = default;

    // Operatori dodele vektora
    tacka& operator=(const Matrica&);
    tacka& operator=(Matrica&&);
    tacka& operator=(const Vektor&);
    tacka& operator=(Vektor&&);

    // Operator indeksiranja za dohvatanje; eksplicitno
    // oznacen da se ne sme odbaciti, kako bi se dodatno
    // istaklo da je iskljucivo za dohvatanje, ne izmenu
    [[nodiscard]]
    const Element& operator[](const Velicina) const;

    // Struktura koja sluzi za citanje
    // toka razdvojenog zapetama
    struct Citac{
        // Objekat citaca
        tacka& t;

        // Indeks citaca
        Velicina i;

        // Konstruktor
        Citac(tacka&, Velicina);

        // Zapeta za upis
        Citac operator,(const Element) const;
    };

    // Pocetni upis
    Citac operator<<(const Element);

private:
    // Provera korektnosti tacke
    void proveri();

    // Provera korektnosti matrice
    void proverim(const Matrica&);

    // Tacka u ravni predstavljena je homogenim
    // vektorom duzine tri, a podrazumevano je
    // u pitanju koordinatni pocetak; nije const
    // zbog mogucnosti pomeranja (move)
    Vektor _mat;

    // Vektor je fiksne velicine,
    // ali slicno nije const
    Velicina _size = 3;

    // Tolerancija u slucaju greske u racunu;
    // nije const iz istog razloga kao dosad
    Element _tol = 1e-5;

    // Operator citanja sa ulaznog toka; mora
    // prijateljski kako bi pristupao vektoru
    friend std::istream& operator>>(std::istream&, tacka&);
};

// Operator ispisa na izlazni tok
std::ostream& operator<<(std::ostream&, const tacka&);

// Operator citanja sa ulaznog toka
std::istream& operator>>(std::istream&, tacka&);

// Operator za mnozenje matrice i tacke
tacka operator*(const geom&, const tacka&);

// Operator za sabiranje sa numerickom vrednoscu
tacka operator+(const double, const tacka&);

// Operator za oduzimanje numericke vrednosti
tacka operator-(const double, const tacka&);

// Operator za mnozenje numerickom vrednoscu
tacka operator*(const double, const tacka&);

// Operator za deljenje numerickom vrednoscu
tacka operator/(const double, const tacka&);

/////////////////////////////////////////////
// !TACKA ///////////////////////////////////
/////////////////////////////////////////////

/////////////////////////////////////////////
// AFIN /////////////////////////////////////
/////////////////////////////////////////////

// Translacija u ravni
class trans : public geom
{
public:
    // Konsktruktori sa koordinatama
    trans(double, double, const bool = false);
    trans(const tacka&, const bool = false);
};

// Skaliranje u ravni
class skal : public geom
{
public:
    // Konsktruktori sa koordinatama
    skal(double, double, const double, const double, const bool = false);
    skal(double, double, const tacka& = {0, 0}, const bool = false);
    skal(const tacka&, const double, const double, const bool = false);
    skal(const tacka&, const tacka& = {0, 0}, const bool = false);
};

// Smicanje u ravni
class smic : public geom
{
public:
    // Konsktruktori sa koordinatama
    smic(double, double, const double, const double, const bool = false);
    smic(double, double, const tacka& = {0, 0}, const bool = false);
    smic(const tacka&, const double, const double, const bool = false);
    smic(const tacka&, const tacka& = {0, 0}, const bool = false);
};

// Rotacija u ravni
class rot : public geom
{
public:
    // Konstruktori sa uglom
    rot(double, const double, const double, const bool = false);
    rot(double, const tacka& = {0, 0}, const bool = false);
};

// Refleksija u ravni
class refl : public geom
{
public:
    // Konstruktori sa uglom
    refl(double, const double, const double, const bool = false);
    refl(double, const tacka& = {0, 0}, const bool = false);
};

/////////////////////////////////////////////
// !AFIN ////////////////////////////////////
/////////////////////////////////////////////

/////////////////////////////////////////////
// OBLIK ////////////////////////////////////
/////////////////////////////////////////////

// Alijas za kolekciju tacaka
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
    NizTacaka _tacke{};
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
    ppoly(const tacka& = {0, 0}, const Velicina = 3, const Element = 3);

};

// Elipsa
class elipsa : public oblik
{
public:
    // Konstruktor sa centrom i poluprecnicima
    elipsa(const tacka& = {0, 0}, const Element = 3, const Element = 6);

    // Konstruktor sa trima glavnim tackama
    elipsa(const tacka&, const tacka&, const tacka&);

};

// Krug
class krug : public oblik
{
public:
    // Konstruktor sa centrom i poluprecnikom
    krug(const tacka& = {0, 0}, const Element = 5);

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

/////////////////////////////////////////////
// !OBLIK ///////////////////////////////////
/////////////////////////////////////////////

}

#endif // GEOM_HPP
