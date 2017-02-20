#ifndef MODIFIEDBORIS_H
#define MODIFIEDBORIS_H

#include "pusher.h"
#include "Interpolator/interpolator.h"


class ModifiedBoris : public Pusher
{
private:
    void prePush_(std::vector<Particle> const& particleIn,
                  std::vector<Particle> & particleOut) ;

    void pushVelocity_(std::vector<Particle> const& particleIn,
                       std::vector<Particle> & particleOut, double m) ;

    void corPush_(std::vector<Particle> const& particleIn,
                  std::vector<Particle> & particleOut) ;

public:
    //ModifiedBoris( const std::string & pusherName, const GridLayout & layout ) {}
    ModifiedBoris(GridLayout layout, double dt):
        Pusher(std::move(layout), dt) {}

    virtual ~ModifiedBoris() {}

    virtual void move( std::vector<Particle> const& partIn ,
                       std::vector<Particle> & partOut,
                       double m,
                       VecField const & E ,
                       VecField const & B ,
                       Interpolator const& interpolator ) override ;

};



#endif // MODIFIEDBORIS_H
