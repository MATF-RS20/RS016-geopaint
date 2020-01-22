#ifndef AFIN_HPP
#define AFIN_HPP

#include "tacka.hpp"

// Imenski prostor za geometriju
namespace geom {

// Neophodne obacavajuce deklaracije
class tacka;

// Translacija u ravni
class trans : public geom
{
public:
    // Konsktruktori sa koordinatama
    trans(double, double, const bool = false);
    trans(const tacka&, const bool = false);
};

// Skaliranje u ravni
class skal : public geom
{
public:
    // Konsktruktori sa koordinatama
    skal(double, double, const double, const double, const bool = false);
    skal(double, double, const tacka& = {0, 0}, const bool = false);
    skal(const tacka&, const double, const double, const bool = false);
    skal(const tacka&, const tacka& = {0, 0}, const bool = false);
};

// Smicanje u ravni
class smic : public geom
{
public:
    // Konsktruktori sa koordinatama
    smic(double, double, const double, const double, const bool = false);
    smic(double, double, const tacka& = {0, 0}, const bool = false);
    smic(const tacka&, const double, const double, const bool = false);
    smic(const tacka&, const tacka& = {0, 0}, const bool = false);
};

// Rotacija u ravni
class rot : public geom
{
public:
    // Konstruktori sa uglom
    rot(double, const double, const double, const bool = false);
    rot(double, const tacka& = {0, 0}, const bool = false);
};

// Refleksija u ravni
class refl : public geom
{
public:
    // Konstruktori sa uglom
    refl(double, const double, const double, const bool = false);
    refl(double, const tacka& = {0, 0}, const bool = false);
};

}

#endif // AFIN_HPP
