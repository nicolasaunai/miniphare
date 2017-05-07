#ifndef PARTICLEMESH_H
#define PARTICLEMESH_H

#include <vector>

#include "Electromag/electromag.h"
#include "Interpolator/interpolator.h"
#include "Plasmas/particles.h"
#include "grid/gridlayout.h"
#include "vecfield/vecfield.h"



/* ----------------------------------------------------------------------------

                      Field interpolations at particles

   ---------------------------------------------------------------------------- */

void fieldsAtParticles(Interpolator& interp, VecField const& E, VecField const& B,
                       GridLayout const& layout, std::vector<Particle>& particles);



/* ----------------------------------------------------------------------------

                      Interpolations from particles to moments

   ---------------------------------------------------------------------------- */

void computeChargeDensityAndFlux(Interpolator& interpolator, Species& species,
                                 GridLayout const& layout, std::vector<Particle>& particles);




#endif // PARTICLEMESH_H
