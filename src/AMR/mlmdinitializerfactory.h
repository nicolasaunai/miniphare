#ifndef MLMDINITIALIZERFACTORY_H
#define MLMDINITIALIZERFACTORY_H

#include "Initializer/ionsinitializer.h"
#include "Initializer/electromaginitializer.h"
#include "Initializer/ohminitializer.h"
#include "Initializer/solverinitializer.h"
#include "Initializer/initializerfactory.h"

#include "AMR/patch.h"
#include "AMR/particleselector.h"





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

    const std::vector<uint32>  interpolationOrders_ ;
    const std::string pusher_ ;

    void buildIonsInitializer_( IonsInitializer & ionInit,
                                    ParticleSelector const & selector ) const ;

    uint32 PRAHalfWidth_( GridLayout const & layout ) const ;

    PRA buildPRA_  ( GridLayout const & layout ) const ;
    PRA buildPRA1D_( GridLayout const & layout ) const ;
    PRA buildPRA2D_( GridLayout const & layout ) const ;
    PRA buildPRA3D_( GridLayout const & layout ) const ;

    void definePRA1Dlimits_( GridLayout const & layout,
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
                           Box const & newPatchCoords,
                           GridLayout const & refinedLayout,
                           std::vector<uint32> const & orders,
                           std::string const & pusher )
        : parentPatch_{parentPatch}, newPatchCoords_{newPatchCoords},
          refinedLayout_{ refinedLayout },
          interpolationOrders_{orders}, pusher_{pusher} {}

    virtual std::unique_ptr<IonsInitializer> createIonsInitializer() const override;
    virtual std::unique_ptr<ElectromagInitializer> createElectromagInitializer() const  override;
    virtual std::unique_ptr<SolverInitializer> createSolverInitializer() const  override;
    virtual std::unique_ptr<OhmInitializer> createOhmInitializer() const override;
    virtual std::unique_ptr<BoundaryCondition> createBoundaryCondition() const override;

    virtual Box getBox() const override { return refinedLayout_.getBox(); }
    virtual GridLayout const& gridLayout() const override { return refinedLayout_; }
    virtual double timeStep() const override { return dt_; }
    virtual std::string const & pusher() const override { return pusher_; }

    virtual std::vector<uint32> const &
    interpolationOrders() const override { return interpolationOrders_; }
};

#endif // MLMDINITIALIZERFACTORY_H



