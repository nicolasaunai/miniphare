#ifndef GCA_H
#define GCA_H


#include <array>
#include <vector>

#include "data/grid/gridlayout.h"
#include "utilities/box.h"
#include "utilities/types.h"




struct GCA
{
    std::array<int32, 3> GCAwidth;

    std::vector<LogicalBox> logicDecomposition;
    std::vector<Box> boxDecomposition;

    GCA() {}

    GCA(std::array<int32, 3> GCAwidth, std::vector<LogicalBox> logicBoxes, std::vector<Box> boxes)
        : GCAwidth{GCAwidth}
        , logicDecomposition{logicBoxes}
        , boxDecomposition{boxes}
    {
    }
};



struct GCALimits
{
    std::array<int32, 2> innerLimitsIndexes;
    std::array<int32, 2> outerLimitsIndexes;
    std::array<double, 2> innerLimitsCoords;
    std::array<double, 2> outerLimitsCoords;
};



GCA buildGCA(GridLayout const& layout);
GridLayout buildGCABoundaryLayout(GCA const& refinedGCA, uint32 ibord,
                                  GridLayout const& refinedLayout);



#endif // GCA_H
