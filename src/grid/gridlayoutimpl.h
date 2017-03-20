#ifndef GRIDLAYOUTIMPL_H
#define GRIDLAYOUTIMPL_H

#include <array>

#include "constants.h"
#include "types.h"
#include "hybridenums.h"

#include "Field/field.h"
#include "gridlayoutdefs.h"


struct WeightPoint
{
    int32 ix, iy, iz;
    double coef;
};


using LinearCombination = std::vector<WeightPoint>;





/**
 * @brief GridLayoutImpl is a virtual pure class, it declares interface
 * prototypes publically inherited by GridLayoutImplYee.
 *
 * The virtual methods are related to grid layout operations:
 * - physical domain start/end indexes
 * - indexes of the first and last ghost nodes
 * - allocation sizes for Field attributes of other classes
 * - partial derivative operator (Faraday)
 * - physical coordinate given a field and a primal point (ix, iy, iz)
 * - cell centered coordinate given a primal point (ix, iy, iz)
 *
 */
class GridLayoutImpl
{

public:

    // start and end index used in computing loops
    virtual uint32 physicalStartIndex(Field const& field, Direction direction) const = 0;
    virtual uint32 physicalStartIndex( QtyCentering centering, Direction direction     ) const = 0 ;

    virtual uint32 physicalEndIndex  (Field const& field, Direction direction) const = 0;
    virtual uint32 physicalEndIndex( QtyCentering centering, Direction direction     ) const = 0 ;

    virtual uint32 ghostStartIndex(Field const& field, Direction direction) const = 0;
    virtual uint32 ghostEndIndex  (Field const& field, Direction direction) const = 0;

    virtual AllocSizeT allocSize( HybridQuantity qtyType ) const = 0 ;
    virtual AllocSizeT  allocSizeDerived( HybridQuantity qty, Direction dir ) const = 0 ;

    virtual void deriv1D(Field const& operand, Field& derivative)const = 0;
    //virtual void deriv2D(Field const& operand, Direction direction, Field& derivative)const = 0;
    //virtual void deriv3D(Field const& operand, Direction direction, Field& derivative)const = 0;
    virtual Point fieldNodeCoordinates( const Field & field, const Point & origin,
                                        uint32 ix, uint32 iy, uint32 iz ) const = 0;

    virtual Point cellCenteredCoordinates(uint32 ix, uint32 iy, uint32 iz ) const = 0 ;
    virtual QtyCentering fieldCentering(Field const & field, Direction dir) const = 0 ;

    virtual uint32 nbrGhostCells( QtyCentering centering ) const = 0 ;
    virtual uint32 nbDimensions() const = 0;

    virtual ~GridLayoutImpl() = default;


    virtual LinearCombination const& momentsToEx() const = 0;
    virtual LinearCombination const& momentsToEy() const = 0;
    virtual LinearCombination const& momentsToEz() const = 0;

    virtual LinearCombination const& ByToEx() const = 0;
    virtual LinearCombination const& ByToEz() const = 0;

    virtual LinearCombination const& BxToEy() const = 0;
    virtual LinearCombination const& BxToEz() const = 0;

    virtual LinearCombination const& BzToEx() const = 0;
    virtual LinearCombination const& BzToEy() const = 0;

    virtual LinearCombination const& ExToMoment() const = 0;
    virtual LinearCombination const& EyToMoment() const = 0;
    virtual LinearCombination const& EzToMoment() const = 0;


};





#endif // GRIDLAYOUTIMPL_H
