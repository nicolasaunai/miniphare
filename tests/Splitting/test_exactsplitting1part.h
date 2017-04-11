#ifndef TEST_SPLITTING1PART_H
#define TEST_SPLITTING1PART_H

#include <array>

#include "types.h"


struct exactSplit1PartParams
{
    std::array<double, 3> dxdydz ;
    std::array<uint32, 3> nbrCells ;

    uint32 nbDim ;

    std::string layout ;

    Point origin{0., 0., 0.} ;

    uint32 interpOrder ;

    uint32 refineFactor ;

    std::string splitMethod ;

    uint32 referenceNode ;


    exactSplit1PartParams() = default ;

    exactSplit1PartParams( std::array<double, 3> dxdydz, std::array<uint32, 3> nbrCells,
                      uint32 nbDim, std::string layout,
                      Point origin, uint32 interpOrder,
                      uint32 refineFactor,
                      std::string splitMethod,
                      uint32 referenceNode ):
        dxdydz{dxdydz}, nbrCells{nbrCells},
        nbDim{nbDim}, layout{layout},
        origin{origin}, interpOrder{interpOrder},
        refineFactor{refineFactor},
        splitMethod{splitMethod},
        referenceNode{referenceNode} {}

    ~exactSplit1PartParams() = default ;

};


#endif // TEST_SPLITTING1PART_H
