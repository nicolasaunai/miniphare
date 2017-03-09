#ifndef MLMDINITIALIZERFACTORY_H
#define MLMDINITIALIZERFACTORY_H

#include "Initializer/ionsinitializer.h"
#include "Initializer/electromaginitializer.h"
#include "Initializer/ohminitializer.h"
#include "Initializer/solverinitializer.h"
#include "Initializer/initializerfactory.h"

#include "AMR/patch.h"





/* ----------------------------------------------------------------------------
                      Interpolation from a coarse patch
                      to a refined patch
   ---------------------------------------------------------------------------- */
void fieldAtRefinedNodes1D( Interpolator const& interp,
                            GridLayout const & coarseLayout,
                            VecField const & Ecoarse , VecField const & Bcoarse,
                            GridLayout const & refinedLayout,
                            VecField & Erefined , VecField & Brefined ) ;



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

    GridLayout refinedLayout_;
    double dt_;

    // this interpolator is used to initialize fields
    // on a refined patch
    uint32 interpolationOrder_;

    PRA buildPRA_  ( GridLayout const & layout ) const ;
    PRA buildPRA1D_( GridLayout const & layout ) const ;
    PRA buildPRA2D_( GridLayout const & layout ) const ;
    PRA buildPRA3D_( GridLayout const & layout ) const ;

    void preCompute_( GridLayout const & layout,
                        Direction direction,
                        uint32 nbrMaxGhost,
                        uint32 & ix0_in , uint32 & ix1_in ,
                        uint32 & ix0_out, uint32 & ix1_out,
                        double & x0_in  , double & x1_in ,
                        double & x0_out , double & x1_out ) const ;

    GridLayout buildPRABoundaryLayout_(
            PRA const & refinedPRA, uint32 idim ) const ;

public:
    MLMDInitializerFactory(std::shared_ptr<Patch> parentPatch,
                           Box newPatchCoords,
                           GridLayout refinedLayout ) // uint32 refinement
        : parentPatch_{parentPatch}, newPatchCoords_{newPatchCoords},
          refinedLayout_{ refinedLayout }, interpolationOrder_{2}
          // layout_{ parentPatch->layout().subLayout(newPatchCoords, refinement) }
    { }

    virtual std::unique_ptr<IonsInitializer> createIonsInitializer() const override;
    virtual std::unique_ptr<ElectromagInitializer> createElectromagInitializer() const  override;
    virtual std::unique_ptr<SolverInitializer> createSolverInitializer() const  override;
    virtual std::unique_ptr<OhmInitializer> createOhmInitializer() const override;
    virtual std::unique_ptr<BoundaryCondition> createBoundaryCondition() const override;

    virtual Box getRefinedBox() const override;
    virtual GridLayout const& gridLayout() const override;
    virtual double timeStep() const override;
};

#endif // MLMDINITIALIZERFACTORY_H
