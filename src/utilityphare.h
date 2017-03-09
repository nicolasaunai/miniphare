#ifndef UTILITYPHARE_H
#define UTILITYPHARE_H

#include <vector>
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

    Box(): x0{0.}, x1{0.},
           y0{0.}, y1{0.},
           z0{0.}, z1{0.} {}

    Box( double xmin, double xmax )
        : x0{xmin}, x1{xmax} {}

    Box( double xmin, double xmax,
         double ymin, double ymax )
        : x0{xmin}, x1{xmax},
          y0{ymin}, y1{ymax} {}

    Box( double xmin, double xmax,
         double ymin, double ymax,
         double zmin, double zmax )
        : x0{xmin}, x1{xmax},
          y0{ymin}, y1{ymax},
          z0{zmin}, z1{zmax} {}

};

/**
 * @brief The LogicalBox struct only stores indexes
 * Remember that indexes are relative to a
 * given GridLayout (i.e. a given patch)
 *
 * We have no information about the patch position
 * in the whole domain
 *
 */
struct LogicalBox
{
    uint32 ix0, ix1, iy0, iy1, iz0, iz1;

    LogicalBox():
        ix0{1}, ix1{1},
        iy0{1}, iy1{1},
        iz0{1}, iz1{1} {}

    LogicalBox(uint32 ix0, uint32 ix1):
        ix0{ix0}, ix1{ix1},
        iy0{1}, iy1{1},
        iz0{1}, iz1{1} {}

    LogicalBox(uint32 ix0, uint32 ix1,
               uint32 iy0, uint32 iy1):
        ix0{ix0}, ix1{ix1},
        iy0{iy0}, iy1{iy1},
        iz0{1}, iz1{1} {}

    LogicalBox(uint32 ix0, uint32 ix1,
               uint32 iy0, uint32 iy1,
               uint32 iz0, uint32 iz1):
        ix0{ix0}, ix1{ix1},
        iy0{iy0}, iy1{iy1},
        iz0{iz0}, iz1{iz1} {}

};


struct PRA
{
    std::array<uint32, 3> nbrCells ;

    std::vector<LogicalBox> logicDecomposition;
    std::vector<Box> boxDecomposition;

    PRA() {}

    PRA( std::array<uint32, 3> nbrCells,
         std::vector<LogicalBox> logicBoxes,
         std::vector<Box> boxes )
        : nbrCells{nbrCells},
          logicDecomposition{logicBoxes},
          boxDecomposition{boxes} {}
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



class isInPRA : public ParticleSelector
{

private:


public:
    isInPRA() {}

    bool operator()(Particle const& particle) const override
    {
        bool isIn = false;


        return isIn;
    }
    virtual ~isInPRA(){}
};



#endif // UTILITYPHARE_H
