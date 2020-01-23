#ifndef ALIJASI_HPP
#define ALIJASI_HPP

#include "util.hpp"

// Imenski prostor za geometriju
namespace geom {

// Neophodna obecavajuca deklaracija
class tacka;

// Izdvajanje imena tipova,
// zapravo njihovih alijasa
using Matrica = std::vector<std::vector<double>>;
using Vektor = Matrica::value_type;
using Element = Vektor::value_type;
using Velicina = Matrica::size_type;
using NizTacaka = std::vector<tacka>;

}

#endif // ALIJASI_HPP
