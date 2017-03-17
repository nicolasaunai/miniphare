
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

    Field& Vx = Ve_.component(compX);
    Field& Vy = Ve_.component(compY);
    Field& Vz = Ve_.component(compZ);

    const Field& Jx  = J.component(compX);
    const Field& Jy  = J.component(compY);
    const Field& Jz  = J.component(compZ);

    const Field& Vix = Vi.component(compX);
    const Field& Viy = Vi.component(compY);
    const Field& Viz = Vi.component(compZ);

    const uint32 totalSize = Vx.size();

    for (uint32 i = 0; i < totalSize; ++i)
    {
        Vx(i) = Vix(i) -Jx(i)/Ni(i);
        Vy(i) = Viy(i) -Jy(i)/Ni(i);
        Vz(i) = Viz(i) -Jz(i)/Ni(i);
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







