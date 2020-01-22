#ifndef GEOM_HPP
#define GEOM_HPP

#include <iostream>
#include <vector>

#include "util.hpp"

// Imenski prostor za geometriju
namespace geom
{

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

}

#endif // GEOM_HPP
