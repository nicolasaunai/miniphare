#ifndef GRIDLAYOUT_H
#define GRIDLAYOUT_H

#include <vector>
#include <array>
#include <memory>

#include "types.h"
#include "Field/field.h"
#include "gridlayoutimpl.h"




class GridLayout
{
private:
    double dx_ ;        // mesh sizes
    double dy_ ;        // mesh sizes
    double dz_ ;        // mesh sizes

    double odx_ ;       // 1./dx
    double ody_ ;       // 1./dy
    double odz_ ;       // 1./dz

    uint32 nbrCellx_  ;
    uint32 nbrCelly_  ;
    uint32 nbrCellz_  ;

    uint32 interpOrder_ ;

    std::unique_ptr<GridLayoutImpl> implPtr_;

public:

    static const uint32 directionX = 0;
    static const uint32 directionY = 1;
    static const uint32 directionZ = 2;

    static const uint32 minNbrCells = 10; // minimum nbr of cells in a
                                          // non-invariant direction

    GridLayout(std::array<double,3> dxdydz, std::array<uint32,3> nbrCells,
               uint32 nbDims      , std::string layoutName,
               uint32 interpOrder );
    GridLayout(GridLayout const& source);
    GridLayout(GridLayout&& source);


    double dx() const {return dx_;}
    double dy() const {return dy_;}
    double dz() const {return dz_;}

    double odx()const {return odx_;}
    double ody()const {return ody_;}
    double odz()const {return odz_;}

    double nbrCellx() const {return nbrCellx_;}
    double nbrCelly() const {return nbrCelly_;}
    double nbrCellz() const {return nbrCellz_;}


    // return the (total) number of mesh points
    // this does depend on the layout
    uint32 nx() const;
    uint32 ny() const;
    uint32 nz() const;

    uint32 physicalStartIndex(Field const& field, uint32 direction) const;
    uint32 physicalEndIndex  (Field const& field, uint32 direction) const;

    uint32 ghostStartIndex(Field const& field, uint32 direction) const;
    uint32 ghostEndIndex  (Field const& field, uint32 direction) const;

    void deriv(Field const& operand, uint32 direction, Field& derivative)const;

    uint32 nbDimensions() const;
};





#endif // GRIDLAYOUT_H
