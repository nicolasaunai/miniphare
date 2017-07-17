#ifndef DOMAINBOUNDARYFACTORY_H
#define DOMAINBOUNDARYFACTORY_H

#include "BoundaryConditions/boundary.h"
#include "BoundaryConditions/frozenboundary.h"
#include "BoundaryConditions/periodicdomainboundary.h"
#include "domainboundarycondition.h"


/**
 * @brief The DomainBoundaryFactory class is in charge of creating specific Boundary
 *
 * The Factory is used by the constructor of DomainBoundaryCondition to create each
 * of the boundaries for the patch.
 */
class DomainBoundaryFactory
{
public:
    DomainBoundaryFactory() = delete;
    static std::unique_ptr<Boundary> makeBoundary(DomainBoundaryCondition::BoundaryInfo info)
    {
        std::unique_ptr<Boundary> bc{nullptr};

        if (info.second == BoundaryType::Periodic)
        {
            bc = std::unique_ptr<Boundary>(new PeriodicDomainBoundary{info.first});
        }
        else if (info.second == BoundaryType::Frozen)
        {
            bc = std::unique_ptr<Boundary>(new FrozenBoundary{info.first});
        }

        return bc;
    }
};

#endif // DOMAINBOUNDARYFACTORY_H
