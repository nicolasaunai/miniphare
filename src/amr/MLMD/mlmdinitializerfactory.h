#ifndef MLMDINITIALIZERFACTORY_H
#define MLMDINITIALIZERFACTORY_H

#include <array>

#include "data/Electromag/electromaginitializer.h"
#include "data/Plasmas/ionsinitializer.h"

#include "initializer/initializerfactory.h"

#include "initializer/ohminitializer.h"
#include "initializer/solverinitializer.h"

#include "amr/MLMD/pra.h"
#include "amr/Patch/patch.h"
#include "amr/Patch/patchinfo.h"
#include "amr/Splitting/splittingstrategy.h"

#include "utilities/particleselector.h"



/**
 * @brief The MLMDInitializerFactory class creates objects to initialize MLMD patches
 *
 * The factory can be used either for Patch initialization or Patch boundary conditions.
 * Patch initialization need an MLMD factory because fields, ions, Solver, etc. of a refined
 * patch are initialized from the Parent Patch and not from user inputs.
 * Methods such as createIonInitializer() and createElectromagInitializer()
 * can be used to initialier patch BCs.
 */
class MLMDInitializerFactory : public InitializerFactory
{
private:
    std::shared_ptr<Patch> parentPatch_;

    Box newPatchCoords_;
    GridLayout refinedLayout_;

    uint32 refinementRatio_;
    uint32 interpolationOrder_;
    std::string pusher_;
    std::vector<std::string> splitMethods_;

    double dt_;

    void buildIonsInitializer_(IonsInitializer& ionInit, std::shared_ptr<ParticleSelector> selector,
                               GridLayout const& targetLayout) const;

    GridLayout getExtendedLayout_(GridLayout const& praLayout) const;

public:
    MLMDInitializerFactory(std::shared_ptr<Patch> parentPatch, Box const& newPatchCoords,
                           GridLayout const& refinedLayout, PatchInfo const& patchInfo,
                           double dt_patch)
        : parentPatch_{parentPatch}
        , newPatchCoords_{newPatchCoords}
        , refinedLayout_{refinedLayout}
        , refinementRatio_{patchInfo.refinementRatio}
        , interpolationOrder_{patchInfo.interpOrder}
        , pusher_{patchInfo.pusher}
        , splitMethods_{patchInfo.splitStrategies}
        , dt_{dt_patch}
    {
    }

    virtual Box getBox() const override { return refinedLayout_.getBox(); }
    virtual GridLayout const& gridLayout() const override { return refinedLayout_; }
    virtual double timeStep() const override { return dt_; }
    virtual std::string const& pusher() const override { return pusher_; }
    virtual uint32 interpolationOrder() const override { return interpolationOrder_; }
    virtual std::vector<std::string> const& splittingStrategies() const override
    {
        return splitMethods_;
    }


    virtual std::unique_ptr<IonsInitializer> createIonsInitializer() const override;
    virtual std::unique_ptr<ElectromagInitializer> createElectromagInitializer() const override;
    virtual std::unique_ptr<SolverInitializer> createSolverInitializer() const override;
    virtual std::unique_ptr<BoundaryCondition> createBoundaryCondition() const override;
};

#endif // MLMDINITIALIZERFACTORY_H
