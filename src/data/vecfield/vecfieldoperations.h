#ifndef VECFIELDOPERATIONS_H
#define VECFIELDOPERATIONS_H


#include "data/vecfield/vecfield.h"


void timeAverage(VecField const& vfAtTime1, VecField const& vfAtTime2, VecField& Vavg);


void getVariation(VecField const& vfAtT1, VecField const& vfAtT2, VecField& variation,
                  double dt2t1);

void timeInterpolation(VecField const& vfAtT1, VecField const& vfAtT2, VecField& interpolated,
                       double dt2t1, double delta);


#endif // VECFIELDOPERATIONS_H
