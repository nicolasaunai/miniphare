#ifndef ELECTROMAG_H
#define ELECTROMAG_H


#include <string>

#include "types.h"
#include "constants.h"
#include "vecfield/vecfield.h"
#include "Initializer/electromaginitializer.h"


class Electromag
{
public:

    Electromag(std::array<AllocSizeT, NBR_COMPO> E_AllocSizes,
               std::array<AllocSizeT, NBR_COMPO> B_AllocSizes,
               std::string name="");

    Field& getEi(uint32 component) {return E_.component(component);}
    Field& getBi(uint32 component) {return B_.component(component);}
    VecField& getE(){return E_;}
    VecField& getB(){return B_;}


    void init(std::unique_ptr<ElectromagInitializer> initializer);


private:
    VecField E_;
    VecField B_;

};

#endif // ELECTROMAG_H
