#ifndef TACKA_HPP
#define TACKA_HPP

#include "geom.hpp"
#include "util.hpp"

// Imenski prostor za geometriju
namespace geom
{

// Tacka afine ravni; dekorisana u smislu
// da implementira potpuno uredjenje
class tacka : public util::total_order<tacka>
{
public:
    // Konstruktor bez argumenata; ne
    // radi nista jer je podrazumevana
    // vrednost postavljenja u polju
    tacka() = default;

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
    tacka(Elem, Elem);
    tacka(Elem, Elem, Elem);

    // Konstruktori od vektora
    tacka(const Tip&);
    tacka(Tip&&);
    tacka(const PodTip&);
    tacka(PodTip&&);

    // Dohvatac za vektor
    const PodTip& mat() const;

    // Dohvatac za velicinu
    Vel size() const;

    // Kolekcijski metodi
    PodIter begin() const noexcept;
    PodIter cbegin() const noexcept;
    PodIter end() const noexcept;
    PodIter cend() const noexcept;

    // Pretvaranje tacke u nisku
    std::string str() const;

    // Unarna potvrda
    tacka operator+() const;

    // Operator sabiranja
    tacka operator+(const tacka&) const;

    // Sabiranje sa numerickom vrednoscu zdesna
    template <typename Num,
              std::enable_if_t<std::is_arithmetic<Num>::value, Num> = 0>
    tacka operator+(Num&& broj) const
    {
        return std::forward<Num>(broj) + *this;
    }

    // Operator sabiranja sa dodelom
    tacka& operator+=(const tacka&);

    // Sabiranje sa numerickom vrednoscu zdesna sa dodelom
    template <typename Num,
              std::enable_if_t<std::is_arithmetic<Num>::value, Num> = 0>
    tacka& operator+=(Num&& broj)
    {
        return *this = std::forward<Num>(broj) + *this;
    }

    // Unarna negacija
    tacka operator-() const;

    // Operator oduzimanja
    tacka operator-(const tacka&) const;

    // Oduzimanje numericke vrednosti
    template <typename Num,
              std::enable_if_t<std::is_arithmetic<Num>::value, Num> = 0>
    tacka operator-(Num&& broj) const
    {
        return -std::forward<Num>(broj) + *this;
    }

    // Operator oduzimanja sa dodelom
    tacka& operator-=(const tacka&);

    // Oduzimanje numericke vrednosti sa dodelom
    template <typename Num,
              std::enable_if_t<std::is_arithmetic<Num>::value, Num> = 0>
    tacka& operator-=(Num&& broj)
    {
        return *this = -std::forward<Num>(broj) + *this;
    }

    // Mnozenje numerickom vrednoscu zdesna
    template <typename Num,
              std::enable_if_t<std::is_arithmetic<Num>::value, Num> = 0>
    tacka operator*(Num&& broj) const
    {
        return std::forward<Num>(broj) * *this;
    }

    // Mnozenje numerickom vrednoscu zdesna sa dodelom
    template <typename Num,
              std::enable_if_t<std::is_arithmetic<Num>::value, Num> = 0>
    tacka& operator*=(Num&& broj)
    {
        return *this = std::forward<Num>(broj) * *this;
    }

    // Deljenje numerickom vrednoscu
    template <typename Num,
              std::enable_if_t<std::is_arithmetic<Num>::value, Num> = 0>
    tacka operator/(Num&& broj) const
    {
        return 1/std::forward<Num>(broj) * *this;
    }

    // Deljenje numerickom vrednoscu sa dodelom
    template <typename Num,
              std::enable_if_t<std::is_arithmetic<Num>::value, Num> = 0>
    tacka& operator/=(Num&& broj)
    {
        return *this = 1/std::forward<Num>(broj) * *this;
    }

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
    tacka& operator=(const Tip&);
    tacka& operator=(Tip&&);
    tacka& operator=(const PodTip&);
    tacka& operator=(PodTip&&);

    // Operator indeksiranja za dohvatanje; eksplicitno
    // oznacen da se ne sme odbaciti, kako bi se dodatno
    // istaklo da je iskljucivo za dohvatanje, ne izmenu
    [[nodiscard]]
    const Elem& operator[] (const Vel) const;

    // Struktura koja sluzi za citanje
    // toka razdvojenog zapetama
    struct Citac{
        // Objekat citaca
        tacka& t;

        // Indeks citaca
        Vel i;

        // Konstruktor
        Citac(tacka&, Vel);

        // Zapeta za upis
        Citac operator,(const Elem) const;
    };

    // Pocetni upis
    Citac operator<<(const Elem);

private:
    // Provera korektnosti tacke
    void proveri();

    // Tacka u ravni predstavljena je homogenim
    // vektorom duzine tri, a podrazumevano je
    // u pitanju koordinatni pocetak; nije const
    // zbog mogucnosti pomeranja (move)
    PodTip _mat{0, 0, 1};

    // Vektor je fiksne velicine,
    // ali slicno nije const
    Vel _size = 3;

    // Tolerancija u slucaju greske u racunu;
    // nije const iz istog razloga kao dosad
    Elem _tol = 1e-5;

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

// Operator za sabiranje sa numerickom vrednoscu;
// ukljucen je (enable_if_t) samo za aritmeticke
// vrednost (is_arithmetic): integralne i realne
template <typename Num,
          std::enable_if_t<std::is_arithmetic<Num>::value, Num> = 0>
tacka operator+(Num&& a, const tacka& b)
{
    return std::move(util::primeni(std::forward<Num>(a),
                                   b,
                                   std::plus<>()));
}

// Operator za oduzimanje numericke vrednosti
template <typename Num,
          std::enable_if_t<std::is_arithmetic<Num>::value, Num> = 0>
tacka operator-(Num&& a, const tacka& b)
{
    return std::move(util::primeni(std::forward<Num>(a),
                                   -b,
                                   std::plus<>()));
}

// Operator za mnozenje numerickom vrednoscu
template <typename Num,
          std::enable_if_t<std::is_arithmetic<Num>::value, Num> = 0>
tacka operator*(Num&& a, const tacka& b)
{
    return std::move(util::primeni(std::forward<Num>(a),
                                   b,
                                   std::multiplies<>()));
}

// Operator za deljenje numerickom vrednoscu
template <typename Num,
          std::enable_if_t<std::is_arithmetic<Num>::value, Num> = 0>
tacka operator/(Num&& a, const tacka& b)
{
    return std::move(util::primeni(std::forward<Num>(a),
                                   tacka(1/b[0], 1/b[1], 1),
                                   std::multiplies<>()));
}

// Provera korektnosti matrice
void proverim(const Tip&);

}

#endif // TACKA_HPP
