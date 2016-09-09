#ifndef GRIDLAYOUTYEE_H
#define GRIDLAYOUTYEE_H


#include <vector>
#include <array>

#include "types.h"
#include "gridconstants.h"

#include "gridlayoutimpl.h"





class GridLayoutImplYee : public GridLayoutImpl, private GridLayoutImplInternals
{
private:



public:

    explicit GridLayoutImplYee(uint32 nbDims, uint32 interpOrder,
                               std::array<uint32,3> nbrCellsXYZ ,
                               std::array<double,3> dxdydz      );

    virtual ~GridLayoutImplYee() = default;

    virtual uint32 nbDimensions() const override {return nbDimensions_();}

    virtual std::vector < std::tuple < uint32, Point> >
    fieldNodeCoordinates1D( const Field & field, const Point & patchOrigin ) const override ;



    virtual std::array<AllocSizeT ,3> allocSize( EMFieldType fieldType ) const override ;

    virtual std::array<AllocSizeT ,3> allocSize( OhmTerm term ) const override ;

    virtual AllocSizeT allocSize( DerivedEMField derivedField ) const override ;


    // start and end index used in computing loops
    virtual uint32 physicalStartIndex(Field const& field, Direction direction) const override;
    virtual uint32 physicalEndIndex  (Field const& field, Direction direction) const override;

    virtual uint32 ghostStartIndex(Field const& field, Direction direction) const override;
    virtual uint32 ghostEndIndex  (Field const& field, Direction direction) const override;

    virtual void deriv(Field const& operand, Direction direction, Field& derivative) const override;


};



#endif // GRIDLAYOUTYEE_H
