#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <numeric>
#include <functional>
#include <cmath>

// Imenski prostor pomocnih funkcija; namenjem je
// za implementaciju mahom sablonskih funkcija
namespace util
{

// Struktura koja od == i <, prateci princip potpunog
// uredjenja (total ordering), implementira preostale
// relacione operatore: !=, <=, >, >=
template <typename T>
struct total_order
{
    // Razlicitost je negacija jednakosti
    bool operator!=(const T& dr) const
    {
        return !(static_cast<const T&>(*this) == dr);
    }

    // Manje ili jednako se proverava disjunkcijom
    bool operator<=(const T& dr) const
    {
        return static_cast<const T&>(*this) < dr ||
               static_cast<const T&>(*this) == dr;
    }

    // Vece se proverava konjunkcijom negacija
    bool operator>(const T& dr) const
    {
        return !(static_cast<const T&>(*this) < dr) &&
               !(static_cast<const T&>(*this) == dr);
    }

    // Vece ili jednako je negacija manjeg
    bool operator>=(const T& dr) const
    {
        return !(static_cast<const T&>(*this) < dr);
    }
};

// Dve prazne strukture, za koje je
// bitno da su u odnosu nasledjivanja
struct A {};
struct B: A {};

// Deklaracija fje, kako bi bila vidljiva lambdi;
// tolerancija je podrazumevano nullptr jer, na
// primer, ukoliko kolekcija nije numericka, cesto
// ni tolerancija niti razlika nemaju smisla
template <typename Objekat, typename Tolerancija>
bool jednakost(const Objekat&, const Objekat&,
               const Tolerancija& = nullptr);

// Sablonska fja za jednakost dva objekta
// koji su kolekcije iz vise nivoa (vektori,
// vektori vektora ili cak vektori vektora
// vektora vektora...) sablonskih vrednosti
// sa prosledjenom tolerancijom na gresku;
// pozvace se samo ukoliko prodje dobijanje
// tipa elementa kolekcije (value_type)
template <typename Kolekcija, typename Tolerancija,
          typename KolTip = typename Kolekcija::value_type>
inline bool jednakost(const Kolekcija& a,
                      const Kolekcija& b,
                      const Tolerancija& tol,
                      const B* const)
{
    // Kolekcije su jednake ukoliko su jednake
    // duzine i skalarni proizvod im je true
    return std::size(a) == std::size(b) &&
           // Skalarni proiz: pocetak prve kolekcije
           std::inner_product(std::cbegin(a),
                              // Kraj prve kolekcije
                              std::cend(a),
                              // Samo pocetak druge,
                              // posto su iste duzine
                              std::cbegin(b),
                              // Podrazumevano jednake
                              true,
                              // Spoljasnja fja je &&
                              std::logical_and<>(),
                              // Unutrasnja je rekurzivna
                              // nad elementima kolekcije
                              [&tol](const KolTip& c, const KolTip& d)
                                    {return jednakost(c, d, tol);});
}

// Sablonska fja za jednakost dva objekta
// koji nisu kolekcijskog tipa, izlaz iz
// rekurzije prethodnog sablona
template <typename Objekat,
          typename Tolerancija = Objekat>
inline bool jednakost(const Objekat& a,
                      const Objekat& b,
                      const Tolerancija& tol,
                      const A* const)
{
  return std::abs(a-b) < tol;
}

// Sablonska fja poput one iznad, samo u
// slucaju da je prosledjen nullptr
template <typename Objekat>
inline bool jednakost(const Objekat& a,
                      const Objekat& b,
                      // Tolerancija == nullptr
                      const std::nullptr_t,
                      const A* const)
{
  return a == b;
}

// SFINAE trik za odredjivanje tacnog poziva funkcije;
// "substitution failure is not an error" predvidja da
// nema greske ako ne uspe instanciranje najbolje
// odgovarajuceg sablona, vec se instancira naredni
// odgovarajuci; u konkretnom slucaju, fja potencira
// na instanciranju sablona ciji je poslednji argument
// tipa B*, i to uspeva ako su prosledjene kolekcije,
// ali ne i kad nisu; ipak, umesto greske, prevodilac
// instancira sledeci sablon, kom je poslednji argument
// A*, posto je moguce kastovati uz hijerarhiju B -> A
template <typename Objekat, typename Tolerancija>
inline bool jednakost(const Objekat& a,
                      const Objekat& b,
                      const Tolerancija& tol)
{
  return jednakost(a, b, tol, static_cast<const B* const>(nullptr));
}

// Verzija sablona bez tolerancije; ona se zamenjuje
// sa nullptr npr. u slucaju kada kolekcija nije
// numericka, pa tol. i razlika nemaju smisla
template <typename Objekat>
inline bool jednakost(const Objekat& a, const Objekat& b)
{
  return jednakost(a, b, nullptr, static_cast<const B* const>(nullptr));
}

// Sablonska fja za mnozenje ovde bitnih
// objekata poput geom. preslikavanja (matrica);
// kako se matrice savrseno prosledjuju (forward),
// za dalji rad neophodno je iz dedukovanog izvuci
// tip koji nije referentan niti konstantan; radi
// i kada se instancira sa STL vektorima
template <typename Matrica,
          typename MatNeRef = typename std::remove_reference<Matrica>::type,
          typename MatNeConst = typename std::remove_const<MatNeRef>::type>
MatNeConst mult(Matrica&& a, Matrica&& b)
{
    // Odredjivanje tipa elemenata matrice;
    // nama je podrazumevano double, ali lepo
    // je da funkcija bude sto generickija;
    // dakle, uzima se tip prvog elementa
    // i skidaju mu se referenca i const
    using Tip = typename std::remove_const<typename std::remove_reference
                             <decltype(a[0][0])>::type>::type;

    // Isto tako odredjivanje velicine
    const auto vel = std::size(a);

    // Nula matrica: vektor vel vektora
    // velicine vel ispunjenih nulama
    auto rez = std::vector<std::vector<Tip>>
               (vel, std::vector<Tip>(vel, 0));

    // Klasicno mnozenje u trostrukoj petlji; lepse
    // bi izgledalo kada bi bio list comprehension,
    // kao sto je to elegantno moguce u Python-u
    using Vel = typename std::vector<Tip>::size_type;
    for (Vel i = 0; i < vel; i++){
        for (Vel j = 0; j < vel; j++){
            for (Vel k = 0; k < vel; k++){
                rez[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    // Vracanje rezultata; eksplicitno pomeranje
    // kako bi se isforsirala optimizacija (RVO)
    return std::move(rez);
}

// Sablonska fja za stepenovanje ovde bitnih
// objekata poput geom. preslikavanja (matrica);
// ista prica sa savrsenim prosledjivanjem
template <typename Matrica,
          typename MatNeRef = typename std::remove_reference<Matrica>::type,
          typename MatNeConst = typename std::remove_const<MatNeRef>::type>
MatNeConst pow(Matrica&& baza, const int exp)
{
    // Jedinicna transformacija
    MatNeConst rez;

    // Argument stepenovanja
    MatNeConst arg = std::forward<Matrica>(baza);

    // Pozitivan stepen
    auto j = std::abs(exp);

    // Dok se ne iscrpi stepen
    while (j > 0){
        // Ako je neparan, mnozi se
        // rez argumentom stepenovanja
        if (j & 1){
            rez *= arg;
        }

        // Deljenje stepena sa dva
        j >>= 1;

        // Kvadriranje argumenta
        arg *= arg;
    }

    // Vracanje rezultata; inverzija u
    // slucaju negativnog polaznog stepena
    return exp >= 0 ? rez : rez.inv();
}

// Sablonska fja za primenu binarne operacije
// nad ovde bitnim objektima poput tacaka
template <typename Levo,
          typename Desno,
          typename DesnoNeRef = typename std::remove_reference<Desno>::type,
          typename DesnoNeConst = typename std::remove_const<DesnoNeRef>::type,
          typename BinOp>
DesnoNeConst primeni(Levo&& a, Desno&& b, BinOp&& operacija)
{
    // Odredjivanje tipa elemenata tacke;
    // nama je podrazumevano double, ali lepo
    // je da funkcija bude sto generickija;
    // dakle, uzima se tip prvog elementa
    // i skidaju mu se referenca i const
    using Tip = typename std::remove_const<typename std::remove_reference
                                  <decltype(b[0])>::type>::type;

    // Isto tako odredjivanje velicine
    const auto vel = std::size(b);

    // Inicijalizacija rezultata;
    // on je vektor polazne tacke
    auto rez = std::vector<Tip>(vel, 1);

    // Operacija nad prvim elementima
    std::transform(std::cbegin(b),
                   std::cend(b)-1,
                   // Upis u rezultat
                   std::begin(rez),
                   // Operacija sleva
                   std::bind(std::forward<BinOp>(operacija),
                             std::forward<Levo>(a),
                             std::placeholders::_1));

    // Vracanje rezultata; eksplicitno pomeranje
    // kako bi se isforsirala optimizacija (RVO)
    return std::move(rez);
}

// Funkcija za pretvaranje stepena u radijane
inline double deg2rad(double u)
{
    return acos(-1) * u / 180;
}

// Funkcija za pretvaranje radijana u stepene
inline double rad2deg(double u)
{
    return 180 * u / acos(-1);
}

}

#endif // UTIL_HPP
