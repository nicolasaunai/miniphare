#ifndef UTILITYPHARE_H
#define UTILITYPHARE_H

#include <limits>
#include <cmath>

#include "Plasmas/particles.h"





double densityProton1(double x, double y, double z) ;
double densityProton2(double x, double y, double z) ;

double thermalSpeedProton1(double x, double y, double z) ;
double thermalSpeedProton2(double x, double y, double z) ;

std::array<double,3> bulkVelocityProton1(double x, double y, double z) ;
std::array<double,3> bulkVelocityProton2(double x, double y, double z) ;

std::array<double,3>  magneticField(double x, double y, double z) ;
std::array<double,3> electricField(double x, double y, double z) ;



namespace utils
{
    inline bool isEqual(double value1, double value2,
                        double absErr = std::numeric_limits<double>::epsilon())
     {
         return std::abs( value1 - value2 )
                 <= absErr;
    }
}



struct Box
{
    double x0, x1, y0, y1, z0, z1;

    Box( double xmin, double xmax,
         double ymin, double ymax,
         double zmin, double zmax )
        : x0{xmin}, x1{xmax},
          y0{ymin}, y1{ymax},
          z0{zmin}, z1{zmax} {}

};




class ParticleSelector
{
public:
    virtual bool operator()(Particle const& particle) const = 0;
    virtual ~ParticleSelector(){};
};





class isInBox : public ParticleSelector
{

private:
    Box parentBox_;
    Box newBox_ ;

    double dx_;
    double dy_;
    double dz_;

public:
    isInBox( Box const & parentBox , Box const & newBox,
             std::array<double, 3> dxdydz )
        : parentBox_{parentBox}, newBox_{newBox},
          dx_{dxdydz[0]}, dy_{dxdydz[1]},
          dz_{dxdydz[2]} {}

    bool operator()(Particle const& particle) const override
    {
        bool isIn = false;

        // TODO: finish implementation of this selector
        double posx = (particle.icell[0] + particle.delta[0]) * dx_  + parentBox_.x0 ;
        double posy = (particle.icell[1] + particle.delta[1]) * dy_  + parentBox_.y0 ;
        double posz = (particle.icell[2] + particle.delta[2]) * dz_  + parentBox_.z0 ;

        //return true if the particle is in the box
        if( posx > newBox_.x0 && posx < newBox_.x1 &&
            posy > newBox_.y0 && posy < newBox_.y1 &&
            posz > newBox_.z0 && posz < newBox_.z1 )
        {
            isIn = true;
        }

        return isIn;
    }
    virtual ~isInBox(){}
};



#endif // UTILITYPHARE_H
