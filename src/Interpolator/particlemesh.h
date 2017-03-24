#ifndef PARTICLEMESH_H
#define PARTICLEMESH_H

#include<vector>

#include "grid/gridlayout.h"
#include "vecfield/vecfield.h"
#include "Plasmas/particles.h"
#include "Interpolator/interpolator.h"
#include "Electromag/electromag.h"



/* ----------------------------------------------------------------------------

                      Field interpolation from a coarse patch
                      to a refined patch

   ---------------------------------------------------------------------------- */

void fieldAtRefinedNodes( Interpolator const& interpolator,
                          GridLayout const & coarseLayout,
                          Electromag const & parentElectromag ,
                          GridLayout const & refinedLayout,
                          ElectromagInitializer & eminit ) ;


/* ----------------------------------------------------------------------------

                      Field interpolations at particles

   ---------------------------------------------------------------------------- */

void fieldsAtParticles(Interpolator const& interp,
                       VecField const& E, VecField const& B,
                       GridLayout const& layout,
                       std::vector<Particle>& particles);



/* ----------------------------------------------------------------------------

                      Interpolations from particles to moments

   ---------------------------------------------------------------------------- */

void computeChargeDensityAndFlux(Interpolator& interpolator,
                                 Species& species,
                                 GridLayout const& layout,
                                 std::vector<Particle>& particles);





#endif // PARTICLEMESH_H
