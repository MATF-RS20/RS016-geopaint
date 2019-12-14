#include <iostream>
#include "geom.hpp"
#include "tacka.hpp"
#include "util.hpp"

int main()
{
    const geom::tacka t{3, 3, 3};

    std::cerr << t << std::endl;

    std::cerr << 2*t << std::endl;

    std::cerr << (t > t) << std::endl;

    const geom::geom g{{2,  0,  1},
                       {0,  2, -1},
                       {0,  0,  1}};

    std::cerr << g << std::endl;

    std::cerr << g*t << std::endl;

    std::cerr << t/2 << std::endl;

    std::cerr << 2/t << std::endl;

    std::cerr << t-2 << std::endl;

    std::cerr << 2-t << std::endl;
}
