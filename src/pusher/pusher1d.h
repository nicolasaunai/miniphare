#ifndef PUSHER1D_H
#define PUSHER1D_H


#include "pusher/pusher.h"



class Pusher1D : public Pusher
{
private:



public:
    Pusher1D( std::unique_ptr<PusherType> impl )
        : Pusher( std::move(impl) ) {}

//    Pusher1D(Pusher1D const& source) = delete;
//    Pusher1D& operator=(Pusher1D const& source) = delete;

//    Pusher1D(Pusher1D&& toMove)      = default;
//    Pusher1D& operator=(Pusher1D&& source) = default;

    virtual ~Pusher1D() {}

    virtual void move(Particle & particle,
                      double dt, double m, double q,
                      VecField const &E,
                      VecField const &B) override
    {
        impl_->move1D( particle,
                       dt, m, q,
                       E , B ) ;
    }

};



#endif // PUSHER1D_H
