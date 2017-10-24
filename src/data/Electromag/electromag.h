#ifndef ELECTROMAG_H
#define ELECTROMAG_H


#include <string>

#include "electromaginitializer.h"
#include "utilities/constants.h"
#include "utilities/types.h"
#include "data/vecfield/vecfield.h"


class Electromag
{
public:
    Electromag();

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

    void setE(VecField const& E) { E_ = E; }
    void setB(VecField const& B) { B_ = B; }

    void setFields(ElectromagInitializer& initializer);


private:
    VecField E_;
    VecField B_;
};

#endif // ELECTROMAG_H
