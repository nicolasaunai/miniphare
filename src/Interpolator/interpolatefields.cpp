
#include "Interpolator/interpolatefields.h"



void compute1DFieldsAtParticles( Interpolator & interp,
                                 Particle & part,
                                 GridLayout const & layout,
                                 VecField const & E , VecField const & B )
{
    uint32 idirX = static_cast<uint32>(Direction::X) ;
    uint32 idirY = static_cast<uint32>(Direction::Y) ;
    uint32 idirZ = static_cast<uint32>(Direction::Z) ;

    Field const & Ex = E.component(idirX) ;
    Field const & Ey = E.component(idirY) ;
    Field const & Ez = E.component(idirZ) ;

    Field const & Bx = B.component(idirX) ;
    Field const & By = B.component(idirY) ;
    Field const & Bz = B.component(idirZ) ;

    std::vector<std::reference_wrapper<Field const>> ExyzBxyzFields \
            = {Ex, Ey, Ez, Bx, By, Bz} ;


    part.Ex = 0. ; part.Ey = 0. ; part.Ez = 0. ;
    part.Bx = 0. ; part.By = 0. ; part.Bz = 0. ;

    std::vector<std::reference_wrapper<double>> partFields = \
    {part.Ex, part.Ey, part.Ez, part.Bx, part.By, part.Bz} ;

    for(uint32 ifield=0 ; ifield<partFields.size() ; ++ifield )
    {
        double & particleField = partFields[ifield] ;

        Field const & meshField = ExyzBxyzFields[ifield] ;

        auto centering = layout.fieldCentering( meshField, Direction::X ) ;

        auto indexesAndWeights = \
                interp.getIndexesAndWeights( part, Direction::X, centering ) ;

        std::vector<uint32> indexes = std::get<0>(indexesAndWeights) ;
        std::vector<double> weights = std::get<1>(indexesAndWeights) ;

        for(uint32 ik=0 ; ik<indexes.size() ; ++ik)
        {
            particleField += meshField(indexes[ik]) * weights[ik] ;
        }
    }


}


