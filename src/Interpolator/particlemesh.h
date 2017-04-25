#ifndef PARTICLEMESH_H
#define PARTICLEMESH_H

#include<vector>

#include "grid/gridlayout.h"
#include "vecfield/vecfield.h"
#include "Plasmas/particles.h"
#include "Interpolator/interpolator.h"
#include "Electromag/electromag.h"



/* ----------------------------------------------------------------------------

                      Field interpolations at particles

   ---------------------------------------------------------------------------- */

void fieldsAtParticles(Interpolator &interp,
                       VecField const& E, VecField const& B,
                       GridLayout const& layout,
                       std::vector<Particle>& particles);



/* ----------------------------------------------------------------------------

                      Interpolations from particles to moments

   ---------------------------------------------------------------------------- */

void computeChargeDensityAndFlux(Interpolator const& interpolator,
                                 Species& species,
                                 GridLayout const& layout,
                                 std::vector<Particle>& particles);





#endif // PARTICLEMESH_H
