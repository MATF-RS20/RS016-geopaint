#include <sstream>
#include <iterator>
#include <iomanip>

#include "geom.hpp"

// Imenski prostor za geometriju;
// u ovoj datoteci se nalazi klasa za
// predstavljanje tacke i povezano
namespace geom
{

// Konstruktor bez argumenata
tacka::tacka()
    : _mat({0, 0, 1})
{}

// Konstruktor od dve vrednosti
tacka::tacka(Elem x, Elem y)
    : _mat({x, y, 1})
{}

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

// Dohvatac (getter) za toleranciju
Elem tacka::tol() const
{
    return _tol;
}

// Kolekcijski metod za pocetak matrice
PodIter tacka::begin() const noexcept
{
    return std::begin(_mat);
}

// Kolekcijski metod za const pocetak matrice
PodIter tacka::cbegin() const noexcept
{
    return std::cbegin(_mat);
}

// Kolekcijski metod za kraj matrice
PodIter tacka::end() const noexcept
{
    return std::end(_mat);
}

// Kolekcijski metod za const kraj matrice
PodIter tacka::cend() const noexcept
{
    return std::cend(_mat);
}

// Pretvaranje tacke u nisku; ova fja je,
// izmedju ostalog, nacin za izbegavanje
// prijateljskog operatora ispisa
std::string tacka::str() const
{
    // Prazna tacka; moguce samo u slucaju
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

// Primena transformacije na tacku
void tacka::primeni(const geom& g)
{
    *this = g * *this;
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
        t._mat[i] = _mat[i] + dr._mat[i];
    }

    return t;
}

// Sabiranje sa numerickom vrednoscu
tacka tacka::operator+(const double broj) const
{
    return broj + *this;
}

// Operator sabiranja sa dodelom
tacka& tacka::operator+=(const tacka& dr)
{
    for (Vel i = 0; i < _size-1; i++){
        _mat[i] += dr._mat[i];
    }

    return *this;
}

// Sabiranje sa numerickom vrednoscu sa dodelom
tacka& tacka::operator+=(const double broj)
{
    for (Vel i = 0; i < _size-1; i++){
        _mat[i] += broj;
    }

    return *this;
}

// Unarna negacija
tacka tacka::operator-() const
{
    tacka t;

    for (Vel i = 0; i < _size-1; i++){
        t._mat[i] = -_mat[i];
    }

    return t;
}

// Operator oduzimanja
tacka tacka::operator-(const tacka& dr) const
{
    tacka t;

    for (Vel i = 0; i < _size-1; i++){
        t._mat[i] = _mat[i] - dr._mat[i];
    }

    return t;
}

// Oduzimanje numericke vrednosti
tacka tacka::operator-(const double broj) const
{
    return -broj + *this;
}

// Operator oduzimanja sa dodelom
tacka& tacka::operator-=(const tacka& dr)
{
    for (Vel i = 0; i < _size-1; i++){
        _mat[i] -= dr._mat[i];
    }

    return *this;
}

// Oduzimanje numericke vrednosti sa dodelom
tacka& tacka::operator-=(const double broj)
{
    for (Vel i = 0; i < _size-1; i++){
        _mat[i] -= broj;
    }

    return *this;
}

// Mnozenje numerickom vrednoscu
tacka tacka::operator*(const double broj) const
{
    return broj * *this;
}

// Mnozenje numerickom vrednoscu sa dodelom
tacka& tacka::operator*=(const double broj)
{
    for (Vel i = 0; i < _size-1; i++){
        _mat[i] *= broj;
    }

    return *this;
}

// Deljenje numerickom vrednoscu
tacka tacka::operator/(const double broj) const
{
    return 1/broj * *this;
}

// Deljenje numerickom vrednoscu sa dodelom
tacka& tacka::operator/=(const double broj)
{
    for (Vel i = 0; i < _size-1; i++){
        _mat[i] /= broj;
    }

    return *this;
}

// Operator jednakosti
bool tacka::operator==(const tacka& dr) const
{
    // Izbegavamo, zbog tolerancije
    // na sitnu gresku u racunu
    //return _mat == dr._mat;

    return util::jednakost(_mat, dr._mat, _tol);
}

// Operator poretka
bool tacka::operator<(const tacka& dr) const
{
    // Izbegavamo, zbog tolerancije
    // na sitnu gresku u racunu
    //return _mat < dr._mat;

    // Pocetni, nulti indeks
    Vel i = 0;

    // Kretanje kroz svaku koordinatu
    // dokle god su jednaki parovi
    while (i < _size-1 && util::jednakost(_mat[i], dr._mat[i], _tol)){
        i++;
    }

    // Ukoliko se stiglo do kraja kolekcija,
    // jednake su odnosno nije prva manja, a
    // u suprotnom se celokupan rezultat dobija
    // poredjenjem prvog nejednakog para
    return i != _size-1 && _mat[i] < dr._mat[i];
}

// Operator dodele l-matrice
tacka& tacka::operator=(const Tip& v)
{
    _mat = v[0];

    // Provera korektnosti matrice
    proverim(v);

    // Provera korektnosti tacke
    proveri();

    return *this;
}

// Operator dodele d-matrice
tacka& tacka::operator=(Tip&& v)
{
    _mat = std::move(v[0]);

    // Provera korektnosti matrice
    proverim(v);

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
const Elem& tacka::operator[](const Vel i) const
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
        t._mat.clear();
        throw Exc("Visak argumenata!");
    }

    // Upis broja
    t._mat[i] = x;

    // Provera po zavrsetku
    if (i == t._size-1){
        t.proveri();
    }

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
    // Dodavanje homogenog dela ako fali
    // na kopiran vec ucitani (pod)vektor
    if (std::size(_mat) == _size-1){
        _mat.push_back(1);
    }

    // Izbacivanje izuzetka u slucaju
    // nekorektne velicine vektora
    if (std::size(_mat) != _size){
        _mat.clear();
        throw Exc("Vektor nije duzine " +
                  std::to_string(_size) + "!");
    }

    // Greska u slucaju beskonacne tacke
    if (util::jednakost(_mat[_size-1], 0.0, _tol)){
        _mat.clear();
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

// Provera korektnosti matrice
void tacka::proverim(const Tip& v)
{
    // Mora biti oblika 1x3
    if (std::size(v) != 1){
        _mat.clear();
        throw Exc("Matrica nije ranga 1!");
    }
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
                t._size-1,
                std::begin(t._mat));

    // Uzimanje poslednje ako postoji
    if (!in.eof()) {
        in >> t._mat[t._size-1];
    }

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
    const auto vel = std::size(t);

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

// Operator za sabiranje sa numerickom vrednoscu
tacka operator+(const double broj, const tacka& t)
{
    return util::primeni(broj,
                         t,
                         std::plus<>());
}

// Operator za oduzimanje numericke vrednosti
tacka operator-(const double broj, const tacka& t)
{
    return util::primeni(broj,
                         -t,
                         std::plus<>());
}

// Operator za mnozenje numerickom vrednoscu
tacka operator*(const double broj, const tacka& t)
{
    return util::primeni(broj,
                         t,
                         std::multiplies<>());
}

// Operator za deljenje numerickom vrednoscu
tacka operator/(const double broj, const tacka& t)
{
    return util::primeni(broj,
                         tacka(1/t[0], 1/t[1], 1),
                         std::multiplies<>());
}

}
