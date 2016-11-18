#ifndef MODIFIEDBORIS_H
#define MODIFIEDBORIS_H

#include "pusher/pushertype.h"


class ModifiedBoris : public PusherType
{


public:
    ModifiedBoris( const std::string & pusherName, const GridLayout & layout )
        : PusherType(pusherName, layout) {}

    virtual ~ModifiedBoris() {}

    virtual void move1D( Particle & particle,
                         double dt, double m, double q,
                         Point const & Epart,
                         Point const & Bpart) override ;

    virtual void move2D( Particle & particle,
                         double dt, double m, double q,
                         Point const &Epart,
                         Point const &Bpart) override ;

    virtual void move3D( Particle & particle,
                         double dt, double m, double q,
                         Point const &Epart,
                         Point const &Bpart) override ;

};



#endif // MODIFIEDBORIS_H
