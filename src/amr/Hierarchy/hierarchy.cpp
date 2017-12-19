
#include <cmath>
#include <memory>
#include <tuple>

#include "data/grid/gridlayout.h"

#include "hierarchy.h"
#include "mlmdinitializerfactory.h"
#include "patch.h"
#include "refinmentanalyser.h"

#include "amr/Splitting/splittingstrategy.h"

#include "utilities/box.h"
#include <utilities/print/outputs.h>




/**
 * @brief Hierarchy::evaluateRefinementNeed analyses the Hierarchy and return
 * a vector of RefinementInfo for each Level.
 */
std::vector<std::vector<RefinementInfo>>
Hierarchy::evaluateRefinementNeed(uint32 refineRatio, GridLayout const& baseLayout,
                                  RefinementAnalyser& analyser, uint32 iter)
{
    std::vector<std::vector<RefinementInfo>> refinementTable;
    uint32 nbrLevels = static_cast<uint32>(patchTable_.size());

    for (uint32 iLevel = 0; iLevel < nbrLevels; iLevel++)
    {
        std::vector<std::shared_ptr<Patch>> const& patchesAtLevel = patchTable_[iLevel];

        std::vector<RefinementInfo> refineInfos;
        for (uint32 iPatch = 0; iPatch < patchesAtLevel.size(); ++iPatch)
        {
            std::shared_ptr<Patch> const& patch = patchesAtLevel[iPatch];

            // if the patch has to be refined we
            // store a reference
            // for further use
            if (analyser.refinementNeeded(iter, iLevel, iPatch))
            {
                analyser.refine(*patch);

                std::vector<Box> const& refinedList = analyser.refinedDomains();

                // TODO: design might change
                // We could decide to build a refinementVector
                // directly in analyser.refinedDomains()
                for (Box const& domain : refinedList)
                {
                    RefinementInfo refine{patch, domain, iLevel + 1, refineRatio, baseLayout};
                    refineInfos.push_back(std::move(refine));
                }
            } // end refinement need
        }     // end patch loop

        if (refineInfos.size() > 0)
            refinementTable.push_back(refineInfos);

    } // end loop on levels

    return refinementTable;
}




/**
 * @brief Hierarchy::updateHierarchy
 *
 * New patches are created here depending on the content
 * of refinementTable (and layoutTable)
 * These two tables have the same number of elements
 *
 * @param refinementTable
 * @param layoutTable
 */
void Hierarchy::refine(std::vector<std::vector<RefinementInfo>> const& refinementTable,
                       PatchInfos const& patchInfo)
{
    uint32 nbrLevels = static_cast<uint32>(refinementTable.size());

    for (uint32 iLevel = 0; iLevel < nbrLevels; ++iLevel)
    {
        uint32 nbrPatches = static_cast<uint32>(refinementTable[iLevel].size());

        for (uint32 iPatch = 0; iPatch < nbrPatches; ++iPatch)
        {
            RefinementInfo const& refineInfo = refinementTable[iLevel][iPatch];

            std::shared_ptr<Patch> newPatch{nullptr};

            // create new Patch and update Hierarchy
            newPatch = addNewPatch(refineInfo, patchInfo);

            // trigger initialization of the patch content
            newPatch->init();
        }
    } // end level loop
}




/**
 * @brief Hierarchy::addNewPatch adds a patch to the Hierarchy
 * The new Patch is built from a MLMDInitializerFactory
 */
std::shared_ptr<Patch> Hierarchy::addNewPatch(RefinementInfo const& refineInfo,
                                              PatchInfos const& patchInfo)
{
    Logger::Debug << "\t \t - adding new patch\n";
    Logger::Debug.flush();

    std::shared_ptr<Patch> coarsePatch = refineInfo.parentPatch;
    Box refinedBox                     = refineInfo.refinedDomain;
    uint32 refinedLevel                = refineInfo.level;
    GridLayout refinedLayout           = buildLayout_(refineInfo);

    uint32 RF = refineInfo.refinementRatio;

    double dt_patch = patchInfo.userTimeStep / std::pow(RF, 2 * refinedLevel);

    // we need to build a factory for PatchData to be built
    std::unique_ptr<InitializerFactory> factory{
        new MLMDInitializerFactory(coarsePatch, refinedBox, refinedLayout, patchInfo, dt_patch)};

    // create a new patch, attach it to the parent patch and updated the hierarchy
    Patch theNewPatch{refinedBox, dt_patch, refinedLayout, PatchData{*factory}};
    std::shared_ptr<Patch> patchPtr = std::make_shared<Patch>(std::move(theNewPatch));
    coarsePatch->addChild(patchPtr);

    if (patchTable_.size() <= refinedLevel)
    {
        patchTable_.push_back({});
    }

    patchTable_[refinedLevel].push_back(patchPtr);

    return patchPtr;
}




/**
 * @brief Hierarchy::buildLayout_ is in charge of creating a GridLayout
 * using a RefinementInfo object
 *
 * This GridLayout will be directly used by addNewPatch(...) to
 * build the new patch
 */
GridLayout Hierarchy::buildLayout_(RefinementInfo const& info)
{
    Box area     = info.refinedDomain;
    uint32 level = info.level;

    uint32 refineRatio   = info.refinementRatio;
    GridLayout const& L0 = info.baseLayout;

    // TODO: return the adequate GridLayout given newBox information
    // new spatial step sizes
    double dx = L0.dx() / std::pow(refineRatio, level);
    double dy = L0.dy() / std::pow(refineRatio, level);
    double dz = L0.dz() / std::pow(refineRatio, level);

    // cell numbers
    uint32 nbx = static_cast<uint32>(std::round((area.x1 - area.x0) / dx));
    uint32 nby = static_cast<uint32>(std::round((area.y1 - area.y0) / dy));
    uint32 nbz = static_cast<uint32>(std::round((area.z1 - area.z0) / dz));

    // we create the layout of a new patch
    // and store it
    return GridLayout({{dx, dy, dz}}, {{nbx, nby, nbz}}, L0.nbDimensions(), L0.layoutName(),
                      Point{area.x0, area.y0, area.z0}, L0.order());
}
