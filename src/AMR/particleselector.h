#ifndef PARTICLESELECTOR_H
#define PARTICLESELECTOR_H

#include "utilityphare.h"

#include "Plasmas/particles.h"


class ParticleSelector
{
public:
    virtual bool operator()(Particle const& particle) const = 0;
    virtual ~ParticleSelector() = default;
};



class isInBox : public ParticleSelector
{

private:
    Box parentBox_;
    Box newBox_ ;

    double dx_;
    double dy_;
    double dz_;

public:
    isInBox( Box const & parentBox , Box const & newBox,
             std::array<double, 3> dxdydz )
        : parentBox_{parentBox}, newBox_{newBox},
          dx_{dxdydz[0]}, dy_{dxdydz[1]},
          dz_{dxdydz[2]} {}

    bool operator()(Particle const& particle) const override ;
    virtual ~isInBox(){}
};




#endif // PARTICLESELECTOR_H
