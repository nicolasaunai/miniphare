
#include "vecfield/vecfieldoperations.h"
#include <algorithm>

void  timeAverage(VecField const& vfAtTime1, VecField const& vfAtTime2,
                  VecField& Vavg )
{
    for (uint32 iComponent=0; iComponent < 3; ++iComponent)
    {
        // add vfAtTime1 and vfAtTime2 into Vavg
        std::transform(vfAtTime1.component(iComponent).begin(),
                       vfAtTime1.component(iComponent).end(),
                       vfAtTime2.component(iComponent).begin(),
                       Vavg.component(iComponent).begin(),
                       std::plus<double>());

        // Vavg *= 0.5
        std::transform(Vavg.component(iComponent).begin(),
                       Vavg.component(iComponent).end(),
                       Vavg.component(iComponent).begin(),
                       [] (double x){return x*0.5;});

    }
}


