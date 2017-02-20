#ifndef DOMAINBOUNDARYCONDITION_H
#define DOMAINBOUNDARYCONDITION_H

#include <vector>

#include "BoundaryConditions/boundary_conditions.h"
#include "BoundaryConditions/boundary.h"
#include "vecfield/vecfield.h"
#include "grid/gridlayout.h"
#include "Plasmas/particles.h"



/**
 * @brief The DomainBoundaryCondition class is a special type of BoundaryCondition
 * that applies only to the physical domain (root patch)
 */
class DomainBoundaryCondition : public BoundaryCondition
{
private:
    std::vector<std::unique_ptr<Boundary>> boundaries_;
    GridLayout layout_;

public:
    using BoundaryInfo  = std::pair<Edge, BoundaryType>;
    DomainBoundaryCondition(GridLayout layout, std::vector<BoundaryInfo> boundaryInfos);


    virtual void applyMagneticBC(VecField& B) const override;
    virtual void applyElectricBC(VecField& E) const override;
    virtual void applyCurrentBC(VecField& J)  const override;
    virtual void applyDensityBC(Field& N)     const override;
    virtual void applyBulkBC(VecField& Vi)    const override;
    virtual void applyParticleBC(std::vector<Particle>& particleArray)  const override;
};


#endif // DOMAINBOUNDARYCONDITION_H
