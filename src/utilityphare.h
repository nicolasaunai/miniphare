#ifndef UTILITYPHARE_H
#define UTILITYPHARE_H

#include <limits>
#include <cmath>

#include "Plasmas/particles.h"




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
    Box box_;

public:
    isInBox(Box& box) : box_{box}{}

    bool operator()(Particle const& particle) const override
    {
        //return true if the particle is in the box
        return false;
    }
    virtual ~isInBox(){}
};



#endif // UTILITYPHARE_H
