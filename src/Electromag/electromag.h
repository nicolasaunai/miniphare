#ifndef ELECTROMAG_H
#define ELECTROMAG_H


#include <string>

#include "Initializer/electromaginitializer.h"
#include "constants.h"
#include "types.h"
#include "vecfield/vecfield.h"


class Electromag
{
public:
    Electromag(std::array<AllocSizeT, NBR_COMPO> E_AllocSizes,
               std::array<AllocSizeT, NBR_COMPO> B_AllocSizes, std::string name = "");

    Electromag(std::unique_ptr<ElectromagInitializer> initializer);

    Field& getEi(uint32 component) { return E_.component(component); }
    Field const& getEi(uint32 component) const { return E_.component(component); }

    Field& getBi(uint32 component) { return B_.component(component); }
    Field const& getBi(uint32 component) const { return B_.component(component); }

    VecField& getE() { return E_; }
    VecField& getB() { return B_; }

    VecField const& getE() const { return E_; }
    VecField const& getB() const { return B_; }

    // void init(ElectromagInitializer* const initializer);


private:
    VecField E_;
    VecField B_;
};

#endif // ELECTROMAG_H
