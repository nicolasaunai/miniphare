#ifndef TEST_SPLITTING1PART_H
#define TEST_SPLITTING1PART_H

#include <array>

#include "types.h"


struct Split1PartParams
{
    std::array<double, 3> dxdydz ;
    std::array<uint32, 3> nbrCells ;

    uint32 nbDim ;

    std::string layout ;

    Point origin{0., 0., 0.} ;

    uint32 interpOrder ;

    uint32 referenceNode ;


    Split1PartParams() = default ;

    Split1PartParams( std::array<double, 3> dxdydz, std::array<uint32, 3> nbrCells,
                      uint32 nbDim, std::string layout,
                      Point origin, uint32 interpOrder,
                      uint32 referenceNode ):
        dxdydz{dxdydz}, nbrCells{nbrCells},
        nbDim{nbDim}, layout{layout},
        origin{origin}, interpOrder{interpOrder},
        referenceNode{referenceNode} {}

    ~Split1PartParams() = default ;

};


#endif // TEST_SPLITTING1PART_H
