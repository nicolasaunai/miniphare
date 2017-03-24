
#include <stdexcept>

#include "grid/gridlayoutdefs.h"
#include "Interpolator/interpolator.h"
#include "IndexesAndWeights/indexesandweightso1.h"
#include "IndexesAndWeights/indexesandweightso2.h"
#include "IndexesAndWeights/indexesandweightso3.h"
#include "IndexesAndWeights/indexesandweightso4.h"



Interpolator::Interpolator(uint32 order)
    : order_{order}, impl_{ nullptr}
{

    // for each interpolation order there is a specific way to calculate
    // nodes and coef. Indexes for dual quantities mush be incremented by
    // dualOffset_, which also depend on the interpolation order.
    switch( order )
    {
    case 1:
        dualOffset_ = -0.5 ;
        impl_ = std::unique_ptr<IndexesAndWeightsO1>( new IndexesAndWeightsO1(order) ) ;
        break;
    case 2:
        dualOffset_ = -0.5 ;
        impl_ = std::unique_ptr<IndexesAndWeightsO2>( new IndexesAndWeightsO2(order) ) ;
        break;
    case 3:
        dualOffset_ = +0.5 ;
        impl_ = std::unique_ptr<IndexesAndWeightsO3>( new IndexesAndWeightsO3(order) ) ;
        break;
    case 4:
        dualOffset_ = -0.5 ;
        impl_ = std::unique_ptr<IndexesAndWeightsO4>( new IndexesAndWeightsO4(order) ) ;
        break;
    }
}











