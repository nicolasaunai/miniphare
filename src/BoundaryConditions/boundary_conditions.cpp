
#include "BoundaryConditions/boundary_conditions.h"

BoundaryCondition::~BoundaryCondition()
{

}


#if 0
void BoundaryConditions::applyElectricBC(VecField& E)
{

    for( auto & ptr : fieldBoundaryConditions_ )
    {
        ptr->applyElectricBC( E ) ;
    }

}


void BoundaryConditions::applyMagneticBC(VecField& B)
{

    for( auto & ptr : fieldBoundaryConditions_ )
    {
        ptr->applyMagneticBC( B ) ;
    }

}


void BoundaryConditions::applyCurrentBC(VecField& J)
{


}
#endif
