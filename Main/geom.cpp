#include "geom.hpp"
#include "util.hpp"
#include <vector>
#include <sstream>
#include <iterator>
#include <numeric>
#include <functional>

// Imenski prostor preslikavanja
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

// Konstruktor od vektora kao l-vrednosti
geom::geom(const Tip& v)
    : _mat(v)
{
    // Provera korektnosti preslikavanja
    proveri();
}

// Konstruktor od vektora kao r-vrednosti
geom::geom(Tip&& v)
    : _mat(std::move(v))
{
    // Provera korektnosti preslikavanja
    proveri();
}

// Konstruktor od vektorâ kao l-vrednosti;
// ovaj se poziva i u mesanom slucaju, ako su
// dva ili jedan od tri zapravo r-vrednosti
geom::geom(const PodTip& v1,
           const PodTip& v2,
           const PodTip& v3)
    : _mat({v1, v2, v3})
{
    // Provera korektnosti preslikavanja
    proveri();
}

// Konstruktor od vektorâ kao r-vrednosti;
// poziva se iskljucivo ako su sva tri desna
geom::geom(PodTip&& v1, PodTip&& v2, PodTip&& v3)
    : _mat({std::move(v1), std::move(v2), std::move(v3)})
{
    // Provera korektnosti preslikavanja
    proveri();
}

// Dohvatac (getter) za matricu
const Tip& geom::mat() const
{
    return _mat;
}

// Dohvatac (getter) za velicinu
Vel geom::size() const
{
    return _size;
}

// Kolekcijski metod za pocetak matrice
Iter geom::begin() const noexcept
{
    return std::begin(this->_mat);
}

// Kolekcijski metod za const pocetak matrice
Iter geom::cbegin() const noexcept
{
    return std::cbegin(this->_mat);
}

// Kolekcijski metod za kraj matrice
Iter geom::end() const noexcept
{
    return std::end(this->_mat);
}

// Kolekcijski metod za const kraj matrice
Iter geom::cend() const noexcept
{
    return std::cend(this->_mat);
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
    auto [a, b, c,
          d, e, f] = std::tie(_mat[0][0], _mat[0][1], _mat[0][2],
                              _mat[1][0], _mat[1][1], _mat[1][2]);

    // Nije moguce izracunati inverz
    // ukoliko je matrica singularna
    if (util::jednakost(a*e, b*d, _tol)){
        throw Exc("Singularna matrica!");
    }

    // Racunanje nekih kofaktora
    auto k = 1 / (a*e - b*d);
    auto t1 = b*f - c*e;
    auto t2 = c*d - a*f;

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

// Pretvaranje preslikavanja u nisku; ova
// fja je, izmedju ostalog, nacin za izbegavanje
// postojanja prijateljskog operatora ispisa
std::string geom::str() const
{
    // Prazno preslikavanje; moguce samo
    // nakon pomeranja (move) objekta
    if (_mat.empty()){
        return "[]";
    }

    // Izlazni stringovni tok; ovo ubrzava
    // stvar u odnosu na konkatenaciju niski
    std::ostringstream niska;

    // Otvaranje glavnih zagrada
    niska << "[";

    // Dodavanje svakog reda u novom redu
    for (Vel i = 0; i < _size; i++){
        // Razmak ako nije prvi
        if (i != 0){
            niska << " ";
        }

        // Kopiranje reda na izlazni tok
        niska << "[";
        std::copy(std::cbegin(_mat[i]),
                  std::cend(_mat[i])-1,
                  // Iterator izlaznog toka sa separatorom
                  std::ostream_iterator<Elem>(niska, ", "));
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

// Operator mnozenja sa dodelom; ne cini
// mi se da je moguca ikakva optimizacija
geom& geom::operator*=(const geom& dr)
{
    return *this = *this * dr;
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
    //return this->_mat == dr._mat;

    return util::jednakost(this->_mat, dr._mat, _tol);
}

// Operator nejednakosti
bool geom::operator!=(const geom& dr) const
{
    return !(*this == dr);
}

// Operator dodele l-vektora
geom& geom::operator=(const Tip& v)
{
    _mat = v;

    // Provera korektnosti preslikavanja
    proveri();

    return *this;
}

// Operator dodele r-vektora
geom& geom::operator=(Tip&& v)
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
const PodTip& geom::operator[](const Vel i) const
{
    return _mat.at(i);
}

// Struktura koja sluzi za citanje
// toka razdvojenog zapetama
geom::Citac::Citac(geom& g, const Vel i)
    : g(g), i(i)
{}

// Upisivanje brojeva po zapeti
geom::Citac geom::Citac::operator,(const Elem x)
{
    // Provera indeksa
    if (i >= g._size*g._size){
        throw Exc("Visak argumenata!");
    }

    // Provera poslednjeg reda
    if (i/g._size == g._size-1){
        if ((i%g._size != g._size-1 && !util::jednakost(x, 0.0, g._tol)) ||
            (i%g._size == g._size-1 && !util::jednakost(x, 1.0, g._tol))){
            throw Exc("Poslednji red mora biti {0, 0, ..., 1}!");
        }
    }

    // Upis broja
    g._mat[i/g._size][i%g._size] = x;

    // Nastavljanje dalje
    return Citac(g, i+1);
}

// Upisivanje prvog broja u matricu
geom::Citac geom::operator<<(const Elem x)
{
    // Upis broja
    _mat[0][0] = x;

    // Nastavljanje dalje
    return Citac(*this, 1);
}

// Provera korektnosti preslikavanja
void geom::proveri() const
{
    // Izbacivanje izuzetka u slucaju
    // nekorektnog ulaznog vektora
    if (std::size(_mat) != _size){
        throw Exc("Vektor nije duzine " +
                  std::to_string(_size) + "!");
    }

    // U slucaju nekorektnih redova
    for (Vel i = 0; i < _size; i++){
        if (std::size(_mat[i]) != _size){
            throw Exc("Podvektor " + std::to_string(i) +
                      " nije duzine " + std::to_string(_size) + "!");
        }
    }

    // U slucaju nekorektnog poslednjeg reda
    if (!util::jednakost(std::vector<Elem>{
                         std::accumulate(std::cbegin(_mat[_size-1]),
                                         std::cend(_mat[_size-1])-1,
                                         0.0,
                                         std::plus<>()),
                         _mat[_size-1][_size-1]},
                         std::vector<Elem>{0, 1},
                         _tol)){
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
    for (Vel i = 0; i < g._size; i++){
                    // Iterator ulaznog toka
        std::copy_n(std::istream_iterator<Elem>(in),
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

}
