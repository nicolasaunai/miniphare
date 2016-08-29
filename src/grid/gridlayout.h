#ifndef GRIDLAYOUT_H
#define GRIDLAYOUT_H

#include <vector>
#include <array>
#include <memory>

#include "types.h"
#include "Field/field.h"




class GridLayoutImpl
{

public:


    // start and end index used in computing loops
    virtual uint32 physicalStartIndex(Field const& field, uint32 direction) const = 0;
    virtual uint32 physicalEndIndex  (Field const& field, uint32 direction) const = 0;

    virtual uint32 ghostStartIndex(Field const& field, uint32 direction) const = 0;
    virtual uint32 ghostEndIndex  (Field const& field, uint32 direction) const = 0;

    virtual void deriv(Field const& operand, uint32 direction, Field& derivative)const = 0;

    virtual uint32 nbDimensions() const = 0;

    virtual ~GridLayoutImpl() = default;

};




class GridLayoutImplInternals
{
protected:

    uint32 nbdims_;

public:
    explicit GridLayoutImplInternals(uint32 nbDims);
    uint32 nbDimensions_() const  {return nbdims_;}
};






class GridLayout
{
private:
    double dx_ ;        // mesh sizes
    double dy_ ;        // mesh sizes
    double dz_ ;        // mesh sizes

    double odx_ ;       // 1./dx
    double ody_ ;       // 1./dy
    double odz_ ;       // 1./dz

    uint32 nx_  ;
    uint32 ny_  ;
    uint32 nz_  ;

    std::unique_ptr<GridLayoutImpl> implPtr_;


public:

    static const uint32 directionX = 0;
    static const uint32 directionY = 1;
    static const uint32 directionZ = 2;


    GridLayout();
    GridLayout(GridLayout const& source);
    GridLayout(GridLayout&& source);

    double dx() const {return dx_;}
    double dy() const {return dy_;}
    double dz() const {return dz_;}

    double odx()const {return odx_;}
    double ody()const {return ody_;}
    double odz()const {return odz_;}

    uint32 nx()const {return nx_;}
    uint32 ny()const {return ny_;}
    uint32 nz()const {return nz_;}



    uint32 physicalStartIndex(Field const& field, uint32 direction) const;
    uint32 physicalEndIndex  (Field const& field, uint32 direction) const;

    uint32 ghostStartIndex(Field const& field, uint32 direction) const;
    uint32 ghostEndIndex  (Field const& field, uint32 direction) const;

    void deriv(Field const& operand, uint32 direction, Field& derivative)const;

    uint32 nbDimensions() const;
};





#endif // GRIDLAYOUT_H
