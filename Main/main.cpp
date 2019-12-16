#include <iostream>
#include "geom.hpp"
#include "tacka.hpp"
#include "afin.hpp"
#include "util.hpp"

int main()
{
    const geom::geom g{{2,  0,  1},
                       {0,  2, -1},
                       {0,  0,  1}};

    std::cerr << (g == geom::trans(1, -1) * geom::skal(2, 2)) << std::endl;

    const geom::tacka t{3, 0, 3};

    std::cerr << t << std::endl;

    std::cerr << geom::rot(90, {2, 0}) * t << std::endl;
}
