#ifndef COARSETOREFINEMESH_H
#define COARSETOREFINEMESH_H

#include "Electromag/electromag.h"
#include "Interpolator/interpolator.h"
#include "Plasmas/particles.h"
#include "grid/gridlayout.h"



void getClosestGridNode1D(Point const& point, GridLayout const& targetLayout,
                          QtyCentering const& fieldCtr, uint32& iNode);


Field buildAlignedChildField1D(Field const& childField, GridLayout const& childLayout,
                               uint32 const& nbrNodesTarget, uint32 const& refineRatio);

/* ----------------------------------------------------------------------------

                      Field interpolation from a coarse patch
                      to a refined patch

   ---------------------------------------------------------------------------- */

void fieldAtRefinedNodes(Interpolator& interpolator, GridLayout const& coarseLayout,
                         Electromag const& parentElectromag, GridLayout const& refinedLayout,
                         ElectromagInitializer& eminit);


#endif // COARSETOREFINEMESH_H
