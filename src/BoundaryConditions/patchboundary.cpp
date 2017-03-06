#include "BoundaryConditions/patchboundary.h"



void PatchBoundary::applyElectricBC(VecField& E, GridLayout const& layout) const
{




}


void PatchBoundary::applyMagneticBC(VecField& B, GridLayout const& layout) const
{

}


void PatchBoundary::applyCurrentBC( VecField& J, GridLayout const& layout) const
{


}


void PatchBoundary::applyDensityBC( Field& J,    GridLayout const& layout) const
{


}


void PatchBoundary::applyBulkBC( VecField& Vi,GridLayout const& layout) const
{


}


void PatchBoundary::applyParticleBC( std::vector<Particle>& particleArray,
                      LeavingParticles const& leavingParticles)  const
{



}



