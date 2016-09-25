#ifndef GRIDLAYOUTYEE_H
#define GRIDLAYOUTYEE_H


#include <vector>
#include <array>

#include "types.h"
#include "hybridenums.h"

#include "gridlayoutimpl.h"



/**
 * @brief GridLayoutImplYee class is a concrete GridLayoutImpl used a Yee
 * type grid layout is needed.
 *
 * It provides methods related to grid layout operations:
 * - physical domain start/end indexes
 * - indexes of the first and last ghost nodes
 * - allocation sizes for Field attributes of other classes
 * - partial derivative operator (Faraday)
 * - physical coordinate given a field and a primal point (ix, iy, iz)
 * - cell centered coordinate given a primal point (ix, iy, iz)
 *
 */
class GridLayoutImplYee : public GridLayoutImpl, private GridLayoutImplInternals
{
private:

    void initLayoutCentering( const gridDataT & staticData ) ;


public:

    explicit GridLayoutImplYee(uint32 nbDims, uint32 interpOrder,
                               std::array<uint32,3> nbrCellsXYZ ,
                               std::array<double,3> dxdydz      );

    virtual ~GridLayoutImplYee() = default;

    virtual uint32 nbDimensions() const override {return nbdims_;}


    // start and end index used in computing loops
    virtual uint32 physicalStartIndex(Field const& field, Direction direction) const override;
    virtual uint32 physicalEndIndex  (Field const& field, Direction direction) const override;

    virtual uint32 ghostStartIndex(Field const& field, Direction direction) const override;
    virtual uint32 ghostEndIndex  (Field const& field, Direction direction) const override;

    virtual AllocSizeT allocSize( HybridQuantity qtyType ) const override ;

    virtual AllocSizeT  allocSizeDerived( HybridQuantity qty, Direction dir ) const override ;

    virtual void deriv1D(Field const& operand, Field& derivative) const override;

    virtual Point fieldNodeCoordinates(
            const Field & field, const Point & origin,
            uint32 ix, uint32 iy, uint32 iz ) const override ;

    virtual Point cellCenteredCoordinates(
            const Point & origin, uint32 ix, uint32 iy, uint32 iz ) const override ;

};



#endif // GRIDLAYOUTYEE_H
