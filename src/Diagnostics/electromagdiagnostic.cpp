
#include <string>
#include <sstream>

#include "electromagdiagnostic.h"




void ElectricDiagnostic::compute(Hierarchy const& hierarchy )
{
    std::cout << "computing Electric diags" << std::endl;
    std::vector< std::vector< std::shared_ptr<Patch> > > const& patchTable = hierarchy.patchTable();


    for (auto const& level : patchTable)
    {
        for (auto const& patch : level)
        {
            PatchData const& patchData = patch->data();
            Electromag const& em = patchData.EMfields();
            GridLayout const& layout = patch->layout();

            std::stringstream ss;
            ss << patch->getID();
            std::string patchID{ ss.str() };
            addVecField_("patch_"+ patchID + "_", em.getE(), layout);
        }
    }
}





void MagneticDiagnostic::compute(Hierarchy const& hierarchy )
{
    std::cout << "computing Magnetic diags" << std::endl;
    std::vector< std::vector< std::shared_ptr<Patch> > > const& patchTable = hierarchy.patchTable();


    for (auto const& level : patchTable)
    {
        for (auto const& patch : level)
        {
            PatchData const& patchData = patch->data();
            Electromag const& em = patchData.EMfields();
            GridLayout const& layout = patch->layout();

            //std::string patchID{patch->getID()};
            std::string patchID{0};
            // TODO add patch ID to string
            addVecField_("patch_"+patchID+"_", em.getB(), layout);
        }
    }
}
