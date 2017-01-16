
#include "Field/fieldbc3dcollection.h"


void CollectionOfBoundaryConditions::applyElectricBC( VecField & E )
{

    for( auto & ptr : allFieldsBoundaryConditions_ )
    {
        ptr->applyElectricBC( E ) ;
    }

}

void CollectionOfBoundaryConditions::applyMagneticBC( VecField & B )
{

    for( auto & ptr : allFieldsBoundaryConditions_ )
    {
        ptr->applyMagneticBC( B ) ;
    }

}


