#ifndef GRIDLAYOUTYEE_H
#define GRIDLAYOUTYEE_H


#include <vector>
#include <array>

#include "gridlayoutimpl.h"
#include "types.h"




class GridLayoutImplYee : public GridLayoutImpl, private GridLayoutImplInternals
{


private:

    // TODO attention nx est utilisé PARTOUT comme le nombre de POINTS dans les TABLEAUX
    // INCLUANT les ghosts, PRA etc. Il faudra les renommer pour rendre ça comprehensible...
    //e.g. allocSizeX, etc.
    // REVOIR tout le systeme d'indices.

#if 0
    int32 nbrCellsX_ ;
    int32 nbrNodesX_ ;  // equals: nbrCellsX_ + 1

    // Indexes corresponding to the physical boundaries of the local domain,
    // ghost nodes are not included.
    // The boundaries including ghost nodes for a node centered field are given by :
    // [ nulmx - centeredOffset ; nulpx + centeredOffset ]
    uint32 nulmx_ ;
    uint32 nulpx_ ;

    std::vector<double> xsupport_ ;     // primal grid coordinates
#endif


    //void initDomain( double xminGlobal ) ;


public:

    explicit GridLayoutImplYee(uint32 nbDims,
                               uint32 interpOrder,
                               std::array<uint32,3> nbrCellsXYZ);
    virtual ~GridLayoutImplYee() = default;


#if 0



    int32  nbrCellsX() const { return nbrCellsX_ ; }

    int32  nbrNodesX() const { return nbrNodesX_ ; }

    uint32  indexXmin() const { return nulmx_ ; }
    uint32  indexXmax() const { return nulpx_ ; }

    std::vector<double>  XSegment() const { return xsupport_ ; }
#endif

    virtual uint32 nbDimensions() const override {return nbDimensions_();}

    virtual uint32 nx(uint32 nbrCellx) const override;
    virtual uint32 ny(uint32 nbrCelly) const override;
    virtual uint32 nz(uint32 nbrCellz) const override;

    // start and end index used in computing loops
    virtual uint32 physicalStartIndex(Field const& field, uint32 direction) const override;
    virtual uint32 physicalEndIndex  (Field const& field, uint32 direction) const override;

    virtual uint32 ghostStartIndex(Field const& field, uint32 direction) const override;
    virtual uint32 ghostEndIndex  (Field const& field, uint32 direction) const override;

    virtual void deriv(Field const& operand, uint32 direction, Field& derivative) const override;


};



#endif // GRIDLAYOUTYEE_H
