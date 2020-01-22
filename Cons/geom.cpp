#include <sstream>
#include <iterator>
#include <iomanip>

#include "geom.hpp"
#include "tacka.hpp"
#include "afin.hpp"

// Imenski prostor za geometriju;
// u ovoj datoteci je osvnovna klasa
// za preslikavanja kao i izuzetak
namespace geom{

// Konstruktor posebnog izuzetka
// od C-ovskog niza karaktera
Exc::Exc(const char* poruka)
    : poruka(poruka)
{}

// Konstruktor posebnog izuzetka
// od C++-ovskog objekta niske
Exc::Exc(const std::string& poruka)
    : poruka(poruka)
{}

// Uobicajeni dohvatac poruke
inline const char* Exc::what() const noexcept
{
    return poruka.c_str();
}

// Konstruktor bez argumenata
geom::geom()
    : _mat({{1, 0, 0},
            {0, 1, 0},
            {0, 0, 1}})
{}

// Konstruktor od vektora kao l-vrednosti
geom::geom(const Matrica& v)
    : _mat(v)
{
    // Provera korektnosti preslikavanja
    proveri();
}

// Konstruktor od vektora kao r-vrednosti
geom::geom(Matrica&& v)
    : _mat(std::move(v))
{
    // Provera korektnosti preslikavanja
    proveri();
}

// Konstruktor od vektorâ kao l-vrednosti;
// ovaj se poziva i u mesanom slucaju, ako su
// dva ili jedan od tri zapravo r-vrednosti
geom::geom(const Vektor& v1,
           const Vektor& v2,
           const Vektor& v3)
    : _mat({v1, v2, v3})
{
    // Provera korektnosti preslikavanja
    proveri();
}

// Konstruktor od vektorâ kao r-vrednosti;
// poziva se iskljucivo ako su sva tri desna
geom::geom(Vektor&& v1, Vektor&& v2, Vektor&& v3)
    : _mat({std::move(v1), std::move(v2), std::move(v3)})
{
    // Provera korektnosti preslikavanja
    proveri();
}

// Konstruktor od dva l-vektora
geom::geom(const Vektor& v1,
           const Vektor& v2)
    : geom(v1, v2, {0, 0, 1})
{}

// Konstruktor od dva d-vektora
geom::geom(Vektor&& v1,
           Vektor&& v2)
    : geom(std::move(v1), std::move(v2), {0, 0, 1})
{}

// Dohvatac (getter) za matricu
const Matrica& geom::mat() const
{
    return _mat;
}

// Dohvatac (getter) za velicinu
Velicina geom::size() const
{
    return _size;
}

// Dohvatac (getter) za toleranciju
Element geom::tol() const
{
    return _tol;
}

// Kolekcijski metod za pocetak matrice
auto geom::begin() const noexcept
{
    return std::begin(_mat);
}

// Kolekcijski metod za const pocetak matrice
auto geom::cbegin() const noexcept
{
    return std::cbegin(_mat);
}

// Kolekcijski metod za kraj matrice
auto geom::end() const noexcept
{
    return std::end(_mat);
}

// Kolekcijski metod za const kraj matrice
auto geom::cend() const noexcept
{
    return std::cend(_mat);
}

// Opsti postupak inverzije afinih transformacijskih
// matrica; u sustini je zasnovan na ideji o adjungovanoj
// matrici, koja se racuna preko kofaktora (determinanti
// manjih 2x2 podmatrica), ali ovde je taj postupak
// skracen i direktno se vraca sracunati rezultat:
//     |a   b   c|                        | e  -b   bf-ce|
// T = |d   e   f|,  inv(T) = 1/(ae-bd) * |-d   a   cd-af|
//     |0   0   1|                        | 0   0     1  |
geom geom::inv(const bool inplace)
{
    // Izvlacenje vrednosti polja matrice
    // dekompozicijom privremene n-torke
    const auto [a, b, c,
                d, e, f] = std::tie(_mat[0][0], _mat[0][1], _mat[0][2],
                                    _mat[1][0], _mat[1][1], _mat[1][2]);

    // Nije moguce izracunati inverz
    // ukoliko je matrica singularna
    if (util::jednakost(a*e, b*d, _tol)){
        throw Exc("Singularna matrica!");
    }

    // Racunanje nekih kofaktora
    const auto k = 1 / (a*e - b*d);
    const auto t1 = b*f - c*e;
    const auto t2 = c*d - a*f;

    // Racunanje rezultata
    geom rez{{k*e, k*-b, k*t1},
             {k*-d, k*a, k*t2},
             {  0,   0,    1 }};

    // Vracanje i izmena
    return inplace ? *this = rez : rez;
}

// Stepen preslikavanja
geom geom::pow(const int i, const bool inplace)
{
    return inplace ? *this = *this^i : *this^i;
}

// Centriranje transformacije prema koordinatama
geom geom::pomeri(const double x, const double y, const bool inplace)
{
    // Odustajanje ako nema promene
    if (util::jednakost(x, 0.0, _tol) &&
        util::jednakost(y, 0.0, _tol)){
        return *this;
    }

    // Pomeranje zeljenog centra u koordinatni
    // pocetak translacijom, primena same
    // transformacije i vracanje u polaznu tacku
    const auto rez = trans(x, y) * *this * trans(-x, -y);

    // Vracanje izracunatog rezultata
    return inplace ? *this = rez : rez;
}

// Centriranje transformacije prema tacki
geom geom::pomeri(tacka& t, const bool inplace)
{
    return pomeri(t[0], t[1], inplace);
}

// Centriranje transformacije prema l-nizu
geom geom::pomeri(Vektor& t, const bool inplace)
{
    if (std::size(t) != 2){
        throw Exc("Vektor nije duzine dva!");
    }

    return pomeri(t[0], t[1], inplace);
}

// Centriranje transformacije prema d-nizu
geom geom::pomeri(Vektor&& t, const bool inplace)
{
    if (std::size(t) != 2){
        throw Exc("Vektor nije duzine dva!");
    }

    return pomeri(t[0], t[1], inplace);
}

// Pretvaranje preslikavanja u nisku; ova
// fja je, izmedju ostalog, nacin za izbegavanje
// postojanja prijateljskog operatora ispisa
std::string geom::str() const
{
    // Prazno preslikavanje; moguce u slucaju
    // pomerenog (move) ili neispravnog, lose
    // inicijalizovanog ili izmenjenog objekta
    if (_mat.empty()){
        return "[]";
    }

    // Izlazni stringovni tok; ovo ubrzava
    // stvar u odnosu na konkatenaciju niski
    std::ostringstream niska;

    // Fiksiranje broja decimala i postavljanje
    // preciznosti na skroman, ne prevelik broj
    niska << std::fixed;
    niska << std::setprecision(2);

    // Otvaranje glavnih zagrada
    niska << "[";

    // Dodavanje svakog reda u novom redu
    for (Velicina i = 0; i < _size; i++){
        // Razmak ako nije prvi
        if (i != 0){
            niska << " ";
        }

        // Kopiranje reda na izlazni tok
        niska << "[";
        std::copy(std::cbegin(_mat[i]),
                  std::cend(_mat[i])-1,
                  // Iterator izlaznog toka sa separatorom
                  std::ostream_iterator<Element>(niska, ", "));
        niska << _mat[i][_size-1];
        niska << "]";

        // Novi red ako nije poslednji
        if (i != _size-1){
            niska << std::endl;
        }
    }

    // Zatvaranje glavnih zagrada
    niska << "]";

    // Vracanje niske iz toka
    return niska.str();
}

// Operator mnozenja
geom geom::operator*(const geom& dr) const
{
    return util::mult(*this, dr);
}

// Mnozenje numerickom vrednoscu zdesna
geom geom::operator*(const double broj) const
{
    return broj * *this;
}

// Operator mnozenja sa dodelom; ne cini
// mi se da je moguca ikakva optimizacija
geom& geom::operator*=(const geom& dr)
{
    return *this = *this * dr;
}

// Mnozenje numerickom vrednoscu zdesna sa dodelom
geom& geom::operator*=(const double broj)
{
    return *this = broj * *this;
}

// Operator stepenovanja; koristi se logaritamski
// algoritam stepenovanja; PAZNJA: ovde je bitovski
// xor preopterecen, pa ima manji prioritet od bilo
// kog aritmetickog ili relacionog operatora, tako
// da u upotrebi mora biti npr. a*(b^2), dok se a*b^2
// tumaci kao (a*b)^2, sto bi vrv bilo neocekivano
inline geom geom::operator^(const int i) const
{
    return util::pow(*this, i);
}

// Operator stepenovanja sa dodelom;
// opet ne vidim mesto za doradu
geom& geom::operator^=(const int i)
{
    return *this = *this ^ i;
}

// Operator jednakosti
bool geom::operator==(const geom& dr) const
{
    // Izbegavamo, zbog tolerancije
    // na sitnu gresku u racunu
    //return _mat == dr._mat;

    return util::jednakost(_mat, dr._mat, _tol);
}

// Operator nejednakosti
bool geom::operator!=(const geom& dr) const
{
    return !(*this == dr);
}

// Operator dodele l-vektora
geom& geom::operator=(const Matrica& v)
{
    _mat = v;

    // Provera korektnosti preslikavanja
    proveri();

    return *this;
}

// Operator dodele r-vektora
geom& geom::operator=(Matrica&& v)
{
    _mat = std::move(v);

    // Provera korektnosti preslikavanja
    proveri();

    return *this;
}

// Operator indeksiranja za dohvatanje;
// vraca referencu da ne bi bilo kopiranja;
// briga o validnosti indeksa prepusta se
// vektoru, koji se nalazi ispod omotaca;
// menjanje nije dozovoljeno (const)
const Vektor& geom::operator[](const Velicina i) const
{
    return _mat.at(i);
}

// Struktura koja sluzi za citanje
// toka razdvojenog zapetama
geom::Citac::Citac(geom& g, const Velicina i)
    : g(g), i(i)
{}

// Upisivanje brojeva po zapeti
geom::Citac geom::Citac::operator,(const Element x) const
{
    // Provera indeksa
    if (i >= g._size*g._size){
        g._mat.clear();
        throw Exc("Visak argumenata!");
    }

    // Provera poslednjeg reda
    if (i/g._size == g._size-1){
        if ((i%g._size != g._size-1 && !util::jednakost(x, 0.0, g._tol)) ||
            (i%g._size == g._size-1 && !util::jednakost(x, 1.0, g._tol))){
            g._mat.clear();
            throw Exc("Poslednji red mora biti {0, 0, ..., 1}!");
        }
    }

    // Upis broja
    g._mat[i/g._size][i%g._size] = x;

    // Nastavljanje dalje
    return Citac(g, i+1);
}

// Upisivanje prvog broja u matricu
geom::Citac geom::operator<<(const Element x)
{
    // Upis broja
    _mat[0][0] = x;

    // Nastavljanje dalje
    return Citac(*this, 1);
}

// Provera korektnosti preslikavanja
void geom::proveri()
{
    if (std::size(_mat) == _size-1){
        // Dodavanje homogenog dela ako fali
        // na kopiran vec ucitani podvektor
        Vektor mat;
        std::fill_n(std::back_inserter(mat),
                    _size-1,
                    0);
        mat.push_back(1);

        // Preuzimanje implementacije
        _mat.push_back(std::move(mat));
    }

    // Izbacivanje izuzetka u slucaju
    // nekorektnog ulaznog vektora
    if (std::size(_mat) != _size){
        _mat.clear();
        throw Exc("Vektor nije duzine " +
                  std::to_string(_size) + "!");
    }

    // U slucaju nekorektnih redova
    for (Velicina i = 0; i < _size; i++){
        if (std::size(_mat[i]) != _size){
            _mat.clear();
            throw Exc("Podvektor " + std::to_string(i) +
                      " nije duzine " + std::to_string(_size) + "!");
        }
    }

    // U slucaju nekorektnog poslednjeg reda
    if (!util::jednakost(std::vector<Element>{
                         std::accumulate(std::cbegin(_mat[_size-1]),
                                         std::cend(_mat[_size-1])-1,
                                         0.0,
                                         std::plus<>()),
                         _mat[_size-1][_size-1]},
                         std::vector<Element>{0, 1},
                         _tol)){
        _mat.clear();
        throw Exc("Poslednji podvektor nije oblika {0, 0, ..., 1}!");
    }
}

// Operator ispisa na izlazni tok
std::ostream& operator<<(std::ostream& out, const geom& g)
{
    return out << g.str();
}

// Operator citanja sa ulaznog toka
std::istream& operator>>(std::istream& in, geom& g)
{
    // Kopiranje vrednosti sa ulaza
    for (Velicina i = 0; i < g._size; i++){
                    // Iterator ulaznog toka
        std::copy_n(std::istream_iterator<Element>(in),
                    g._size,
                    std::begin(g._mat[i]));
    }

    // Provera korektnosti ulaza
    try{
        g.proveri();
    } catch (Exc&){
        in.setstate(std::ios::failbit);
    }

    // Vracanje citanog toka
    return in;
}

// Staticka verzija fje za inverz
geom inv(geom& g, const bool inplace)
{
    return g.inv(inplace);
}

// Staticko centriranje prema koordinatama
geom pomeri(geom& g, const double x, const double y, const bool inplace)
{
    return g.pomeri(x, y, inplace);
}

// Centriranje transformacije prema tacki
geom pomeri(geom& g, tacka& t, const bool inplace)
{
    return g.pomeri(t, inplace);
}

// Centriranje transformacije prema l-nizu
geom pomeri(geom& g, Vektor& t, const bool inplace)
{
    return g.pomeri(t, inplace);
}

// Centriranje transformacije prema d-nizu
geom pomeri(geom& g, Vektor&& t, const bool inplace)
{
    return g.pomeri(t, inplace);
}

// Mmnozenje numerickom vrednoscu sleva;
geom operator*(const double broj, const geom& g)
{
    // Inicijalizacija rezultata;
    // on je matrica transformacije
    auto rez = static_cast<Matrica>(g.mat());

    // Mnozenje prvih redova
    for (Velicina i = 0; i < std::size(rez)-1; i++){
        std::transform(std::cbegin(rez[i]),
                       std::cend(rez[i]),
                       // Red = rezultat
                       std::begin(rez[i]),
                       // Mnozenje sleva
                       std::bind(std::multiplies<>(),
                                 broj,
                                 std::placeholders::_1));
    }

    // Vracanje rezultata; eksplicitno pomeranje
    // kako bi se isforsirala optimizacija (RVO)
    return std::move(rez);
}

}
