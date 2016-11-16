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

    virtual ~Pusher() ;

    virtual void move(Particle & particle,
                      double dt, double m, double q,
                      VecField const &E,
                      VecField const &B) = 0 ;

};


#endif // PUSHER_H
