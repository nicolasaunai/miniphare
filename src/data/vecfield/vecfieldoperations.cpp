
#include "vecfieldoperations.h"
#include <algorithm>

void timeAverage(VecField const& vfAtTime1, VecField const& vfAtTime2, VecField& Vavg)
{
    for (uint32 iComponent = 0; iComponent < 3; ++iComponent)
    {
        // add vfAtTime1 and vfAtTime2 into Vavg
        std::transform(vfAtTime1.component(iComponent).begin(),
                       vfAtTime1.component(iComponent).end(),
                       vfAtTime2.component(iComponent).begin(), Vavg.component(iComponent).begin(),
                       std::plus<double>());

        // Vavg *= 0.5
        std::transform(Vavg.component(iComponent).begin(), Vavg.component(iComponent).end(),
                       Vavg.component(iComponent).begin(), [](double x) { return x * 0.5; });
    }
}


void getVariation(VecField const& vfAtT1, VecField const& vfAtT2, VecField& variation, double dt2t1)
{
    for (uint32 iComponent = 0; iComponent < 3; ++iComponent)
    {
        // vfAtT2 - vfAtT1
        std::transform(vfAtT2.component(iComponent).begin(), vfAtT2.component(iComponent).end(),
                       vfAtT1.component(iComponent).begin(),
                       variation.component(iComponent).begin(), std::minus<double>());

        // (vfAtT2 - vfAtT1)/(t2 - t1)
        std::transform(
            variation.component(iComponent).begin(), variation.component(iComponent).end(),
            variation.component(iComponent).begin(), [&](double x) { return x / dt2t1; });
    }
}


void timeInterpolation(VecField const& vfAtT1, VecField& variation, VecField& interpolated,
                       double delta)
{
    for (uint32 iComponent = 0; iComponent < 3; ++iComponent)
    {
        // varprime =  delta * variation
        std::transform(
            variation.component(iComponent).begin(), variation.component(iComponent).end(),
            variation.component(iComponent).begin(), [&](double x) { return x * delta; });

        // vfInterp = vfAtT1 + varprime
        std::transform(vfAtT1.component(iComponent).begin(), vfAtT1.component(iComponent).end(),
                       variation.component(iComponent).begin(),
                       interpolated.component(iComponent).begin(), std::plus<double>());
    }
}
