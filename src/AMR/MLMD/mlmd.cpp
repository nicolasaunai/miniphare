
#include <cmath>

#include "Solver/solver.h"

#include "AMR/Hierarchy/hierarchy.h"
#include "AMR/MLMD/mlmd.h"
#include "AMR/patch.h"
#include "AMR/patchdata.h"




MLMD::MLMD(InitializerFactory const& initFactory)
    : baseLayout_{GridLayout{initFactory.gridLayout()}}
{
    patchInfos_.interpOrders    = initFactory.interpolationOrders();
    patchInfos_.pusher          = initFactory.pusher();
    patchInfos_.splitStrategies = initFactory.splittingStrategies();
    patchInfos_.refinementRatio = 2;

    patchInfos_.userTimeStep = initFactory.timeStep();

    // will probably have to change the way objects are initialized.
    // if we want, at some point, start from an already existing hierarchy
    // (in case of restart for e.g.

    patchInfos_.fakeStratIteration     = {0}; // 0, 1
    patchInfos_.fakeStratLevelToRefine = {0}; // 0, 1
    patchInfos_.fakeStratPatchToRefine = {0}; // 0, 0
}




void MLMD::initializeRootLevel(Hierarchy& patchHierarchy)
{
    std::cout << "building root level...";
    Patch& rootLevel = patchHierarchy.root();
    rootLevel.init();
}


/**
 * @brief MLMD::evolveFullDomain contains the main operations
 * achieved in one computational cycle
 * For the moment:
 * - we evolve fields and particles
 * - we evaluate the consistency of the refinement with the physical processes
 * - if necessary the hierarchy is updated with new patches
 * - ...
 *
 * Diagnostics will be added soon
 *
 *
 */
void MLMD::evolveFullDomain(Hierarchy& hierarchy, uint32 iter)
{
    // evolve fields and particle for a time step
    evolvePlasma_(hierarchy, patchInfos_.refinementRatio);

    // build RefinementAnalyser here
    RefinementAnalyser analyser{patchInfos_.fakeStratIteration, patchInfos_.fakeStratLevelToRefine,
                                patchInfos_.fakeStratPatchToRefine};

    // Here, AMR patches will say whether they need refinement
    // the ouput of this method is used by updateHierarchy()
    // Note for later: will probably not be called every time step.
    std::vector<std::vector<RefinementInfo>> refinementTable = hierarchy.evaluateRefinementNeed(
        patchInfos_.refinementRatio, baseLayout_, analyser, iter);

    // New patches are created here if necessary
    // it depends on evaluateHierarchy()
    hierarchy.refine(refinementTable, patchInfos_);
}



/**
 * @brief Hierarchy::evolveHierarchy
 *
 * evolve fields and particle for a time step
 *
 */
void MLMD::evolvePlasma_(Hierarchy& hierarchy, uint32 refineRatio)
{
    Patch& root = hierarchy.root();

    uint32 ilevel   = 0;
    uint32 nbrSteps = 1;

    if (root.hasChildren())
    {
        recursivEvolve_(root, ilevel, refineRatio, nbrSteps);
    }
    else
    {
        evolve_(root, 1);
    }
}


void MLMD::evolve_(Patch& patch, uint32 nbrSteps)
{
    std::cout << "Evolve" << std::endl;

    std::cout << "dt = " << patch.timeStep() << std::endl;

    for (uint32 itime = 0; itime < nbrSteps; ++itime)
    {
        std::cout << "solveStep = " << itime + 1 << " / " << nbrSteps << std::endl;

        patch.solveStep();

        // MLMD mecanism step 4
        interpolateFieldBCInTime_(patch);
    }
}



void evaluateNbrSteps(uint32 refineRatio, uint32& nbrSteps_new)
{
    std::cout << "evaluateNbrSteps(...)" << std::endl;

    uint32 CFL_coef = refineRatio * refineRatio;

    // CFL constraint
    // dt < dx**2
    nbrSteps_new = CFL_coef;

    // dt_new = dt / CFL_coef;
}


/**
 * @brief recursivEvolve
 *
 *
 * @param patch
 * @param ilevel
 */
void MLMD::recursivEvolve_(Patch& patch, uint32 ilevel, uint32 refineRatio, uint32 nbrSteps)
{
    std::cout << "recursivEvolve:"
              << "dt = " << patch.timeStep() << ", nbrSteps = " << nbrSteps << std::endl;

    uint32 nbrChildren = patch.nbrChildren();

    std::cout << "Level = " << ilevel << std::endl;
    std::cout << "nbrChildren = " << nbrChildren << std::endl;

    if (nbrChildren > 0)
    {
        for (uint32 istep = 0; istep < nbrSteps; istep++)
        {
            std::cout << "Level = " << ilevel << std::endl;
            std::cout << "istep/nbrSteps = " << istep + 1 << " / " << nbrSteps << std::endl;

            // MLMD mecanism step 1
            // loop over children patches
            // trigger: Part BC at tn
            for (uint32 ik = 0; ik < nbrChildren; ik++)
            {
                std::cout << "\nchild/nbrChildren = " << ik + 1 << " / " << nbrChildren
                          << std::endl;

                auto patchPtr = patch.children(ik);

                // MLMD mecanism step 1
                manageParticlesInGhostDomain_(*patchPtr);
            }

            // MLMD mecanism step 2
            evolve_(patch, 1);

            uint32 nbrSteps_new = 0;
            evaluateNbrSteps(refineRatio, nbrSteps_new);

            for (uint32 ik = 0; ik < nbrChildren; ik++)
            {
                std::cout << "child/nbrChildren = " << ik + 1 << " / " << nbrChildren << std::endl;

                auto patchPtr = patch.children(ik);

                // MLMD mecanism step 3
                sendCorrectedFieldsToChildrenPRA_(patch);

                recursivEvolve_(*patchPtr, ilevel + 1, refineRatio, nbrSteps_new);
            }
        }
    }
    else
    {
        std::cout << "Level = " << ilevel << std::endl;

        // MLMD mecanism step 1
        // warning part BC at tn + dt(parent_patch)
        //        patch.initParticlesInGhostDomain() ;

        std::cout << "Patch total population = " << patch.population() << std::endl;

        // MLMD mecanism step 2 (times nbrSteps) on a patch of the finest level
        evolve_(patch, nbrSteps);

        // MLMD mecanism step 5
        updateFieldsWithRefinedSolutions_(patch);
    }
}



void MLMD::manageParticlesInGhostDomain_(Patch& patch)
{
}


void MLMD::sendCorrectedFieldsToChildrenPRA_(Patch& patch)
{
    std::cout << "sendCorrectedFieldsToChildrenPRA" << std::endl;
}


void MLMD::interpolateFieldBCInTime_(Patch& patch)
{
    std::cout << "interpolateFieldBCInTime" << std::endl;
}


void MLMD::updateFieldsWithRefinedSolutions_(Patch& patch)
{
    std::cout << "updateFieldsWithRefinedSolutions" << std::endl;
}
