#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <memory>

#include "types.h"


#include "grid/gridlayout.h"
#include "Field/field.h"
#include "Plasmas/particles.h"

#include "indexesandweights.h"



class Interpolator
{
private:
    uint32  dim_ ;

    Point  minGlobal_ ;

    std::unique_ptr<IndexesAndWeights> impl_ ;

public:

    virtual ~Interpolator() ;

    Interpolator( std::unique_ptr<IndexesAndWeights> && impl,
                  const GridLayout & layout, const Point & minGlobal )
        : dim_{layout.nbDimensions()}, minGlobal_{minGlobal},
          impl_{ std::move(impl) } {}

    // interpolate a field known on a mesh to a particle
    virtual void gridToParticle( const Field & field, const Particle & part ) const = 0 ;

};



#endif // INTERPOLATOR_H
