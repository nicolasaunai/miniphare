#ifndef PRA_H
#define PRA_H


#include <vector>
#include <array>

#include "types.h"
#include "utilityphare.h"
#include "grid/gridlayout.h"


struct PRA
{
    std::array<uint32, 3> PRAwidth ;

    std::vector<LogicalBox> logicDecomposition;
    std::vector<Box> boxDecomposition;

    PRA() {}

    PRA( std::array<uint32, 3> PRAwidth,
         std::vector<LogicalBox> logicBoxes,
         std::vector<Box> boxes )
        : PRAwidth{PRAwidth},
          logicDecomposition{logicBoxes},
          boxDecomposition{boxes} {}
};


struct PRALimits
{
    std::array<uint32, 2> innerLimitsIndexes;
    std::array<uint32, 2> outerLimitsIndexes;
    std::array<double, 2> innerLimitsCoords;
    std::array<double, 2> outerLimitsCoords;
};




PRA buildPRA(GridLayout const& layout);
GridLayout buildPRABoundaryLayout(PRA const& refinedPRA, uint32 ibord,
                                  GridLayout const& refinedLayout);



#endif // PRA_H
