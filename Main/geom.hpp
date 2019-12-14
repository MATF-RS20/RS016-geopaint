#ifndef GEOM_HPP
#define GEOM_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

// Imenski prostor za geometriju; namenjen je
// za implementaciju klasa i povezanih metoda
namespace geom
{

// Izdvajanje imena tipova,
// zapravo njihovih alijasa
using Tip = std::vector<std::vector<double>>;
using Iter = Tip::const_iterator;
using PodTip = Tip::value_type;
using PodIter = PodTip::const_iterator;
using Elem = PodTip::value_type;
using Vel = Tip::size_type;

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

// Afino preslikavanje ravni
class geom
{
public:
    // Konstruktor bez argumenata; ne
    // radi nista jer je podrazumevana
    // vrednost postavljenja u polju
    geom() = default;

    // Destruktor; ne radi nista jer
    // klasa nema dinamickih objekata
    ~geom() = default;

    // Konstruktor kopije; uzima se podrazumevani
    // sablon zasnovan na idiomu zamene (copy-swap)
    geom(const geom&) = default;

    // Konstruktor kopije sa semantikom pomeranja;
    // uzima se podrazumevani sablon pomeranja (move)
    geom(geom&&) noexcept = default;

    // Konstruktori od vektora
    geom(const Tip&);
    geom(Tip&&);
    geom(const PodTip&, const PodTip&, const PodTip&);
    geom(PodTip&&, PodTip&&, PodTip&&);

    // Dohvatac za matricu
    const Tip& mat() const;

    // Dohvatac za velicinu
    Vel size() const;

    // Kolekcijski metodi
    Iter begin() const noexcept;
    Iter cbegin() const noexcept;
    Iter end() const noexcept;
    Iter cend() const noexcept;

    // Inverz preslikavanja
    geom inv(const bool = false);

    // Stepen preslikavanja
    geom pow(const int, const bool = false);

    // Pretvaranje preslikavanja u nisku
    std::string str() const;

    // Operator mnozenja
    geom operator*(const geom&) const;

    // Mnozenje numerickom vrednoscu zdesna
    template <typename Num,
              std::enable_if_t<std::is_arithmetic<Num>::value, Num> = 0>
    geom operator*(Num&& broj) const
    {
        return std::forward<Num>(broj) * *this;
    }

    // Operator mnozenja sa dodelom
    geom& operator*=(const geom&);

    // Mnozenje numerickom vrednoscu zdesna sa dodelom
    template <typename Num,
              std::enable_if_t<std::is_arithmetic<Num>::value, Num> = 0>
    geom& operator*=(Num&& broj)
    {
        return *this = std::forward<Num>(broj) * *this;
    }

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
    geom& operator=(const Tip&);
    geom& operator=(Tip&&);

    // Operator indeksiranja za dohvatanje; eksplicitno
    // oznacen da se ne sme odbaciti, kako bi se dodatno
    // istaklo da je iskljucivo za dohvatanje, ne izmenu
    [[nodiscard]]
    const PodTip& operator[] (const Vel) const;

    // Struktura koja sluzi za citanje
    // toka razdvojenog zapetama
    struct Citac{
        // Objekat citaca
        geom& g;

        // Indeks citaca
        Vel i;

        // Konstruktor
        Citac(geom&, Vel);

        // Zapeta za upis
        Citac operator,(const Elem) const;
    };

    // Pocetni upis
    Citac operator<<(const Elem);

private:
    // Provera korektnosti preslikavanja
    void proveri() const;

    // Transformacija ravni je predstavljena
    // homogenom 3x3 matricom, a podrazumevano
    // je u pitanju jedinicna transformacija;
    // nije const zbog pomeranja (move)
    Tip _mat{{1, 0, 0},
             {0, 1, 0},
             {0, 0, 1}};

    // Matrica preslikavanja ravni fiksno
    // je velicine 3, ali slicno nije const
    Vel _size = 3;

    // Tolerancija u slucaju greske u racunu;
    // nije const iz istog razloga kao dosad
    Elem _tol = 1e-5;

    // Operator citanja sa ulaznog toka; mora
    // prijateljski kako bi pristupao matrici
    friend std::istream& operator>>(std::istream&, geom&);
};

// Staticka verzija fje za inverz
geom inv(geom&, const bool = false);

// Operator ispisa na izlazni tok
std::ostream& operator<<(std::ostream&, const geom&);

// Operator citanja sa ulaznog toka
std::istream& operator>>(std::istream&, geom&);

// Operator za mnozenje numerickom vrednoscu sleva;
// ukljucen je (enable_if_t) samo za aritmeticke
// vrednost (is_arithmetic): integralne i realne
template <typename Num,
          std::enable_if_t<std::is_arithmetic<Num>::value, Num> = 0>
geom operator*(Num&& a, const geom& b)
{
    // Inicijalizacija rezultata;
    // on je matrica transformacije
    auto rez = static_cast<Tip>(b.mat());

    // Mnozenje prvih redova
    for (Vel i = 0; i < std::size(rez)-1; i++){
        std::transform(std::cbegin(rez[i]),
                       std::cend(rez[i]),
                       // Red = rezultat
                       std::begin(rez[i]),
                       // Mnozenje sleva
                       std::bind(std::multiplies<>(),
                                 std::forward<Num>(a),
                                 std::placeholders::_1));
    }

    // Vracanje rezultata; eksplicitno pomeranje
    // kako bi se isforsirala optimizacija (RVO)
    return std::move(rez);
}

}

#endif // GEOM_HPP
