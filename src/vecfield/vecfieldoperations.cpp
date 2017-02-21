
#include "vecfield/vecfieldoperations.h"


void  average( VecField const& VF1, VecField const& VF2,
               VecField& Vavg, GridLayout const& layout )
{
    switch (layout.nbDimensions())
    {
    case 1:
        average1D( VF1, VF2, Vavg, layout ) ;
        break;

    case 2:
        average2D( VF1, VF2, Vavg, layout ) ;
        break;

    case 3:
        average3D( VF1, VF2, Vavg, layout ) ;
        break;
    }

}


void  average1D( VecField const& VF1, VecField const& VF2,
                 VecField& Vavg, GridLayout const& layout )
{
    std::vector<std::reference_wrapper<Field const>> VF1_fields = VF1.components() ;
    std::vector<std::reference_wrapper<Field const>> VF2_fields = VF2.components() ;

    std::vector<std::reference_wrapper<Field>> Vavg_fields = Vavg.components() ;

    uint32 nbrCompo = static_cast<uint32>( VF1_fields.size() ) ;

    for (uint32 iCompo=0 ; iCompo<nbrCompo ; iCompo++)
    {
        Field const & field1 = VF1_fields[iCompo] ;
        Field const & field2 = VF2_fields[iCompo] ;

        Field & avgField = Vavg_fields[iCompo] ;

        auto field1LayoutX = layout.fieldCentering( field1, Direction::X ) ;
        auto field2LayoutX = layout.fieldCentering( field2, Direction::X ) ;

        // Are these fields on the same layout ?
        if( field1LayoutX != field2LayoutX )
        {
            throw std::runtime_error("Not Implemented");
        } else
        {
            // compute average
            uint32 startX = layout.physicalStartIndex( field1, Direction::X) ;
            uint32 endX   = layout.physicalEndIndex  ( field1, Direction::X) ;

            for( uint32 ig=startX ; ig<=endX ; ++ig )
            {
                avgField(ig) = 0.5*(field1(ig) + field2(ig)) ;
            }
        }
    }
}


void  average2D( VecField const& VF1, VecField const& VF2,
                 VecField& Vavg, GridLayout const& layout )
{

}


void  average3D( VecField const& VF1, VecField const& VF2,
                 VecField& Vavg, GridLayout const& layout )
{

}


