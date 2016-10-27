#ifndef PROJECTOR1D_H
#define PROJECTOR1D_H

#include <memory>

#include "projector.h"


class Projector1D : public Projector
{
private:
    // inputs for 1 call to IndexesAndWeights
    double  odx_ ;
    double  xminGlobal_ ;

public:
    Projector1D( std::unique_ptr<IndexesAndWeights> impl,
                    const GridLayout & layout, const Point & minGlobal )
        : Projector( std::move(impl), layout, minGlobal )
    {
        odx_ = layout.odx() ;
        xminGlobal_ = minGlobal.x_ ;
    }

    // deposit a particle quantity onto the mesh
    virtual void particleToGrid( const Field & source, const Particle & part ) const override ;

};


#endif // PROJECTOR1D_H
