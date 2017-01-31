
#include "Field/fieldbc3dcollection.h"


void CollectionOfBoundaryConditions::applyElectricBC( VecField & E )
{

    for( auto & ptr : edgesBoundaryConditions_ )
    {
        ptr->applyElectricBC( E ) ;
    }

}


void CollectionOfBoundaryConditions::applyMagneticBC( VecField & B )
{

    for( auto & ptr : edgesBoundaryConditions_ )
    {
        ptr->applyMagneticBC( B ) ;
    }

}


void CollectionOfBoundaryConditions::applyCurrentBC( VecField & J )
{

    for( auto & ptr : edgesBoundaryConditions_ )
    {
        ptr->applyMomentsBC( J ) ;
    }

}

