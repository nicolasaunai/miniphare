#include "electromagdiagnostic.h"




void ElectromagDiagnostic::compute(PatchData const& patchData , GridLayout const& layout)
{
    std::cout << "computing EM diags" << std::endl;
    Electromag const& em = patchData.EMfields();

    addVecField_(em.getE(), layout, Eqty_);
    addVecField_(em.getB(), layout, Bqty_);
}
