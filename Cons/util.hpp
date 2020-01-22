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
          typename MatNeConst = typename std::decay<Matrica>::type>
MatNeConst mult(Matrica&& a, Matrica&& b)
{
    // Odredjivanje tipa elemenata matrice;
    // nama je podrazumevano double, ali lepo
    // je da funkcija bude sto generickija;
    // dakle, uzima se tip prvog elementa
    // i skidaju mu se referenca i const
    using Element = typename std::decay<decltype(a[0][0])>::type;

    // Isto tako odredjivanje velicine
    const auto vel = std::size(a);

    // Nula matrica: vektor vel vektora
    // velicine vel ispunjenih nulama
    auto rez = std::vector<std::vector<Element>>
               (vel, std::vector<Element>(vel, 0));

    // Klasicno mnozenje u trostrukoj petlji; lepse
    // bi izgledalo kada bi bio list comprehension,
    // kao sto je to elegantno moguce u Python-u
    using Velicina = typename std::vector<Element>::size_type;
    for (Velicina i = 0; i < vel; i++){
        for (Velicina j = 0; j < vel; j++){
            for (Velicina k = 0; k < vel; k++){
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
          typename MatNeConst = typename std::decay<Matrica>::type>
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
          typename DesnoNeConst = typename std::decay<Desno>::type,
          typename BinOp>
DesnoNeConst primeni(Levo&& a, Desno&& b, BinOp&& operacija)
{
    // Odredjivanje tipa elemenata tacke;
    // nama je podrazumevano double, ali lepo
    // je da funkcija bude sto generickija;
    // dakle, uzima se tip prvog elementa
    // i skidaju mu se referenca i const
    using Element = typename std::decay<decltype(b[0])>::type;

    // Isto tako odredjivanje velicine
    const auto vel = std::size(b);

    // Inicijalizacija rezultata;
    // on je vektor polazne tacke
    auto rez = std::vector<Element>(vel, 1);

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

// Sablonska funkcija za apstrakciju odnosno
// 'razumevanje' listi (list comprehension)
template<typename Niz, typename P1, typename P2,
         typename NizNeConst = typename std::decay<Niz>::type,
         typename Element = typename NizNeConst::value_type>
NizNeConst listcomp(Niz&& izv, P1&& pred, P2&& trans)
{
    // Inicijalizacija rezultata
    // prostim kopiranjem izvora
    NizNeConst rez = std::forward<Niz>(izv);

    // Filtriranje u mestu koriscenjem
    // poznatog erase-remove idioma
    rez.erase(std::remove_if(std::begin(rez),
                             std::end(rez),
                             std::not_fn(std::forward<P1>(pred))),
              std::end(rez));

    // Primena transformacije na svaki element
    std::for_each(std::begin(rez),
                  std::end(rez),
                  std::forward<P2>(trans));

    // Vracanje rezultata; eksplicitno pomeranje
    // kako bi se isforsirala optimizacija (RVO)
    return std::move(rez);
}

// Gornja fja bez transformacije
template<typename Niz, typename P,
         typename NizNeConst = typename std::decay<Niz>::type>
NizNeConst listcomp(Niz&& izv, P&& pred)
{
    return listcomp(std::forward<Niz>(izv),
                    std::forward<P>(pred),
                    [](auto&){});
}

// Vracanje tacaka sa leve strane vektora; nesto tipa
// return [t for t in tacke if vekt_proiz(t, u, v) < 0]
const auto vekt_proiz = [](const auto& t, const auto& u, const auto&v)
                          {
                              const auto a = std::make_tuple(t[0]-u[0], t[1]-u[1]);
                              const auto b = std::make_tuple(v[0]-u[0], v[1]-u[1]);

                              // Vracanje dela vektorskog proizvoda
                              return std::get<0>(a)*std::get<1>(b) -
                                     std::get<1>(a)*std::get<0>(b);
                          };

// Podela prostora prema polozaju u odnosu na vektor
const auto podela = [](const auto& u, const auto& v, const auto& tacke)
                      {return listcomp(tacke, [&u, &v](const auto& t)
                                                      {return vekt_proiz(t, u, v) < 0;});};

// Lambda dohvatac indeksa
const auto itemgetter = [](const auto& i)
                          {return [&i](const auto niz){return niz[i];};};
const auto get0 = itemgetter(0);
const auto get0comp = [](const auto& a, const auto& b)
                        {return get0(a) < get0(b);};

// Prosirivanje pretrage omotnih tacaka
template <typename Niz,
          typename NizNeConst = typename std::decay<Niz>::type,
          typename Tacka = typename NizNeConst::value_type>
NizNeConst prosiri(Tacka& u, Tacka& v, Niz&& tacke)
{
    // Nema prosirivanja praznog niza
    if (std::forward<Niz>(tacke).empty()){
        return {};
    }

    // Nalazenje najudaljenije tacke
    const auto vpk = [&u, &v](const auto& t)
                             {return vekt_proiz(t, u, v);};
    const auto key = [&vpk](const auto& a, const auto& b)
                           {return vpk(a) < vpk(b);};

    const auto w = *std::min_element(std::cbegin(std::forward<Niz>(tacke)),
                                     std::cend(std::forward<Niz>(tacke)),
                                     key);

    // Podela pretrage po odredjenoj tacki
    const auto t1 = podela(w, v, std::forward<Niz>(tacke));
    const auto t2 = podela(u, w, std::forward<Niz>(tacke));

    // Prosirivanje pretrage
    const auto p1 = prosiri(w, v, t1);
    const auto p2 = prosiri(u, w, t2);

    // Formiranje rezultata
    NizNeConst rez;
    rez.insert(std::end(rez),
               std::cbegin(p1),
               std::cend(p1));
    rez.push_back(w);
    rez.insert(std::end(rez),
               std::cbegin(p2),
               std::cend(p2));

    // Vracanje rezultata; eksplicitno pomeranje
    // kako bi se isforsirala optimizacija (RVO)
    return std::move(rez);
}

// Sablonska fja za pronalazak konveksnog omotaca;
// jednostavnosti radi ogranicena na vektor tacaka
template <typename Niz,
          typename NizNeConst = typename std::decay<Niz>::type,
          typename Tacka = typename NizNeConst::value_type>
NizNeConst konv_omot(Niz&& tacke)
{
    // Prazan niz tacaka nema konveksni omot
    if (std::forward<Niz>(tacke).empty()){
        return {};
    }

    // Nalazenje dve tacke omota
    const auto u = *std::min_element(std::cbegin(std::forward<Niz>(tacke)),
                                     std::cend(std::forward<Niz>(tacke)),
                                     get0comp);
    const auto v = *std::max_element(std::cbegin(std::forward<Niz>(tacke)),
                                     std::cend(std::forward<Niz>(tacke)),
                                     get0comp);

    // Podela pretrage na levu i desnu stranu
    const auto levo = podela(u, v, std::forward<Niz>(tacke));
    const auto desno = podela(v, u, std::forward<Niz>(tacke));

    // Nalazenje omota na obe strane
    const auto plevo = prosiri(u, v, levo);
    const auto pdesno = prosiri(v, u, desno);

    // Formiranje rezultata
    NizNeConst rez;
    rez.push_back(v);
    rez.insert(std::end(rez),
               std::cbegin(plevo),
               std::cend(plevo));
    rez.push_back(u);
    rez.insert(std::end(rez),
               std::cbegin(pdesno),
               std::cend(pdesno));

    // Popravka u slucaju dve tacke
    if (rez.size() == 2 && rez[0] == rez[1]){
        rez.pop_back();
    }

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
