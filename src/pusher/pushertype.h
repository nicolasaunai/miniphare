#ifndef PUSHERTYPE_H
#define PUSHERTYPE_H

#include <string>

#include "Plasmas/particles.h"

#include "vecfield/vecfield.h"


/**
 * @brief PusherType class is a bridge pattern
 * for Pusher.
 * PusherType might be Boris pusher,
 * modified Boris pusher, Vay pusher ...
 *
 *
 */
class PusherType
{
private:
    std::string pusherName_ ;


public:
    PusherType( const std::string & pusherName)
        : pusherName_{pusherName} {}

    PusherType(PusherType&& toMove)      = default;
    PusherType& operator=(PusherType&& source) = default;

    // Dont't forget =default HERE
    // or move operations won't be generated
    virtual ~PusherType() = default ;

    virtual void move1D(Particle & particle,
                        double dt, double m, double q,
                        VecField const &E,
                        VecField const &B) = 0 ;

    virtual void move2D(Particle & particle,
                        double dt, double m, double q,
                        VecField const &E,
                        VecField const &B) = 0 ;

    virtual void move3D(Particle & particle,
                        double dt, double m, double q,
                        VecField const &E,
                        VecField const &B) = 0 ;

};


#endif // PUSHERTYPE_H
