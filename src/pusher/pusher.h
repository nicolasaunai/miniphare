#ifndef PUSHER_H
#define PUSHER_H

#include "pusher/pushertype.h"
#include "Plasmas/particles.h"
#include "vecfield/vecfield.h"
#include <array>

class Pusher
{

protected:
    uint32 nbdims_;
    GridLayout layout_;



    // Bridge pattern for Pusher
    // PusherType might be Boris pusher,
    // modified Boris, Vay pusher ...
   // std::unique_ptr<PusherType> impl_ ;


public:
    //Pusher( std::unique_ptr<PusherType> && impl )
        //: impl_{std::move(impl) } {}

    Pusher(GridLayout layout):
        nbdims_{layout.nbDimensions()},
        layout_{std::move(layout)}
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
                      double dt, double m,
                      VecField const & E ,
                      VecField const & B ,
                      Interpolator const& interpolator ) = 0 ;

};


#endif // PUSHER_H
