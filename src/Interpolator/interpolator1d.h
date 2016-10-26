#ifndef INTERPOLATOR1D_H
#define INTERPOLATOR1D_H

#include <memory>

#include "Interpolator/interpolator.h"


class Interpolator1D : public Interpolator
{
private:
    // inputs for 1 call to IndexesAndWeights
    double  odx_ ;
    double  xminGlobal_ ;

public:
    Interpolator1D( std::unique_ptr<IndexesAndWeights> impl,
                    const GridLayout & layout, const Point & minGlobal )
        : Interpolator( std::move(impl), layout, minGlobal )
    {
        odx_ = layout.odx() ;
        xminGlobal_ = minGlobal.x_ ;
    }

    // concrete implementation : interpolate a field known on a 1D mesh to a particle
    virtual void gridToParticle( const Field & field, const Particle & part ) const override ;

};


#endif // INTERPOLATOR1D_H
