#ifndef ELECTROMAG_H
#define ELECTROMAG_H


#include <string>

#include "types.h"
#include "vecfield/vecfield.h"



class Electromag
{
public:

    Electromag(uint32 nx, uint32 ny, uint32 nz,  std::string name="");


    Field& getEi(uint32 component) {return E_.component(component);}
    Field& getBi(uint32 component) {return B_.component(component);}
    VecField& getE(){return E_;}
    VecField& getB(){return B_;}


private:
    VecField E_;
    VecField B_;

};

#endif // ELECTROMAG_H
