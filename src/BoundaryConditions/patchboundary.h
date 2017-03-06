#ifndef PATCHBOUNDARY_H
#define PATCHBOUNDARY_H

#include "Initializer/ionsinitializer.h"
#include "Initializer/electromaginitializer.h"

#include "Plasmas/ions.h"

#include "BoundaryConditions/boundary.h"
#include "Electromag/electromag.h"
#include "vecfield/vecfield.h"


class PatchBoundary : public Boundary
{
private:
    Ions ions_ ;
    Electromag EMfields_ ;
    Box position_ ;

public:
    PatchBoundary(GridLayout const & layout, Box const & box,
                  std::unique_ptr<IonsInitializer> ionsInit,
                  std::unique_ptr<ElectromagInitializer> electromagInit )
        :ions_{layout, std::move(ionsInit)},
          EMfields_{std::move(electromagInit)},
          position_{box}
    {std::cout << "creating patch boundary\n";}

    virtual ~PatchBoundary() = default ;

    virtual void applyElectricBC(VecField& E, GridLayout const& layout) const override ;
    virtual void applyMagneticBC(VecField& B, GridLayout const& layout) const override ;
    virtual void applyCurrentBC( VecField& J, GridLayout const& layout) const override ;
    virtual void applyDensityBC( Field& J,    GridLayout const& layout) const override ;
    virtual void applyBulkBC(    VecField& Vi,GridLayout const& layout) const override ;
    virtual void applyParticleBC(std::vector<Particle>& particleArray,
                                 LeavingParticles const& leavingParticles)  const override ;

};


#endif // PATCHBOUNDARY_H
