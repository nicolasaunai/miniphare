#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <memory>

#include "types.h"
#include "Field/field.h"
#include "Plasmas/species.h"
#include "grid/gridlayout.h"
#include "Plasmas/particles.h"
#include "grid/gridlayoutdefs.h"
#include "IndexesAndWeights/indexesandweights.h"



class Interpolator
{
private:

    uint32 order_ ;
    std::unique_ptr<IndexesAndWeights> impl_ ;

public:

    Interpolator(uint32 order);

    std::tuple<std::vector<uint32>, std::vector<double>>
    getIndexesAndWeights( double reducedCoord, \
                          QtyCentering centering ) const ;

    // we might interpolate a field from
    // a primal or a dual mesh
    std::tuple<std::vector<uint32>, std::vector<double>>
    getIndexesAndWeights( Particle const & particle, \
                          Direction dir, QtyCentering centering ) const ;

    std::tuple<std::vector<uint32>, std::vector<double>>
    getIndexesAndWeights( Particle const & particle, Direction dir ) const;
};



/* ----------------------------------------------------------------------------

                      Field interpolations at particles

   ---------------------------------------------------------------------------- */

void fieldsAtParticles(Interpolator const& interp,
                       VecField const& E, VecField const& B,
                       GridLayout const& layout,
                       std::vector<Particle>& particles);

/* ----------------------------------------------------------------------------

                      Interpolation from a coarse patch
                      to a refined patch

   ---------------------------------------------------------------------------- */

void fieldAtRefinedNodes1D(Interpolator const& interp,
                           GridLayout const & parentLayout,
                           VecField const & Eparent , VecField const & Bparent,
                           GridLayout const & newLayout,
                           VecField & newE , VecField & newB);

/* ----------------------------------------------------------------------------

                      Interpolations from particles to moments

   ---------------------------------------------------------------------------- */

void computeChargeDensityAndFlux(Interpolator& interpolator,
                                 Species& species,
                                 GridLayout const& layout,
                                 std::vector<Particle>& particles);





#endif // INTERPOLATOR_H
