#include <iostream>

#include "util.hpp"
#include "geom.hpp"
#include "tacka.hpp"
#include "afin.hpp"

int main()
{
    const geom::tacka t{3, 0, 3};
    std::cerr << t << std::endl << std::endl;

    geom::rot r(90, {2, 0});
    std::cerr << r << std::endl;
    std::cerr << r * t << std::endl;
}
