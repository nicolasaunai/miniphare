#ifndef GCABOUNDARYCONDITION_H
#define GCABOUNDARYCONDITION_H

#include <vector>


#include "core/BoundaryConditions/boundary_conditions.h"
#include "data/Plasmas/particles.h"
#include "data/grid/gridlayout.h"
#include "data/vecfield/vecfield.h"



/**
 * @brief The GCABoundaryCondition class is a special type of BoundaryCondition
 * used by PatchBoundaryCondition::applyIncomingParticleBC(...)
 *
 * It is required by the pusher used to move particles in the GCA
 *
 * This boundary condition only defines applyOutgoingParticleBC(...)
 *
 */
class GCABoundaryCondition : public BoundaryCondition
{
private:
    GridLayout patchLayout_;
    GridLayout GCALayout_;

    std::vector<Particle> incomingParticleBucket_;

public:
    GCABoundaryCondition(GridLayout const& patchLayout, GridLayout const& GCALayout);


    virtual void applyMagneticBC(VecField& B) const override;
    virtual void applyElectricBC(VecField& E) const override;
    virtual void applyCurrentBC(VecField& J) const override;
    virtual void applyDensityBC(Field& N) const override;
    virtual void applyFluxBC(Ions& ions) const override;

    virtual void applyOutgoingParticleBC(std::vector<Particle>& GCAparticles,
                                         LeavingParticles const& leavingParticles) override;

    virtual void applyIncomingParticleBC(std::vector<Particle>& particles,
                                         std::string const& pusher, double const& dt,
                                         std::string const& species, bool update) const override;

    std::vector<Particle>& incomingBucket() { return incomingParticleBucket_; }

    void resetBucket() { incomingParticleBucket_.clear(); }

    virtual ~GCABoundaryCondition();
};


#endif // GCABOUNDARYCONDITION_H
