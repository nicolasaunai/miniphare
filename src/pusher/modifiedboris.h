#ifndef MODIFIEDBORIS_H
#define MODIFIEDBORIS_H

#include "pusher/pushertype.h"


class ModifiedBoris : public PusherType
{


public:
    ModifiedBoris( const std::string & pusherName, const GridLayout & layout )
        : PusherType(pusherName, layout) {}

    virtual ~ModifiedBoris() {}

    virtual void move1D( std::vector<Particle> & partIn ,
                         std::vector<Particle> & partOut,
                         double dt, double m,
                         VecField const & E ,
                         VecField const & B ) override ;

    virtual void move2D( std::vector<Particle> & partIn ,
                         std::vector<Particle> & partOut,
                         double dt, double m,
                         VecField const & E ,
                         VecField const & B ) override ;

    virtual void move3D( std::vector<Particle> & partIn ,
                         std::vector<Particle> & partOut,
                         double dt, double m,
                         VecField const & E ,
                         VecField const & B ) override ;

    void prePush1D( Particle & particleIn,
                    Particle & particleOut,
                    double dt ) ;

    void pushVelocity1D( Particle & particleIn,
                         Particle & particleOut,
                         double dt, double m ) ;

    void corPush1D( Particle & particleIn,
                    Particle & particleOut,
                    double dt ) ;



};



#endif // MODIFIEDBORIS_H
