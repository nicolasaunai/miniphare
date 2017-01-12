#ifndef PUSHERTYPE_H
#define PUSHERTYPE_H

#include <string>

#include "grid/gridlayout.h"
#include "Plasmas/particles.h"
#include "vecfield/vecfield.h"

#include "Interpolator/interpolator.h"




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
protected:
    std::string pusherName_ ;

    double dx_ ;
    uint32 nbrCellx_ ;

    GridLayout layout_ ;


public:
    PusherType( std::string const & pusherName, GridLayout const & layout)
        : pusherName_{pusherName}, layout_{layout}
    {
        dx_ = layout.dx() ;
        nbrCellx_ = layout.nbrCellx() ;
    }

    PusherType(PusherType&& toMove)      = default;
    PusherType& operator=(PusherType&& source) = default;

    // Dont't forget =default HERE
    // or move operations won't be generated
    virtual ~PusherType() = default ;


    virtual void move1D(std::vector<Particle> & partIn ,
                        std::vector<Particle> & partOut,
                        double dt, double m,
                        VecField const & E ,
                        VecField const & B ,
                        Interpolator & interpolator ) = 0 ;

    virtual void move2D(std::vector<Particle> & partIn ,
                        std::vector<Particle> & partOut,
                        double dt, double m,
                        VecField const & E ,
                        VecField const & B ,
                        Interpolator & interpolator ) = 0 ;

    virtual void move3D(std::vector<Particle> & partIn ,
                        std::vector<Particle> & partOut,
                        double dt, double m,
                        VecField const & E ,
                        VecField const & B ,
                        Interpolator & interpolator ) = 0 ;

};


#endif // PUSHERTYPE_H
