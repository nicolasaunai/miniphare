#ifndef MLMDINITIALIZERFACTORY_H
#define MLMDINITIALIZERFACTORY_H

#include "Initializer/ionsinitializer.h"
#include "Initializer/electromaginitializer.h"
#include "Initializer/ohminitializer.h"
#include "Initializer/solverinitializer.h"
#include "Initializer/initializerfactory.h"

#include "AMR/patch.h"



std::array<double,3>  zeroMagneticField(double x, double y, double z) ;

std::array<double,3>  zeroElectricField(double x, double y, double z) ;


/* ----------------------------------------------------------------------------
                      Interpolation from a coarse patch
                      to a refined patch
   ---------------------------------------------------------------------------- */
void fieldAtRefinedNodes1D(Interpolator const& interp,
                           GridLayout const & parentLayout,
                           VecField const & Eparent , VecField const & Bparent,
                           GridLayout const & newLayout,
                           VecField & newE , VecField & newB);



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
    Box newPatchCoords_ ;

    GridLayout layout_;
    double dt_;

    // this interpolator is used to initialize fields
    // on a refined patch
    uint32 interpolationOrder_;

public:
    MLMDInitializerFactory(std::shared_ptr<Patch> parentPatch,
                           Box newPatchCoords,
                           GridLayout newLayout ) // uint32 refinement
        : parentPatch_{parentPatch}, newPatchCoords_{newPatchCoords},
          layout_{ newLayout }, interpolationOrder_{2}
          // layout_{ parentPatch->layout().subLayout(newPatchCoords, refinement) }
    { }

    virtual std::unique_ptr<IonsInitializer> createIonsInitializer() const override;
    virtual std::unique_ptr<ElectromagInitializer> createElectromagInitializer() const  override;
    virtual std::unique_ptr<SolverInitializer> createSolverInitializer() const  override;
    virtual std::unique_ptr<OhmInitializer> createOhmInitializer() const override;
    virtual std::unique_ptr<BoundaryCondition> createBoundaryCondition() const override;

    virtual Box getBox() const override;
    virtual GridLayout const& gridLayout() const override;
    virtual double timeStep() const override;
};

#endif // MLMDINITIALIZERFACTORY_H
