#ifndef PRABOUNDARYCONDITION_H
#define PRABOUNDARYCONDITION_H

#include <vector>


#include "BoundaryConditions/boundary_conditions.h"
#include "Plasmas/particles.h"
#include "grid/gridlayout.h"
#include "vecfield/vecfield.h"



/**
 * @brief The PRABoundaryCondition class is a special type of BoundaryCondition
 * used by PatchBoundaryCondition::applyIncomingParticleBC(...)
 *
 * It is required by the pusher used to move particles in the PRA
 *
 * This boundary condition only defines applyOutgoingParticleBC(...)
 *
 */
class PRABoundaryCondition : public BoundaryCondition
{
private:
    GridLayout patchLayout_;
    GridLayout PRALayout_;

    std::vector<Particle> incomingParticleBucket_;

public:
    PRABoundaryCondition(GridLayout const& patchLayout, GridLayout const& PRALayout);


    virtual void applyMagneticBC(VecField& B) const override;
    virtual void applyElectricBC(VecField& E) const override;
    virtual void applyCurrentBC(VecField& J) const override;
    virtual void applyDensityBC(Field& N) const override;
    virtual void applyFluxBC(Ions& ions) const override;

    virtual void applyOutgoingParticleBC(std::vector<Particle>& PRAparticles,
                                         LeavingParticles const& leavingParticles) override;

    virtual void applyIncomingParticleBC(std::vector<Particle>& particles,
                                         std::string const& pusher, double const& dt,
                                         std::string const& species) const override;

    std::vector<Particle>& incomingBucket() { return incomingParticleBucket_; }

    void resetBucket() { incomingParticleBucket_.clear(); }

    virtual ~PRABoundaryCondition();
};


#endif // PRABOUNDARYCONDITION_H
