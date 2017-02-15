#ifndef PATCHBOUNDARYCONDITION_H
#define PATCHBOUNDARYCONDITION_H


#include "Plasmas/ions.h"
#include "Electromag/electromag.h"
#include "Initializer/initializerfactory.h"
#include "BoundaryConditions/boundary_conditions.h"

class PatchBoundaryCondition : public BoundaryCondition
{

private:
    Electromag EMfields_;
    Ions ions_;


public:
    PatchBoundaryCondition(std::unique_ptr<InitializerFactory> initFactory);

    virtual void applyMagneticBC(VecField& B) const override;
    virtual void applyElectricBC(VecField& E) const override;
    virtual void applyCurrentBC(VecField& J)  const override;

    virtual ~PatchBoundaryCondition();

};

#endif // PATCHBOUNDARYCONDITION_H
