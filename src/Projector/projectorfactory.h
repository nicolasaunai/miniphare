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
    createProjector( const GridLayout & layout, const Point & minLocal )
    {
        uint32 order = layout.order() ;
        uint32 dim = layout.nbDimensions() ;

        std::unique_ptr<IndexesAndWeights> impl  ;
        std::unique_ptr<Projector> projector ;

        switch(order){
        case 1:
            impl = std::unique_ptr<IndexesAndWeightsO1>( new IndexesAndWeightsO1(order) ) ;
            break;
        case 2:
            impl = std::unique_ptr<IndexesAndWeightsO2>( new IndexesAndWeightsO2(order) ) ;
            break;
        case 3:
            impl = std::unique_ptr<IndexesAndWeightsO3>( new IndexesAndWeightsO3(order) ) ;
            break;
        case 4:
            impl = std::unique_ptr<IndexesAndWeightsO4>( new IndexesAndWeightsO4(order) ) ;
            break;
        }

        switch(dim){
        case 1:
            projector = std::unique_ptr<Projector1D>
                    ( new Projector1D( std::move(impl), layout, minLocal) ) ;
            break;
            // ... same for 2D and 3D
        }

        return  projector ;
    }

};


#endif // PROJECTORFACTORY_H
