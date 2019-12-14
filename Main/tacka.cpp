#include <sstream>
#include <iterator>
#include "tacka.hpp"

// Imenski prostor za geometriju
namespace geom
{

// Konstruktor od dve vrednosti
tacka::tacka(Elem x, Elem y)
    : _mat({x, y, 1})
{
    // Provera korektnosti tacke
    proveri();
}

// Konstruktor od tri vrednosti
tacka::tacka(Elem x, Elem y, Elem z)
    : _mat({x, y, z})
{
    // Provera korektnosti tacke
    proveri();
}

// Konstruktor od matrice kao l-vrednost
tacka::tacka(const Tip& v)
    : _mat(v[0])
{
    // Provera korektnosti matrice
    proverim(v);

    // Provera korektnosti tacke
    proveri();
}

// Konstruktor od matrice kao r-vrednost
tacka::tacka(Tip&& v)
    : _mat(std::move(v[0]))
{
    // Provera korektnosti matrice
    proverim(v);

    // Provera korektnosti tacke
    proveri();
}

// Konstruktor od vektora kao l-vrednost
tacka::tacka(const PodTip& v)
    : _mat(v)
{
    // Provera korektnosti tacke
    proveri();
}

// Konstruktor od vektora kao r-vrednost
tacka::tacka(PodTip&& v)
    : _mat(std::move(v))
{
    // Provera korektnosti tacke
    proveri();
}

// Dohvatac (getter) za matricu
const PodTip& tacka::mat() const
{
    return _mat;
}

// Dohvatac (getter) za velicinu
Vel tacka::size() const
{
    return _size;
}

// Kolekcijski metod za pocetak matrice
PodIter tacka::begin() const noexcept
{
    return std::begin(this->_mat);
}

// Kolekcijski metod za const pocetak matrice
PodIter tacka::cbegin() const noexcept
{
    return std::cbegin(this->_mat);
}

// Kolekcijski metod za kraj matrice
PodIter tacka::end() const noexcept
{
    return std::end(this->_mat);
}

// Kolekcijski metod za const kraj matrice
PodIter tacka::cend() const noexcept
{
    return std::cend(this->_mat);
}

// Pretvaranje tacke u nisku; ova fja je,
// izmedju ostalog, nacin za izbegavanje
// prijateljskog operatora ispisa
std::string tacka::str() const
{
    // Prazna tacka; moguce samo
    // nakon pomeranja (move)
    if (_mat.empty()){
        return "[]";
    }

    // Izlazni stringovni tok; ovo ubrzava
    // stvar u odnosu na konkatenaciju niski
    std::ostringstream niska;

    // Otvaranje glavnih zagrada
    niska << "[";

    // Kopiranje prvih vrednosti na izlazni tok
    std::copy(std::cbegin(_mat),
              std::cend(_mat)-1,
              // Iterator izlaznog toka sa separatorom
              std::ostream_iterator<Elem>(niska, ", "));

    // Kopiranje krajnje vrednosti na izlazni tok
    niska << _mat[_size-1];

    // Zatvaranje glavnih zagrada
    niska << "]";

    // Vracanje niske iz toka
    return niska.str();
}

// Unarna potvrda
tacka tacka::operator+() const
{
    return *this;
}

// Operator sabiranja
tacka tacka::operator+(const tacka& dr) const
{
    tacka t;

    for (Vel i = 0; i < _size-1; i++){
        t._mat[i] = this->_mat[i] + dr._mat[i];
    }

    return t;
}

// Operator sabiranja sa dodelom
tacka& tacka::operator+=(const tacka& dr)
{
    for (Vel i = 0; i < _size-1; i++){
        this->_mat[i] += dr._mat[i];
    }

    return *this;
}

// Unarna negacija
tacka tacka::operator-() const
{
    tacka t;

    for (Vel i = 0; i < _size-1; i++){
        t._mat[i] = -this->_mat[i];
    }

    return t;
}

// Operator oduzimanja
tacka tacka::operator-(const tacka& dr) const
{
    tacka t;

    for (Vel i = 0; i < _size-1; i++){
        t._mat[i] = this->_mat[i] - dr._mat[i];
    }

    return t;
}

// Operator oduzimanja sa dodelom
tacka& tacka::operator-=(const tacka& dr)
{
    for (Vel i = 0; i < _size-1; i++){
        this->_mat[i] -= dr._mat[i];
    }

    return *this;
}

// Operator jednakosti
bool tacka::operator==(const tacka& dr) const
{
    // Izbegavamo, zbog tolerancije
    // na sitnu gresku u racunu
    //return this->_mat == dr._mat;

    return util::jednakost(this->_mat, dr._mat, _tol);
}

// Operator poretka
bool tacka::operator<(const tacka& dr) const
{
    // Izbegavamo, zbog tolerancije
    // na sitnu gresku u racunu
    //return this->_mat < dr._mat;

    // Pocetni, nulti indeks
    Vel i = 0;

    // Kretanje kroz svaku koordinatu
    // dokle god su jednaki parovi
    while (i < _size-1 && util::jednakost(this->_mat[i], dr._mat[i], _tol)){
        i++;
    }

    // Ukoliko se stiglo do kraja kolekcija,
    // jednake su odnosno nije prva manja
    if (i == _size-1){
        return false;
    // U suprotnom se celokupan rezultat dobija
    // poredjenjem prvog nejednakog para
    } else {
        return this->_mat[i] < dr._mat[i];
    }
}

// Operator dodele l-matrice
tacka& tacka::operator=(const Tip& v)
{
    // Provera korektnosti matrice
    proverim(v);

    _mat = v[0];

    // Provera korektnosti tacke
    proveri();

    return *this;
}

// Operator dodele d-matrice
tacka& tacka::operator=(Tip&& v)
{
    // Provera korektnosti matrice
    proverim(v);

    _mat = std::move(v[0]);

    // Provera korektnosti tacke
    proveri();

    return *this;
}

// Operator dodele l-vektora
tacka& tacka::operator=(const PodTip& v)
{
    _mat = v;

    // Provera korektnosti tacke
    proveri();

    return *this;
}

// Operator dodele d-vektora
tacka& tacka::operator=(PodTip&& v)
{
    _mat = std::move(v);

    // Provera korektnosti tacke
    proveri();

    return *this;
}

// Operator indeksiranja za dohvatanje;
// vraca referencu da ne bi bilo kopiranja;
// briga o validnosti indeksa prepusta se
// vektoru, koji se nalazi ispod omotaca;
// menjanje nije dozovoljeno (const)
const Elem& tacka::operator[] (const Vel i) const
{
    return _mat.at(i);
}

// Struktura koja sluzi za citanje
// toka razdvojenog zapetama
tacka::Citac::Citac(tacka& t, Vel i)
    : t(t), i(i)
{}

// Upisivanje brojeva po zapeti
tacka::Citac tacka::Citac::operator,(const Elem x) const
{
    // Provera indeksa
    if (i >= t._size){
        throw Exc("Visak argumenata!");
    }

    // Provera po zavrsetku
    if (i == t._size-1){
        t.proveri();
    }

    // Upis broja
    t._mat[i] = x;

    // Nastavljanje dalje
    return Citac(t, i+1);
}

// Pocetni upis
tacka::Citac tacka::operator<<(const Elem x)
{
    // Upis broja
    _mat[0] = x;

    // Nastavljanje dalje
    return Citac(*this, 1);
}

// Provera korektnosti tacke
void tacka::proveri()
{
    // Izbacivanje izuzetka u slucaju
    // nekorektne velicine vektora
    if (std::size(_mat) != _size){
        throw Exc("Vektor nije duzine " +
                  std::to_string(_size) + "!");
    }

    // Greska u slucaju beskonacne tacke
    if (util::jednakost(_mat[_size-1], 0.0, _tol)){
        throw Exc("Nisu podrzane beskonacne tacke!");
    }

    // Popravka nehomogenizovanih tacaka
    // deljenjem svake poslednjom koordinatom
    std::transform(std::cbegin(_mat),
                   std::cend(_mat),
                   std::begin(_mat),
                   std::bind(std::divides<>(),
                             std::placeholders::_1,
                             _mat[_size-1]));
}

// Operator ispisa na izlazni tok
std::ostream& operator<<(std::ostream& out, const tacka& t)
{
    return out << t.str();
}

// Operator citanja sa ulaznog toka
std::istream& operator>>(std::istream& in, tacka& t)
{
    // Kopiranje vrednosti sa ulaza
    std::copy_n(std::istream_iterator<Elem>(in),
                t._size,
                std::begin(t._mat));

    // Provera korektnosti ulaza
    try{
        t.proveri();
    } catch (Exc&){
        in.setstate(std::ios::failbit);
    }

    // Vracanje citanog toka
    return in;
}

// Operator za mnozenje matrice i tacke
tacka operator*(const geom& g, const tacka& t)
{
    // Velicina izlazne tacke
    auto vel = std::size(t);

    // Nula-vektor kao inicijalni rezultat
    auto rez = PodTip(vel, 0);

    // Klasicno mnozenje u dvostrukoj petlji
    for (Vel i = 0; i < vel; i++){
        for (Vel j = 0; j < vel; j++){
            rez[i] += g[i][j] * t[j];
        }
    }

    // Vracanje rezultata; eksplicitno pomeranje
    // kako bi se isforsirala optimizacija (RVO)
    return std::move(rez);
}

// Provera korektnosti matrice
inline void proverim(const Tip& v)
{
    // Mora biti oblika 1x3
    if (std::size(v) != 1){
        throw Exc("Matrica nije ranga 1!");
    }
}

}
