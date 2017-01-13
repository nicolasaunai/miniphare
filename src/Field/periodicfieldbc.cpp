
#include "Field/periodicfieldbc.h"



void PeriodicFieldBC::applyElectricBC( VecField & E )
{

    uint32 idirX = static_cast<uint32>(Direction::X) ;
    uint32 idirY = static_cast<uint32>(Direction::Y) ;
    uint32 idirZ = static_cast<uint32>(Direction::Z) ;

    Field const & Ex = E.component(idirX) ;
    Field const & Ey = E.component(idirY) ;
    Field const & Ez = E.component(idirZ) ;

    std::vector<std::reference_wrapper<Field const>> ExyzFields
            = {Ex, Ey, Ez} ;

    for( uint32 ifield=0 ; ifield<ExyzFields.size() ; ++ifield )
    {
        Field const & field = ExyzFields[ifield] ;

//        auto centering = layout.fieldCentering( field, Direction::X ) ;

    }



}


void PeriodicFieldBC::applyMagneticBC( VecField & E )
{


}



