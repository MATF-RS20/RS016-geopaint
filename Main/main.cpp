#include <iostream>

#include "geom.hpp"

int main()
{
    const geom::tacka t{3, 0, 3};
    std::cerr << t << std::endl << std::endl;

    const geom::rot r(90, {2, 0});
    std::cerr << r << std::endl << std::endl;

    std::cerr << r * t << std::endl;
}
