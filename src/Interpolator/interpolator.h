#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <memory>

#include "types.h"


#include "grid/gridlayout.h"
#include "grid/gridlayoutdefs.h"

#include "Field/field.h"
#include "Plasmas/particles.h"

#include "IndexesAndWeights/indexesandweights.h"



class Interpolator
{
private:

    uint32 order_ ;
    std::unique_ptr<IndexesAndWeights> impl_ ;

public:

    Interpolator(uint32 order);

    // we might interpolate a field from
    // a primal or a dual mesh
    std::tuple<std::vector<uint32>, std::vector<double>>
    getIndexesAndWeights( Particle const & particle, \
                          Direction dir, QtyCentering centering ) const ;

    std::tuple<std::vector<uint32>, std::vector<double>>
    getIndexesAndWeights( Particle const & particle, Direction dir ) const;
};




void fieldsAtParticles(Interpolator const& interp,
                       VecField const& E, VecField const& B,
                       GridLayout const& layout,
                       std::vector<Particle>& particles);


void fieldAtParticle1D(Interpolator const& interp,
                       VecField const & E , VecField const & B,
                       GridLayout const & layout,
                       std::vector<Particle>& particles);


void fieldAtParticle2D(Interpolator const& interp,
                       VecField const & E , VecField const & B,
                       GridLayout const & layout,
                       std::vector<Particle>& particles);


void fieldAtParticle3D(Interpolator const& interp,
                       VecField const & E , VecField const & B,
                       GridLayout const & layout,
                       std::vector<Particle>& particles);





#endif // INTERPOLATOR_H
