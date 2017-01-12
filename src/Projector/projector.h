#ifndef PROJECTOR_H
#define PROJECTOR_H


#include <memory>

#include "types.h"


#include "grid/gridlayout.h"
#include "Field/field.h"
#include "Plasmas/particles.h"

#include "IndexesAndWeights/indexesandweights.h"



class Projector
{
private:
    uint32 order_ ;
    std::unique_ptr<IndexesAndWeights> impl_ ;

public:

    virtual ~Projector() = default ;

    Projector(uint32 order);

    std::tuple<std::vector<uint32>, std::vector<double>>
    getIndexesAndWeights( Particle const & particle, Direction dir ) ;

};



#endif // PROJECTOR_H
