#ifndef PUSHER1D_H
#define PUSHER1D_H


#include "pusher/pusher.h"

#include "grid/gridlayout.h"


class Pusher1D : public Pusher
{
private:

    uint32 nbDims_ ;

    double dx_ ;

    uint32 nbrCellx_ ;

public:
    Pusher1D( std::unique_ptr<PusherType> impl, const GridLayout & layout)
        : Pusher( std::move(impl) )
    {
        nbDims_ = layout.nbDimensions() ;
        dx_ = layout.dx() ;
        nbrCellx_ = layout.nbrCellx() ;
    }

    virtual ~Pusher1D() ;

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
