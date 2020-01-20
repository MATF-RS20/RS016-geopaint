#ifndef POSETILAC_H
#define POSETILAC_H

class Posetilac {
public:
    Posetilac() = default;
    virtual ~Posetilac() = default;

    virtual void poseti_poligon() = 0;
    virtual void poseti_elipsu() = 0;
    virtual void poseti_krug() = 0;
};


#endif // POSETILAC_H
