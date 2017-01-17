
#include "grid/gridlayoutdefs.h"

#include "Interpolator/interpolator.h"
#include "IndexesAndWeights/indexesandweightso1.h"
#include "IndexesAndWeights/indexesandweightso2.h"
#include "IndexesAndWeights/indexesandweightso3.h"
#include "IndexesAndWeights/indexesandweightso4.h"



Interpolator::Interpolator(uint32 order)
    : order_{order},
      impl_{ nullptr}
{

    switch( order )
    {
    case 1:
        impl_ = std::unique_ptr<IndexesAndWeightsO1>( new IndexesAndWeightsO1(order) ) ;
        break;
    case 2:
        impl_ = std::unique_ptr<IndexesAndWeightsO2>( new IndexesAndWeightsO2(order) ) ;
        break;
    case 3:
        impl_ = std::unique_ptr<IndexesAndWeightsO3>( new IndexesAndWeightsO3(order) ) ;
        break;
    case 4:
        impl_ = std::unique_ptr<IndexesAndWeightsO4>( new IndexesAndWeightsO4(order) ) ;
        break;
    }
}



std::tuple<std::vector<uint32>, std::vector<double>>
Interpolator::getIndexesAndWeights( Particle const & particle, \
                                    Direction dir, \
                                    QtyCentering centering ) const
{

    uint32 idir = static_cast<uint32>( dir ) ;

    // we compute the reduced coordinate
    // in the adequate direction
    double reducedCoord = particle.icell[idir] + \
            static_cast<double>(particle.delta[idir]) ;

    if(centering == QtyCentering::dual){
        reducedCoord += 0.5 ;
    }

    impl_->computeIndexes( reducedCoord ) ;
    impl_->computeWeights( reducedCoord ) ;

    std::vector<uint32> indexList  = impl_->indexList () ;
    std::vector<double> weightList = impl_->weightList() ;

    return std::make_tuple( indexList, weightList ) ;
}



std::tuple<std::vector<uint32>, std::vector<double>>
Interpolator::getIndexesAndWeights( Particle const & particle, Direction dir ) const
{

    uint32 idir = static_cast<uint32>( dir ) ;

    // we compute the reduced coordinate
    // in the adequate direction
    double reducedCoord = particle.icell[idir] + static_cast<double>(particle.delta[idir]) ;

    impl_->computeIndexes( reducedCoord ) ;
    impl_->computeWeights( reducedCoord ) ;

    std::vector<uint32> indexList  = impl_->indexList () ;
    std::vector<double> weightList = impl_->weightList() ;

    return std::make_tuple( indexList, weightList ) ;
}







void fieldAtParticle1D(Interpolator const& interp,
                       VecField const & E , VecField const & B,
                       GridLayout const & layout,
                       std::vector<Particle>& particles)
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

    for (uint32 iPart=0; iPart < particles.size(); ++iPart)
    {
        Particle& part = particles[iPart];

        part.Ex = 0. ; part.Ey = 0. ; part.Ez = 0. ;
        part.Bx = 0. ; part.By = 0. ; part.Bz = 0. ;

        std::vector<std::reference_wrapper<double>> partFields = \
        {part.Ex, part.Ey, part.Ez, part.Bx, part.By, part.Bz} ;

        for(uint32 ifield=0 ; ifield<partFields.size() ; ++ifield )
        {
            double & particleField = partFields[ifield] ;

            Field const& meshField = ExyzBxyzFields[ifield] ;

            auto centering = layout.fieldCentering( meshField, Direction::X ) ;

            auto indexesAndWeights = \
                    interp.getIndexesAndWeights( part, Direction::X, centering ) ;

            std::vector<uint32> indexes = std::get<0>(indexesAndWeights) ;
            std::vector<double> weights = std::get<1>(indexesAndWeights) ;

            for(uint32 ik=0 ; ik<indexes.size() ; ++ik)
            {
                particleField += meshField(indexes[ik]) * weights[ik] ;
            }
        }// end loop on fields
    }// end loop on particles
}



void fieldAtParticle2D(Interpolator const& interp,
                       VecField const & E , VecField const & B,
                       GridLayout const & layout,
                       std::vector<Particle>& particles)
{

}


void fieldAtParticle3D(Interpolator const& interp,
                       VecField const & E , VecField const & B,
                       GridLayout const & layout,
                       std::vector<Particle>& particles)
{

}


