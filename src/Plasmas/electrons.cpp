
#include <algorithm>
#include <functional>

#include "electrons.h"



Electrons::Electrons(GridLayout const& layout, double Te)
    :  Ve_    { layout.allocSize(HybridQuantity::V),
                layout.allocSize(HybridQuantity::V),
                layout.allocSize(HybridQuantity::V),
                { {HybridQuantity::V, HybridQuantity::V, HybridQuantity::V} },
                "_electronBulkFlow" },
      Pe_     { layout.allocSize(HybridQuantity::P), HybridQuantity::P, "_electronPressure" },
      Te_{Te},
      layout_{layout}
{

}


const VecField& Electrons::bulkVel(VecField const& Vi, const Field& Ni, const VecField& J)
{
    const uint32 compX=0;
    const uint32 compY=1;
    const uint32 compZ=2;

    LinearCombination const& JxOnMoment = layout_.ExToMoment();
    LinearCombination const& JyOnMoment = layout_.EyToMoment();
    LinearCombination const& JzOnMoment = layout_.EzToMoment();


    Field& Vex = Ve_.component(compX);
    Field& Vey = Ve_.component(compY);
    Field& Vez = Ve_.component(compZ);

    const Field& Jx  = J.component(compX);
    const Field& Jy  = J.component(compY);
    const Field& Jz  = J.component(compZ);

    const Field& Vix = Vi.component(compX);
    const Field& Viy = Vi.component(compY);
    const Field& Viz = Vi.component(compZ);



    // every V (x,y,z) component and density has the same centering: ppp
    // so only one spatial loop is necessary
    uint32 const iStart = layout_.physicalStartIndex(Vex, Direction::X);
    uint32 const iEnd   = layout_.physicalEndIndex(Vex, Direction::X);

    for (uint32 ix = iStart; ix <= iEnd; ++ix)
    {

        // J is defined on E. We don't know here where E (thus J) here
        // we need to call the gridLayout LinearCombinations to get J components
        // at the primal nodes.
        double jxloc = 0;
        for (WeightPoint const& wp : JxOnMoment)
        {
            jxloc += wp.coef * Jx(ix + wp.ix);
        }

        double jyloc = 0;
        for (WeightPoint const& wp : JyOnMoment)
        {
            jyloc += wp.coef * Jy(ix + wp.ix);
        }

        double jzloc = 0;
        for (WeightPoint const& wp : JzOnMoment)
        {
            jzloc += wp.coef * Jz(ix + wp.ix);
        }

        // now we have all J components at the primal nodes
        // we can safely compute Ve.

        Vex(ix)  = Vix(ix) - jxloc / Ni(ix);
        Vey(ix)  = Viy(ix) - jyloc / Ni(ix);
        Vez(ix)  = Viz(ix) - jzloc / Ni(ix);
    }

    return Ve_;
}



const Field& Electrons::pressure(Field const& Ni)
{
    const uint32 totalSize = Ni.size();

    for (uint32 i = 0; i < totalSize; ++i)
    {
        Pe_(i) = Ni(i) * Te_;
    }
    return Pe_;
}







