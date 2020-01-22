#include <iostream>

#include "oblik.hpp"

int main()
{
    std::vector<geom::tacka> niz{{3, 0, 3}};
    std::cerr << niz[0] << std::endl << std::endl;

    geom::poly o(niz);
    o.dodaj({1, 1});
    std::cerr << o.tacke()[0] << std::endl;
    std::cerr << o.tacke()[1] << std::endl;
}
