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
    uint32  dim_ ;
    std::unique_ptr<IndexesAndWeights> impl_ ;

public:

    // Dont't forget =default HERE
    // or move operations won't be generated
    virtual ~Interpolator() = default ;

    Interpolator(const GridLayout & layout);

    // we might interpolate a field from
    // a primal or a dual mesh
    std::tuple<std::vector<uint32>, std::vector<double>>
    getIndexesAndWeights( Particle const & particle, \
                          Direction dir, QtyCentering centering ) ;

};



#endif // INTERPOLATOR_H
