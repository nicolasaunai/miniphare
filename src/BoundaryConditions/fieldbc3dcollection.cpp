
#include "BoundaryConditions/fieldbc3dcollection.h"


void CollectionOfBoundaryConditions::applyElectricBC( VecField & E )
{

    for( auto & ptr : fieldBoundaryConditions_ )
    {
        ptr->applyElectricBC( E ) ;
    }

}


void CollectionOfBoundaryConditions::applyMagneticBC( VecField & B )
{

    for( auto & ptr : fieldBoundaryConditions_ )
    {
        ptr->applyMagneticBC( B ) ;
    }

}


void CollectionOfBoundaryConditions::applyCurrentBC( VecField & J )
{


}

