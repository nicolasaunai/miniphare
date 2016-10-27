#ifndef PROJECTORFACTORY_H
#define PROJECTORFACTORY_H


#include <memory>

#include "types.h"

#include "projector.h"
#include "projector1d.h"

#include "IndexesAndWeights/indexesandweightso1.h"
#include "IndexesAndWeights/indexesandweightso2.h"
#include "IndexesAndWeights/indexesandweightso3.h"
#include "IndexesAndWeights/indexesandweightso4.h"



class ProjectorFactory
{

public:

    static std::unique_ptr<Projector>
    createProjector( const GridLayout & layout, const Point & minGlobal, Direction direction )
    {
        uint32 order = layout.order() ;
        uint32 dim = layout.nbDimensions() ;

        double ods  = layout.odx() ;
        double smin = minGlobal.x_ ;

        switch(direction)
        {
        case Direction::X:
            break;

        case Direction::Y:
            ods  = layout.ody() ;
            smin = minGlobal.y_ ;
            break;

        case Direction::Z:
            ods  = layout.odz() ;
            smin = minGlobal.z_ ;
            break;
        }

        std::unique_ptr<IndexesAndWeights> impl  ;
        std::unique_ptr<Projector> projector ;

        switch(order){
        case 1:
            impl = std::unique_ptr<IndexesAndWeightsO1>( new IndexesAndWeightsO1(order, ods, smin) ) ;
            break;
        case 2:
            impl = std::unique_ptr<IndexesAndWeightsO2>( new IndexesAndWeightsO2(order, ods, smin) ) ;
            break;
        case 3:
            impl = std::unique_ptr<IndexesAndWeightsO3>( new IndexesAndWeightsO3(order, ods, smin) ) ;
            break;
        case 4:
            impl = std::unique_ptr<IndexesAndWeightsO4>( new IndexesAndWeightsO4(order, ods, smin) ) ;
            break;
        }

        switch(dim){
        case 1:
            projector = std::unique_ptr<Projector1D>
                    ( new Projector1D( std::move(impl), layout, minGlobal) ) ;
            break;
            // ... same for 2D and 3D
        }

        return  projector ;
    }

};


#endif // PROJECTORFACTORY_H
