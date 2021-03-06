
#include <cmath>

#include "core/Solver/solver.h"

#include "amr/Hierarchy/hierarchy.h"
#include "amr/MLMD/mlmd.h"
#include "amr/Patch/patch.h"
#include "amr/Patch/patchdata.h"
#include "amr/Refinement/coarsetorefinemesh.h"
#include "amr/Refinement/refinmentanalyser.h"
#include "core/Interpolator/particlemesh.h"
#include <utilities/print/outputs.h>




MLMD::MLMD(std::unique_ptr<MLMDInitializer> initializer)
    : baseLayout_{initializer->baseLayout}
    , patchInfos_{initializer->patchInfos}
    , mlmdInfos_{initializer->mlmdInfos}
{
}


void MLMD::initializeRootLevel(Hierarchy& patchHierarchy)
{
    Logger::Debug << "Building AMR root level\n";
    Logger::Debug.flush();
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
    RefinementAnalyser analyser{mlmdInfos_};

    // Here, AMR patches will say whether they need refinement
    // the ouput of this method is used by updateHierarchy()
    // Note for later: will probably not be called every time step.
    std::vector<std::vector<RefinementInfo>> refinementTable = hierarchy.evaluateRefinementNeed(
        patchInfos_.refinementRatio, baseLayout_, analyser, iter);

    // New patches are created here if necessary
    // it depends on evaluateHierarchy()
    Logger::Debug << "\t - Analizing domain for refinement\n";
    Logger::Debug.flush();
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
    Logger::Info << "\t - Patch evolution : " << nbrSteps << " steps with dt = " << patch.timeStep()
                 << "\n";
    Logger::Debug.flush();

    for (uint32 itime = 0; itime < nbrSteps; ++itime)
    {
        Logger::Info << "\t \t - solving step " << itime + 1 << " / " << nbrSteps << "\n";
        Logger::Info.flush();
        patch.data().solveStep();
        updateFreeEvolutionTime_(patch);
    }
}



void evaluateNbrSteps(uint32 refineRatio, uint32& nbrSteps_new)
{
    Logger::Debug << "evaluateNbrSteps(...)\n";
    Logger::Debug.flush();

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
    auto nbrChildren = patch.nbrChildren();

    Logger::Debug << "\t - recursivEvolve:"
                  << "dt(" << patch.timeStep() << ") nbrSteps = " << nbrSteps << "\n";
    Logger::Debug << "\t - Level = " << ilevel << "\n";
    Logger::Debug << "\t - nbrChildren = " << nbrChildren << "\n";
    Logger::Debug.flush();


    if (nbrChildren > 0)
    {
        for (uint32 istep = 0; istep < nbrSteps; istep++)
        {
            Logger::Debug << "\t - Level = " << ilevel << "\n";
            Logger::Debug << "\t - istep/nbrSteps = " << istep + 1 << " / " << nbrSteps << "\n";
            Logger::Debug.flush();

            // MLMD mecanism step 1
            // loop over children patches
            // trigger: Part BC at tn
            for (uint32 ik = 0; ik < nbrChildren; ik++)
            {
                Logger::Debug << "\t - child/nbrChildren = " << ik + 1 << " / " << nbrChildren
                              << "\n";
                Logger::Debug.flush();

                auto patchPtr = patch.children(ik);

                // MLMD mecanism step 1
                initGCAparticlesAndMoments_(*patchPtr);

                updateGCA_EMfields_(*patchPtr);
            }

            // MLMD mecanism step 2
            evolve_(patch, 1);

            uint32 nbrSteps_new = 0;
            evaluateNbrSteps(refineRatio, nbrSteps_new);

            for (uint32 ik = 0; ik < nbrChildren; ik++)
            {
                Logger::Debug << "\t - child/nbrChildren = " << ik + 1 << " / " << nbrChildren
                              << "\n";
                Logger::Debug.flush();

                auto patchPtr = patch.children(ik);

                // MLMD mecanism step 3
                sendCorrectedFieldsToChildrenGCA_(patch, *patchPtr);

                recursivEvolve_(*patchPtr, ilevel + 1, refineRatio, nbrSteps_new);
            }

            Logger::Debug << "\t - Level = " << ilevel << "\n";
            Logger::Debug << "\t - istep/nbrSteps = " << istep + 1 << " / " << nbrSteps << "\n";
            Logger::Debug.flush();

            // MLMD mecanism step 5
            updateFieldsWithRefinedSolutions_(patch);

            resetFreeEvolutionOfChildren_(patch);
        }
    }
    else
    {
        // MLMD mecanism step 2 (times nbrSteps) on a patch of the finest level
        evolve_(patch, nbrSteps);
    }
}



void MLMD::initGCAparticlesAndMoments_(Patch& patch)
{
    Logger::Debug << "manageParticlesInGhostDomain()\n";
    Logger::Debug.flush();

    BoundaryCondition* boundaryCond = patch.data().boundaryCondition();

    initGCAparticles_(boundaryCond);
    Logger::Debug << " GCA initialization: OK\n";
    Logger::Debug.flush();

    // for each species
    computeGCADensityAndFlux_(boundaryCond, patchInfos_.interpOrder);

    computeGCAChargeDensity_(boundaryCond);
    if (PatchBoundaryCondition* condition = dynamic_cast<PatchBoundaryCondition*>(boundaryCond))
        condition->applyDensityBC(patch.data().ions().rho());

    if (PatchBoundaryCondition* condition = dynamic_cast<PatchBoundaryCondition*>(boundaryCond))
        condition->applyFluxBC(patch.data().ions());
}



/**
 * @brief PatchData::initGCA will trigger the initialization of
 * the Particle Repopulation Area, contained in
 * boundaryCondition_ attribute
 *
 */
void MLMD::initGCAparticles_(BoundaryCondition* boundaryCondition)
{
    // we have to trigger loadParticles() method of Ions,
    // the latter will call loadParticles() of Species
    if (PatchBoundaryCondition* boundaryCond
        = dynamic_cast<PatchBoundaryCondition*>(boundaryCondition))
    {
        boundaryCond->initializeGCAparticles();
    }
}


void MLMD::computeGCADensityAndFlux_(BoundaryCondition* boundaryCondition, uint32 order)
{
    if (PatchBoundaryCondition* boundaryCond
        = dynamic_cast<PatchBoundaryCondition*>(boundaryCondition))
    {
        boundaryCond->computeGCADensityAndFlux(order);
    }
}


void MLMD::computeGCAChargeDensity_(BoundaryCondition* boundaryCondition)
{
    if (PatchBoundaryCondition* boundaryCond
        = dynamic_cast<PatchBoundaryCondition*>(boundaryCondition))
    {
        boundaryCond->computeGCAChargeDensity();
    }
}


/**
 * @brief MLMD::sendCorrectedFieldsToChildrenGCA_
 * This method fills the GCAs of a child patch
 * - it fills the corrected Electromag field
 *
 *
 */
void MLMD::sendCorrectedFieldsToChildrenGCA_(Patch const& parentPatch, Patch& childPatch)
{
    Logger::Debug << "sendCorrectedFieldsToChildrenGCA\n";
    Logger::Debug.flush();

    GridLayout const& parentLayout     = parentPatch.layout();
    Electromag const& parentElectromag = parentPatch.data().EMfields();

    BoundaryCondition* boundaryCondition = childPatch.data().boundaryCondition();

    if (PatchBoundaryCondition* patchCondition
        = dynamic_cast<PatchBoundaryCondition*>(boundaryCondition))
    {
        patchCondition->updateCorrectedEMfields(parentLayout, parentElectromag);
    }
}


void MLMD::updateGCA_EMfields_(Patch& childPatch)
{
    BoundaryCondition* boundaryCondition = childPatch.data().boundaryCondition();

    if (PatchBoundaryCondition* patchCondition
        = dynamic_cast<PatchBoundaryCondition*>(boundaryCondition))
    {
        patchCondition->updateEMfields();
    }
}




void MLMD::updateFieldsWithRefinedSolutions_(Patch& parentPatch)
{
    Logger::Debug << "updateFieldsWithRefinedSolutions\n";
    Logger::Debug.flush();

    // get EM vecfield of parent patch
    VecField& E_parent = parentPatch.data().EMfields().getE();
    VecField& B_parent = parentPatch.data().EMfields().getB();

    GridLayout const& parentLayout = parentPatch.layout();

    for (uint32 ik = 0; ik < parentPatch.nbrChildren(); ++ik)
    {
        auto patchPtr = parentPatch.children(ik);

        GridLayout const& childLayout = patchPtr->layout();

        // get EM vecfield of child patch
        VecField const& E_child = patchPtr->data().EMfields().getE();
        VecField const& B_child = patchPtr->data().EMfields().getB();

        // loop on the electric field components
        addChildVecFieldToPatch(E_parent, E_child, parentLayout, childLayout);

        // loop on the magnetic field components
        addChildVecFieldToPatch(B_parent, B_child, parentLayout, childLayout);
    }
}



void MLMD::addChildVecFieldToPatch(VecField& parentVf, VecField const& childVf,
                                   GridLayout const& parentLayout, GridLayout const& childLayout)
{
    // loop over fields
    for (uint32 icompo = 0; icompo < NBR_COMPO; ++icompo)
    {
        Field& parentField      = parentVf.component(icompo);
        Field const& childField = childVf.component(icompo);

        switch (childLayout.nbDimensions())
        {
            case 1:
                addChildFieldToPatch1D_(parentField, childField, parentLayout, childLayout);
                break;

            case 2:
                addChildFieldToPatch2D_(parentField, childField, parentLayout, childLayout);
                break;

            case 3:
                addChildFieldToPatch3D_(parentField, childField, parentLayout, childLayout);
                break;
        }
    }
}


std::array<uint32, 3> MLMD::getStartIndexes_(GridLayout const& childLayout, Field const& childField)
{
    return {{childLayout.physicalStartIndex(childField, Direction::X),
             childLayout.physicalStartIndex(childField, Direction::Y),
             childLayout.physicalStartIndex(childField, Direction::Z)}};
}


std::array<uint32, 3> MLMD::getEndIndexes_(GridLayout const& childLayout, Field const& childField)
{
    return {{childLayout.physicalEndIndex(childField, Direction::X),
             childLayout.physicalEndIndex(childField, Direction::Y),
             childLayout.physicalEndIndex(childField, Direction::Z)}};
}



void MLMD::addChildFieldToPatch1D_(Field& parentField, Field const& childField,
                                   GridLayout const& parentLayout, GridLayout const& childLayout)
{
    // we need extremal coordinates of the field defined on the child patch
    std::array<uint32, 3> iStart_child = getStartIndexes_(childLayout, childField);
    std::array<uint32, 3> iEnd_child   = getEndIndexes_(childLayout, childField);

    QtyCentering fieldCtr = childLayout.fieldCentering(childField, Direction::X);

    // get absolute coordinates
    Point p_iStart_child = childLayout.fieldNodeCoordinates(
        childField, childLayout.origin(), iStart_child[0], iStart_child[1], iStart_child[2]);

    Point p_iEnd_child = childLayout.fieldNodeCoordinates(
        childField, childLayout.origin(), iEnd_child[0], iEnd_child[1], iEnd_child[2]);

    uint32 iStart_OnParent = 0;
    uint32 iEnd_OnParent   = 0;
    // get corresponding closest grid nodes on the parent layout
    // we get: iStart_parent, iEnd_parent
    getClosestGridNode1D(p_iStart_child, parentLayout, fieldCtr, iStart_OnParent);
    getClosestGridNode1D(p_iEnd_child, parentLayout, fieldCtr, iEnd_OnParent);

    uint32 nbrNodes = iEnd_OnParent - iStart_OnParent + 1;

    // we build a temporary Field aligned with the
    // target field on the parent patch
    Field alignedChildField{
        buildAlignedChildField1D(childField, childLayout, nbrNodes, patchInfos_.refinementRatio)};

    // Add childField to the parentField
    // and then average
    uint32 iChild = 0;
    for (uint32 ik = iStart_OnParent; ik <= iEnd_OnParent; ++ik)
    {
        parentField(ik) = (parentField(ik) + alignedChildField(iChild)) / 2.;
        ++iChild;
    }
}


void MLMD::addChildFieldToPatch2D_(Field& parentField, Field const& childField,
                                   GridLayout const& parentLayout, GridLayout const& childLayout)
{
    (void)parentField;
    (void)childField;
    (void)parentLayout;
    (void)childLayout;
}


void MLMD::addChildFieldToPatch3D_(Field& parentField, Field const& childField,
                                   GridLayout const& parentLayout, GridLayout const& childLayout)
{
    (void)parentField;
    (void)childField;
    (void)parentLayout;
    (void)childLayout;
}



void MLMD::resetFreeEvolutionOfChildren_(Patch& parentPatch)
{
    for (uint32 ik = 0; ik < parentPatch.nbrChildren(); ++ik)
    {
        auto patchPtr = parentPatch.children(ik);

        resetFreeEvolutionTime_(*patchPtr);
    }
}


void MLMD::resetFreeEvolutionTime_(Patch& childPatch)
{
    BoundaryCondition* bc = childPatch.data().boundaryCondition();
    if (PatchBoundaryCondition* patchBC = dynamic_cast<PatchBoundaryCondition*>(bc))
    {
        patchBC->resetFreeEvolutionTime();
    }
}



void MLMD::updateFreeEvolutionTime_(Patch& patch)
{
    BoundaryCondition* bc = patch.data().boundaryCondition();
    if (PatchBoundaryCondition* patchBC = dynamic_cast<PatchBoundaryCondition*>(bc))
    {
        patchBC->updateFreeEvolutionTime(patch.timeStep());
    }
}
