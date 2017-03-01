#ifndef VECFIELDOPERATIONS_H
#define VECFIELDOPERATIONS_H

#include "vecfield/vecfield.h"
#include "grid/gridlayout.h"


void average( VecField const& VF1, VecField const& VF2,
              VecField& Vavg, GridLayout const& layout );

void average1D( VecField const& VF1, VecField const& VF2,
                VecField& Vavg, GridLayout const& layout );

void average2D( VecField const& VF1, VecField const& VF2,
                VecField& Vavg, GridLayout const& layout );

void average3D( VecField const& VF1, VecField const& VF2,
                VecField& Vavg, GridLayout const& layout );


#endif // VECFIELDOPERATIONS_H
