#ifndef PUSHER_H
#define PUSHER_H

#include "pusher/pushertype.h"

#include "Plasmas/particles.h"

#include "vecfield/vecfield.h"


class Pusher
{
protected:

    // Bridge pattern for Pusher
    // PusherType might be Boris pusher,
    // modified Boris, Vay pusher ...
    std::unique_ptr<PusherType> impl_ ;


public:
    Pusher( std::unique_ptr<PusherType> && impl )
        : impl_{std::move(impl) } {}

    Pusher(Pusher const& source) = delete;
    Pusher& operator=(Pusher const& source) = delete;

    Pusher(Pusher&& toMove)      = default;
    Pusher& operator=(Pusher&& source) = default;

    // Dont't forget =default HERE
    // or move operations won't be generated
    virtual ~Pusher() = default ;

    virtual void move(std::vector<Particle> & partIn ,
                      std::vector<Particle> & partOut,
                      double dt, double m,
                      VecField const & E ,
                      VecField const & B ) = 0 ;

};


#endif // PUSHER_H
