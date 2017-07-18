#ifndef FROZENBOUNDARY_H
#define FROZENBOUNDARY_H


#include "BoundaryConditions/boundary.h"
#include "vecfield/vecfield.h"



/**
 * @brief The FrozenBoundary is to be used by a Pusher object
 * in the Particle Repopulation Area (PRA)
 *
 * Only one interface method is needed in this context:
 * - applyOutgoingParticleBC(...)
 *
 */
class FrozenBoundary : public Boundary
{
private:
    Edge edge_;

public:
    FrozenBoundary(Edge edge)
        : edge_{edge}
    {
        std::cout << "creating frozen boundary\n";
    }

    virtual ~FrozenBoundary() = default;

    virtual void applyElectricBC(VecField& E, GridLayout const& layout) const override;
    virtual void applyMagneticBC(VecField& B, GridLayout const& layout) const override;
    virtual void applyCurrentBC(VecField& J, GridLayout const& layout) const override;
    virtual void applyDensityBC(Field& J, GridLayout const& layout) const override;
    virtual void applyBulkBC(VecField& Vi, GridLayout const& layout) const override;
    virtual void applyOutgoingParticleBC(std::vector<Particle>& particleArray,
                                         LeavingParticles const& leavingParticles) const override;
    virtual void applyIncomingParticleBC(BoundaryCondition& temporaryBC, Pusher& pusher,
                                         GridLayout const& patchLayout,
                                         std::vector<Particle>& patchParticles,
                                         uint32 iesp) override;
};


#endif // FROZENBOUNDARY_H
