#ifndef PARTICLEMESH_H
#define PARTICLEMESH_H

#include <vector>

#include "core/Interpolator/interpolator.h"
#include "data/Electromag/electromag.h"
#include "data/Plasmas/particles.h"
#include "data/grid/gridlayout.h"
#include "data/vecfield/vecfield.h"



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

void updateChargeDensityAndFlux(Interpolator& interpolator, Species& species,
                                GridLayout const& layout, std::vector<Particle>& particles);



#endif // PARTICLEMESH_H
