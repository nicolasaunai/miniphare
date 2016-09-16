#ifndef GRIDLAYOUT_H
#define GRIDLAYOUT_H

#include <vector>
#include <array>
#include <memory>

#include "types.h"
#include "hybridenums.h"

#include "Electromag/electromag.h"
#include "Field/field.h"
#include "gridlayoutimpl.h"
#include "utility.h"



class GridLayout
{
private:
    uint32 nbDims_ ;

    double dx_ ;        // mesh sizes
    double dy_ ;        // mesh sizes
    double dz_ ;        // mesh sizes

    double odx_ ;       // 1./dx
    double ody_ ;       // 1./dy
    double odz_ ;       // 1./dz

    uint32 nbrCellx_  ;
    uint32 nbrCelly_  ;
    uint32 nbrCellz_  ;

    uint32 ghostParameter_ ;

    std::unique_ptr<GridLayoutImpl> implPtr_;

    using error = std::runtime_error;
    static const std::string errorInverseMesh;


    // test the validity of the GridLayout construction
    void throwNotValid1D() const;
    void throwNotValid2D() const;
    void throwNotValid3D() const;


public:

    static const uint32 minNbrCells = 10; // minimum nbr of cells in a
                                          // non-invariant direction

    GridLayout(std::array<double,3> dxdydz, std::array<uint32,3> nbrCells,
               uint32 nbDims      , std::string layoutName,
               uint32 ghostParameter ); // TODO see if better name

    GridLayout(GridLayout const& source);
    GridLayout(GridLayout&& source);

    GridLayout& operator=(GridLayout const& source) = delete;
    GridLayout& operator=(GridLayout&& source) = delete;

    uint32 nbDimensions() const { return nbDims_ ; }

    double dx() const {return dx_;}
    double dy() const {return dy_;}
    double dz() const {return dz_;}



    double odx()const { return dx_ == 0. ? throw error(errorInverseMesh +" dz() (dz==0)"): odz_;}
    double ody()const { return dy_ == 0. ? throw error(errorInverseMesh +" dy() (dy==0)"): ody_;}
    double odz()const { return dz_ == 0. ? throw error(errorInverseMesh +" dz() (dz==0)"): odz_;}



    double nbrCellx() const {return nbrCellx_;}
    double nbrCelly() const {return nbrCelly_;}
    double nbrCellz() const {return nbrCellz_;}



    Point fieldNodeCoordinates( const Field & field, const Point & origin,
                                uint32 ix, uint32 iy, uint32 iz ) const;


    // return the (total) number of mesh points
    // for the 3 components of Ex, Ey, Ez      ( if EMFieldType==EVecField )
    // or for the 3 components of Bx, By, Bz   ( if EMFieldType==BVecField )
    //std::array<AllocSizeT, NBR_COMPO> allocSize( EMFieldType fieldType ) const ;
    //std::array<AllocSizeT, NBR_COMPO> allocSize( OhmTerm term ) const ;
    AllocSizeT allocSize(HybridQuantity qtyType) const;

    AllocSizeT  allocSizeDerived( HybridQuantity qty, Direction dir ) const ;


    uint32 physicalStartIndex(Field const& field, Direction direction) const;
    uint32 physicalEndIndex  (Field const& field, Direction direction) const;

    uint32 ghostStartIndex(Field const& field, Direction direction) const;
    uint32 ghostEndIndex  (Field const& field, Direction direction) const;

    void deriv(Field const& operand, Direction direction, Field& derivative)const;

};



#endif // GRIDLAYOUT_H
