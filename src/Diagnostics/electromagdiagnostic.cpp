
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
            Field const& Ex = em.getEi(0);
            Field const& Ey = em.getEi(1);
            Field const& Ez = em.getEi(2);
            GridLayout const& layout = patch->layout();

            std::stringstream ss;
            ss << patch->getID();
            std::string patchID{ ss.str() };
            addField_("patch_"+ patchID + "_", Ex, layout);
            addField_("patch_"+ patchID + "_", Ey, layout);
            addField_("patch_"+ patchID + "_", Ez, layout);
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
            Field const& Bx = em.getBi(0);
            Field const& By = em.getBi(1);
            Field const& Bz = em.getBi(2);
            GridLayout const& layout = patch->layout();

            std::stringstream ss;
            ss << patch->getID();
            std::string patchID{ ss.str() };
            addField_("patch_"+ patchID + "_", Bx, layout);
            addField_("patch_"+ patchID + "_", By, layout);
            addField_("patch_"+ patchID + "_", Bz, layout);
        }
    }
}
