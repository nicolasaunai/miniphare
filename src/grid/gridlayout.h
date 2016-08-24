#ifndef GRIDLAYOUT_H
#define GRIDLAYOUT_H

#include<vector>

#include "types.h"


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

    uint32 nbdims_;


    // TODO attention nx est utilisé PARTOUT comme le nombre de POINTS dans les TABLEAUX
    // INCLUANT les ghosts, PRA etc. Il faudra les renommer pour rendre ça comprehensible...
    //e.g. allocSizeX, etc.
    // REVOIR tout le systeme d'indices.

    int32 nbrCellsX_ ;
    int32 nbrNodesX_ ;  // equals: nbrCellsX_ + 1

    // Indexes corresponding to the physical boundaries of the local domain,
    // ghost nodes are not included.
    // The boundaries including ghost nodes for a node centered field are given by :
    // [ nulmx - centeredOffset ; nulpx + centeredOffset ]
    uint32 nulmx_ ;
    uint32 nulpx_ ;

    std::vector<double> xsupport_ ;     // primal grid coordinates



    //void initDomain( double xminGlobal ) ;


public:
    GridLayout():dx_{0.1},dy_{0.1},dz_{0.1},
                 odx_{1./dx_},ody_{1./dy_},odz_{1./dz_},
                 nx_{10}, ny_{1}, nz_{1},nbdims_{1}
    {}

    //GridLayout( uint32 level, const GlobalParams & globalParams );

    //void  displayXSegment() ;


    double dx() const {return dx_;}
    double dy() const {return dy_;}
    double dz() const {return dz_;}

    double odx()const {return odx_;}
    double ody()const {return ody_;}
    double odz()const {return odz_;}

    uint32 nx()const {return nx_;}
    uint32 ny()const {return ny_;}
    uint32 nz()const {return nz_;}

    uint32 nbDimensions() const {return nbdims_;}


    int32  nbrCellsX() const { return nbrCellsX_ ; }

    int32  nbrNodesX() const { return nbrNodesX_ ; }

    uint32  indexXmin() const { return nulmx_ ; }
    uint32  indexXmax() const { return nulpx_ ; }

    std::vector<double>  XSegment() const { return xsupport_ ; }


    //friend std::ostream & operator << (std::ostream & ostr, GridLayout & layout) ;

};

#endif // GRIDLAYOUT_H
