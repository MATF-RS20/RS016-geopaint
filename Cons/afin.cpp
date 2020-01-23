#include "afin.hpp"

// Imenski prostor za geometriju;
// u ovoj datoteci su izvedene konkretne
// afine transformacije i povezano
namespace geom {

// Translacija od koordinata
trans::trans(double x, double y, const bool inv)
{
    // Inverzna je aditivni inverz:
    //     |1   0   x|            |1   0  -x|
    // T = |0   1   y|,  inv(T) = |0   1  -y|
    //     |0   0   1|            |0   0   1|
    if (inv){
        x = -x;
        y = -y;
    }

    // Matrica translacije u ravni
    _mat = {{1, 0, x},
            {0, 1, y},
            {0, 0, 1}};
}

// Translacija od tacke
trans::trans(const tacka& t, const bool inv)
    : trans(t[0], t[1], inv)
{}

// Skaliranje od koordinata
skal::skal(double x, double y, const double t1, const double t2, const bool inv)
{
    // Inverzna je multiplikativni inverz:
    //     |x   0   0|            |1/x  0   0|
    // T = |0   y   0|,  inv(T) = | 0  1/y  0|
    //     |0   0   1|            | 0   0   1|
    if (inv){
        if (util::jednakost(x, 0.0, _tol) ||
            util::jednakost(y, 0.0, _tol)){
            throw Exc("Deljenje nulom pri skaliranju!");
        }

        x = 1/x;
        y = 1/y;
    }

    // Matrica skaliranja u ravni
    _mat = {{x, 0, 0},
            {0, y, 0},
            {0, 0, 1}};

    // Eventualno centriranje preslikavanja
    pomeri(t1, t2, true);
}

// Skaliranje od koordinata
skal::skal(double t1, double t2, const tacka& t, const bool inv)
    : skal(t1, t2, t[0], t[1], inv)
{}

// Skaliranje od tacke
skal::skal(const tacka& t, const double t1, const double t2, const bool inv)
    : skal(t[0], t[1], t1, t2, inv)
{}

// Skaliranje od tacke
skal::skal(const tacka& t1, const tacka& t2, const bool inv)
    : skal(t1[0], t1[1], t2[0], t2[1], inv)
{}

// Smicanje od koordinata
smic::smic(double x, double y, const double t1, const double t2, const bool inv)
{
   // Inverzna je aditivni inverz, ali
   // skaliran za korektivni faktor:
   //     |1   x   0|                       | 1  -x   0|
   // T = |y   1   0|,  inv(T) = 1/(1-xy) * |-y   1   0|
   //     |0   0   1|                       | 0   0   1|

    auto f = 1.0;
    if (inv){
        if (util::jednakost(x*y, 1.0, _tol)){
            throw Exc("Deljenje nulom pri smicanju!");
        }

        f = 1/(1-x*y);
        x = -x;
        y = -y;
    }

    // Matrica smicanja u ravni
    _mat = {{ f,  f*x, 0},
            {f*y,  f,  0},
            { 0,   0,  1}};

    // Eventualno centriranje preslikavanja
    pomeri(t1, t2, true);
}

// Smicanje od koordinata
smic::smic(double t1, double t2, const tacka& t, const bool inv)
    : smic(t1, t2, t[0], t[1], inv)
{}

// Smicanje od tacke
smic::smic(const tacka& t, const double t1, const double t2, const bool inv)
    : smic(t[0], t[1], t1, t2, inv)
{}

// Smicanje od tacke
smic::smic(const tacka& t1, const tacka& t2, const bool inv)
    : smic(t1[0], t1[1], t2[0], t2[1], inv)
{}

// Rotacija od ugla sa koordinatama;
// NAPOMENA: ugao u_deg je u stepenima!!
rot::rot(double u_deg, const double t1, const double t2, const bool inv)
{
    // Inverzna je aditivni inverz:
    //     |cos(u)  -sin(u)   0|            | cos(u)   sin(u)   0|
    // T = |sin(u)   cos(u)   0|,  inv(T) = |-sin(u)   cos(u)   0|
    //     |  0        0      1|            |   0        0      1|
    if (inv){
        u_deg = -u_deg;
    }

    // Koeficijenti rotacije u ravni
    const auto pom1 = cos(util::deg2rad(u_deg));
    const auto pom2 = sin(util::deg2rad(u_deg));

     // Matrica rotacije u ravni
     _mat = {{pom1, -pom2,  0},
             {pom2,  pom1,  0},
             { 0,     0,    1}};

     // Eventualno centriranje preslikavanja
     pomeri(t1, t2, true);
}

// Rotacija od ugla sa tackom
rot::rot(double u, const tacka& t, const bool inv)
    : rot(u, t[0], t[1], inv)
{}

// Refleksija od ugla sa koordinatama;
// NAPOMENA: ugao u_deg je u stepenima!!
refl::refl(double u_deg, const double t1, const double t2, const bool)
{
    // Svaka refleksija je samoj sebi inverzna:
    //     |cos(2u)   sin(2u)   0|
    // T = |sin(2u)  -cos(2u)   0|,  inv(T) = T
    //     |  0         0       1|

    // Koeficijenti refleksije u ravni
    const auto pom1 = cos(util::deg2rad(2*u_deg));
    const auto pom2 = sin(util::deg2rad(2*u_deg));

    // Matrica refleksije u ravni
    _mat = {{pom1,  pom2,  0},
            {pom2, -pom1,  0},
            { 0,     0,    1}};

    // Eventualno centriranje preslikavanja
    pomeri(t1, t2, true);
}

// Refleksija od ugla sa tackom
refl::refl(double u, const tacka& t, const bool inv)
    : refl(u, t[0], t[1], inv)
{}

}
