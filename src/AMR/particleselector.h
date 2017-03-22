#ifndef PARTICLESELECTOR_H
#define PARTICLESELECTOR_H

#include "utilityphare.h"

#include "Plasmas/particles.h"


/**
 * @brief The ParticleSelector class is an interface, it mainly
 * provides an operator on a Particle.
 * This operator tests whether the particle belongs to
 * an arbitrary geometrical domain.
 *
 * The most simple and evident domain being a
 * 1D, 2D or 3D cuboid.
 *
 * In the future, other concrete implementations might be
 * required.
 *
 */
class ParticleSelector
{
public:
    virtual bool operator()(Particle const& particle) const = 0;
    virtual ~ParticleSelector() = default;
};


/**
 * @brief The isInBox class is a concrete implementation of
 * ParticleSelector.
 * The operator tests whether the particle belongs a 1D,
 * 2D or 3D cuboid.
 *
 */
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

    inline bool operator()(Particle const& particle) const override
    {

        double posx = (particle.icell[0] + particle.delta[0]) * dx_  + parentBox_.x0 ;
        double posy = (particle.icell[1] + particle.delta[1]) * dy_  + parentBox_.y0 ;
        double posz = (particle.icell[2] + particle.delta[2]) * dz_  + parentBox_.z0 ;

        //return true if the particle is in the box
        return  posx >= newBox_.x0 && posx <= newBox_.x1 &&
                posy >= newBox_.y0 && posy <= newBox_.y1 &&
                posz >= newBox_.z0 && posz <= newBox_.z1 ;
    }


    virtual ~isInBox(){}
};




#endif // PARTICLESELECTOR_H
