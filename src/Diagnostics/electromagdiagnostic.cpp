#include "electromagdiagnostic.h"




void ElectromagDiagnostic::compute(Hierarchy const& hierarchy )
{
    std::cout << "computing EM diags" << std::endl;
    std::vector< std::vector< std::shared_ptr<Patch> > > const& patchTable = hierarchy.patchTable();


    for (auto const& level : patchTable)
    {
        for (auto const& patch : level)
        {
            PatchData const& patchData = patch->data();
            Electromag const& em = patchData.EMfields();
            GridLayout const& layout = patch->layout();

            addVecField_(em.getE(), layout, Eqty_);
            addVecField_(em.getB(), layout, Bqty_);
        }
    }
}
