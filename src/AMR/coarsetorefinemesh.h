#ifndef COARSETOREFINEMESH_H
#define COARSETOREFINEMESH_H

#include "grid/gridlayout.h"
#include "Plasmas/particles.h"
#include "Interpolator/interpolator.h"
#include "Electromag/electromag.h"



/* ----------------------------------------------------------------------------

                      Field interpolation from a coarse patch
                      to a refined patch

   ---------------------------------------------------------------------------- */

void fieldAtRefinedNodes( Interpolator& interpolator,
                          GridLayout const & coarseLayout,
                          Electromag const & parentElectromag ,
                          GridLayout const & refinedLayout,
                          ElectromagInitializer & eminit ) ;


#endif // COARSETOREFINEMESH_H
