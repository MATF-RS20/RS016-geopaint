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
    std::cerr << r * t << std::endl << std::endl;

    r.inv();
    std::cerr << r << std::endl;
    std::cerr << r * t << std::endl << std::endl;

    r.inv(true);
    std::cerr << r << std::endl;
    std::cerr << r * t << std::endl << std::endl;

    geom::rot f(90);
    f.pomeri(1, 0, true);
    std::cerr << f << std::endl;
    std::cerr << f * t << std::endl << std::endl;
}
