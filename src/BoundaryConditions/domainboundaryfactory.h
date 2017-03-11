#ifndef DOMAINBOUNDARYFACTORY_H
#define DOMAINBOUNDARYFACTORY_H

#include "BoundaryConditions/boundary.h"
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
        if (info.second == BoundaryType::Periodic)
        {
            std::unique_ptr<Boundary> bc{ new PeriodicDomainBoundary{info.first} };
            return bc;
        }
        return nullptr;
    }

};

#endif // DOMAINBOUNDARYFACTORY_H
