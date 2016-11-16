#ifndef MODIFIEDBORIS_H
#define MODIFIEDBORIS_H

#include "pusher/pushertype.h"


class ModifiedBoris : public PusherType
{

public:
    ModifiedBoris() {}
    virtual ~ModifiedBoris() ;

    virtual void move1D( Particle & particle,
                         double dt, double m, double q,
                         VecField const &E,
                         VecField const &B) override ;

    virtual void move2D( Particle & particle,
                         double dt, double m, double q,
                         VecField const &E,
                         VecField const &B) override ;

    virtual void move3D( Particle & particle,
                         double dt, double m, double q,
                         VecField const &E,
                         VecField const &B) override ;

};



#endif // MODIFIEDBORIS_H
