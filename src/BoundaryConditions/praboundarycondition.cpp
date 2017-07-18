
#include "praboundarycondition.h"




PRABoundaryCondition::PRABoundaryCondition(GridLayout const & patchLayout, GridLayout const & PRALayout)
    : patchLayout_{patchLayout},
      PRALayout_{PRALayout},
      incomingParticleBucket_{{}}
{

}



void PRABoundaryCondition::applyMagneticBC(VecField& B) const
{
    (void) B;
}


void PRABoundaryCondition::applyElectricBC(VecField& E) const
{
    (void) E;
}


void PRABoundaryCondition::applyCurrentBC(VecField& J) const
{
    (void) J;
}



void PRABoundaryCondition::applyDensityBC(Field& Ni) const
{
    (void) Ni;
}


void PRABoundaryCondition::applyBulkBC(VecField& Vi) const
{
    (void) Vi;
}



void PRABoundaryCondition::applyOutgoingParticleBC(
    std::vector<Particle>& particleArray, LeavingParticles const& leavingParticles) const
{


}


void PRABoundaryCondition::applyIncomingParticleBC(Ions& ions, std::string const& pusher,
                                                      double const& dt) const
{
    (void) ions;
    (void) pusher;
    (void) dt;
}



PRABoundaryCondition::~PRABoundaryCondition()
{
}
