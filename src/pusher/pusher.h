#ifndef PUSHER_H
#define PUSHER_H

#include "grid/gridlayout.h"
#include "Plasmas/particles.h"
#include "vecfield/vecfield.h"
#include "Interpolator/interpolator.h"

class Pusher
{

protected:
    uint32 nbdims_;
    GridLayout layout_;
    double dt_;


public:

    Pusher(GridLayout layout, double dt):
        nbdims_{layout.nbDimensions()},
        layout_{std::move(layout)},
        dt_{dt}
    {}

    Pusher(Pusher const& source) = delete;
    Pusher& operator=(Pusher const& source) = delete;

    Pusher(Pusher&& toMove)      = default;
    Pusher& operator=(Pusher&& source) = default;

    // Dont't forget =default HERE
    // or move operations won't be generated
    virtual ~Pusher() = default ;

    virtual void move(std::vector<Particle> const& partIn ,
                      std::vector<Particle> & partOut,
                      double m,
                      VecField const & E ,
                      VecField const & B ,
                      Interpolator const& interpolator ) = 0 ;

};


#endif // PUSHER_H
