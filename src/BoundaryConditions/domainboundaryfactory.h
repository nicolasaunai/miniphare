#ifndef DOMAINBOUNDARYFACTORY_H
#define DOMAINBOUNDARYFACTORY_H

#include "BoundaryConditions/boundary.h"
#include "BoundaryConditions/periodicdomainboundary.h"
#include "domainboundarycondition.h"


class DomainBoundaryFactory
{
public:
    DomainBoundaryFactory() = delete;
    static std::unique_ptr<Boundary> makeBoundary(DomainBoundaryCondition::BoundaryInfo)
    {
        return nullptr;
    }

};

#endif // DOMAINBOUNDARYFACTORY_H
