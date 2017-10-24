#ifndef DOMAINBOUNDARYCONDITION_H
#define DOMAINBOUNDARYCONDITION_H

#include <vector>


#include "boundary.h"
#include "boundary_conditions.h"

#include "data/Plasmas/particles.h"
#include "data/grid/gridlayout.h"
#include "data/vecfield/vecfield.h"



/**
 * @brief The DomainBoundaryCondition class is a special type of BoundaryCondition
 * that applies only to the physical domain (root patch)
 *
 * This class does NOT KNOW which kind of domain boundary condition there is at each
 * boundary. It defines the pure virtual functions of the BoundaryCondition interface
 * The methods here are just looping on a collection of Boundary objects (as many
 * as the patch has boundaries) and call each Boundary's methods.
 */
class DomainBoundaryCondition : public BoundaryCondition
{
private:
    // these boundaries know what they are : periodic, etc.
    std::vector<std::unique_ptr<Boundary>> boundaries_;
    GridLayout layout_;

public:
    //! says what kind of boundary each boundary is
    using BoundaryInfo = std::pair<Edge, BoundaryType>;
    DomainBoundaryCondition(GridLayout layout, std::vector<BoundaryInfo> boundaryInfos);


    virtual void applyMagneticBC(VecField& B) const override;
    virtual void applyElectricBC(VecField& E) const override;
    virtual void applyCurrentBC(VecField& J) const override;
    virtual void applyDensityBC(Field& N) const override;
    virtual void applyFluxBC(Ions& ions) const override;

    virtual void applyOutgoingParticleBC(std::vector<Particle>& particleArray,
                                         LeavingParticles const& leavingParticles) override;

    virtual void applyIncomingParticleBC(std::vector<Particle>& particles,
                                         std::string const& pusher, double const& dt,
                                         std::string const& species) const override;

    virtual ~DomainBoundaryCondition();
};


#endif // DOMAINBOUNDARYCONDITION_H
