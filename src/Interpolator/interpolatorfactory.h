#ifndef INTERPOLATORFACTORY_H
#define INTERPOLATORFACTORY_H

#include <memory>

#include "types.h"

#include "interpolator.h"
#include "interpolator1d.h"

#include "IndexesAndWeights/indexesandweightso1.h"
#include "IndexesAndWeights/indexesandweightso2.h"
#include "IndexesAndWeights/indexesandweightso3.h"
#include "IndexesAndWeights/indexesandweightso4.h"



class InterpolatorFactory
{

public:

    // TODO : where should we store - Point minGlobal - attribute ?
    // In class GridLayout ? Patch ? PatchData ?
    // Maybe a Point minLocal accounting for partitioning
    // will reveal more adequate in the future ...
    static std::unique_ptr<Interpolator>
    createInterpolator( const GridLayout & layout, const Point & minGlobal, Direction direction )
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
        std::unique_ptr<Interpolator> interpolator ;

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
            interpolator = std::unique_ptr<Interpolator1D>
                    ( new Interpolator1D( std::move(impl), layout, minGlobal) ) ;
            break;
            // ... same for 2D and 3D
        }

        return  interpolator ;
    }

};


#endif // INTERPOLATORFACTORY_H
