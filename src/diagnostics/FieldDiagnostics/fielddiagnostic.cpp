

#include "fielddiagnostic.h"



/**
 * @brief compute loops over a Hierarchy and for each Patch call the abstract
 * FieldDiagnosticComputeStrategy::compute() method. From this methods it gets
 * a FieldPack that is added to the FieldPack vector.
 */
void FieldDiagnostic::compute(Hierarchy const& hierarchy)
{
    if (strategy_ == nullptr)
        throw std::runtime_error("FieldDiagnostic Error - No compute Strategy");


    auto const& patchTable = hierarchy.patchTable();
    for (auto const& level : patchTable)
    {
        for (auto const& patch : level)
        {
            packs_.push_back(strategy_->compute(*patch));
        }
    }
}


void FieldDiagnostic::flushPacks()
{
    std::vector<FieldPack> tmp;
    std::swap(tmp, packs_);
}
