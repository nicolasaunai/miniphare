#include "gca.h"


// Here we set the number of complete nodes for
// any arbitrary direction of the GCA
//
// This number should be small to limit computational cost
static const int32 GCAcompleteNodeNbr = 5;

// ideally this should depend on the interpolation order
// Here, we overestimate the number of incomplete nodes
// 3 is ok to handle any order from 1st to 4th
static const int32 MAXincompleteNodeNbr = 3;



std::array<int32, 3> computeGCACellNumbers(GridLayout const& layout)
{
    int32 cellNumber = 2 * MAXincompleteNodeNbr + GCAcompleteNodeNbr - 1;

    switch (layout.nbDimensions())
    {
        case 1: return {{cellNumber, 0, 0}};

        case 2: return {{cellNumber, cellNumber, 0}};

        case 3: return {{cellNumber, cellNumber, cellNumber}};

        default: throw std::runtime_error("Fatal Error : computeGCACellNumbers");
    }
}


/**
 * @brief MLMDInitializerFactory::defineGCA1Dlimits_ is in charge of
 * computing indexes - and the associated coordinates - defining a GCA
 * in a prescribed direction.
 * A GCA can be defined knowing an inner and an outer box
 * (see
 * https://hephaistos.lpp.polytechnique.fr/redmine/projects/hyb-par/wiki/FieldBCs#GCA-and-the-non-connexe-topology-issue)
 *
 *
 * @param layout of the refined patch
 * @param direction
 * @param nbrMaxGhost
 */
GCALimits getGCAlimits(GridLayout const& layout, Direction direction)
{
    GCALimits limits;
    auto& innerLimitsIndexes = limits.innerLimitsIndexes;
    auto& outerLimitsIndexes = limits.outerLimitsIndexes;
    auto& innerLimitsCoords  = limits.innerLimitsCoords;
    auto& outerLimitsCoords  = limits.outerLimitsCoords;
    uint32 start = 0, end = 1;

    double origin = layout.origin().getCoord(static_cast<uint32>(direction));

    //    innerLimitsIndexes[start] = GCAincompleteNodeNbr + GCAphysicalNodeNbr - 1;
    //    innerLimitsIndexes[end]
    //        = static_cast<int32>(layout.nbrCellx()) - GCAphysicalNodeNbr - GCAincompleteNodeNbr +
    //        1;
    //    outerLimitsIndexes[start] = -GCAincompleteNodeNbr;
    //    outerLimitsIndexes[end]   = static_cast<int32>(layout.nbrCellx()) + GCAincompleteNodeNbr;

    outerLimitsIndexes[start] = -2 * MAXincompleteNodeNbr - GCAcompleteNodeNbr + 1;
    innerLimitsIndexes[start] = 0;
    innerLimitsIndexes[end]   = static_cast<int32>(layout.nbrCellx());
    outerLimitsIndexes[end]
        = static_cast<int32>(layout.nbrCellx()) + 2 * MAXincompleteNodeNbr + GCAcompleteNodeNbr - 1;

    innerLimitsCoords[start] = innerLimitsIndexes[0] * layout.dx() + origin;
    innerLimitsCoords[end]   = innerLimitsIndexes[1] * layout.dx() + origin;
    outerLimitsCoords[start] = outerLimitsIndexes[0] * layout.dx() + origin;
    outerLimitsCoords[end]   = outerLimitsIndexes[1] * layout.dx() + origin;

    return limits;
}




/**
 * @brief MLMDInitializerFactory::buildGCA1D_ returns a simple
 * 1D GCA made of 2 segments
 *
 * @param layout
 * @return
 */
GCA buildGCA1D(GridLayout const& layout)
{
    std::array<int32, 3> GCAwidth{computeGCACellNumbers(layout)};

    GCALimits limits = getGCAlimits(layout, Direction::X);

    std::vector<LogicalBox> logicBoxes
        = {{limits.outerLimitsIndexes[0], limits.innerLimitsIndexes[0]},
           {limits.innerLimitsIndexes[1], limits.outerLimitsIndexes[1]}};

    std::vector<Box> boxes = {{limits.outerLimitsCoords[0], limits.innerLimitsCoords[0]},
                              {limits.innerLimitsCoords[1], limits.outerLimitsCoords[1]}};

    return GCA{GCAwidth, logicBoxes, boxes};
}




/**
 * @brief MLMDInitializerFactory::buildGCA2D_ returns a 2D GCA
 * made of 4 rectangle areas
 *
 *
 * @param layout
 * @return
 */
GCA buildGCA2D(GridLayout const& layout)
{
    std::array<int32, 3> GCAwidth{computeGCACellNumbers(layout)};

    GCALimits xLimits = getGCAlimits(layout, Direction::X);
    GCALimits yLimits = getGCAlimits(layout, Direction::Y);

    // Build logic (primal) decomposition
    // TODO: provide a link to redmine with a drawing
    // to check rapidly the points
    // box at x=xmin
    std::vector<LogicalBox> logicBoxes
        = {{xLimits.outerLimitsIndexes[0], xLimits.innerLimitsIndexes[0],
            yLimits.outerLimitsIndexes[0], yLimits.outerLimitsIndexes[1]},
           // box at x=xmax
           {xLimits.innerLimitsIndexes[1], xLimits.outerLimitsIndexes[1],
            yLimits.outerLimitsIndexes[0], yLimits.outerLimitsIndexes[1]},
           // box at y=ymin
           {xLimits.innerLimitsIndexes[0], xLimits.innerLimitsIndexes[1],
            yLimits.outerLimitsIndexes[0], yLimits.innerLimitsIndexes[0]},
           // box at y=ymax
           {xLimits.innerLimitsIndexes[0], xLimits.innerLimitsIndexes[1],
            yLimits.innerLimitsIndexes[1], yLimits.outerLimitsIndexes[1]}};

    // box at x=xmin
    std::vector<Box> boxes = {{xLimits.outerLimitsCoords[0], xLimits.innerLimitsCoords[0],
                               yLimits.outerLimitsCoords[0], yLimits.outerLimitsCoords[1]},
                              // box at x=xmax
                              {xLimits.innerLimitsCoords[1], xLimits.outerLimitsCoords[1],
                               yLimits.outerLimitsCoords[0], yLimits.outerLimitsCoords[1]},
                              // box at y=ymin
                              {xLimits.innerLimitsCoords[0], xLimits.innerLimitsCoords[1],
                               yLimits.outerLimitsCoords[0], yLimits.innerLimitsCoords[0]},
                              // box at y=ymax
                              {xLimits.innerLimitsCoords[0], xLimits.innerLimitsCoords[1],
                               yLimits.innerLimitsCoords[1], yLimits.outerLimitsCoords[1]}};

    return GCA{GCAwidth, logicBoxes, boxes};
}




/**
 * @brief MLMDInitializerFactory::buildGCA3D_ returns a 3D GCA
 * made of 6 cuboid volumes
 *
 *
 * @param layout
 * @return
 */
GCA buildGCA3D(GridLayout const& layout)
{
    std::array<int32, 3> GCAwidth{computeGCACellNumbers(layout)};

    // Commented to prevent Warning
    //    GCALimits xLimits = getGCAlimits(layout, Direction::X);
    //    GCALimits yLimits = getGCAlimits(layout, Direction::Y);
    //    GCALimits zLimits = getGCAlimits(layout, Direction::Z);

    // Build logic (primal) decomposition
    // TODO: 3D generalization
    // TODO: provide a link to redmine with a drawing
    // to check rapidly the points
    std::vector<LogicalBox> logicBoxes;
    std::vector<Box> boxes;

    return GCA{GCAwidth, logicBoxes, boxes};
}




/**
 * @brief MLMDInitializerFactory::buildGCA_ has to return a valid
 * GCA depending on the dimension.
 * In 1D, the GCA splits into 2 segments.
 * In 2D, the GCA splits into 4 rectangle areas.
 * In 3D, the GCA splits into 6 cuboid volumes.
 *
 * @param layout
 * @return
 */
GCA buildGCA(GridLayout const& layout)
{
    switch (layout.nbDimensions())
    {
        case 1: return buildGCA1D(layout);

        case 2: return buildGCA2D(layout);

        case 3: return buildGCA3D(layout);

        default: throw std::runtime_error("wrong dimensionality");
    }
}




/**
 * @brief MLMDInitializerFactory::buildGCABoundaryLayout_ has to
 * return a valid GridLayout corrsponding to a part of a complete GCA.
 * In 1D, we have 2 possible boundaries.
 * In 2D, we have 4 possible boundaries.
 * In 3D, we have 6 possible boundaries.
 *
 * @param refinedGCA
 * @param ibord
 * @return
 */
GridLayout buildGCABoundaryLayout(GCA const& refinedGCA, uint32 ibord,
                                  GridLayout const& refinedLayout)
{
    std::array<double, 3> dxdydz = refinedLayout.dxdydz();
    uint32 nbDims          = refinedLayout.nbDimensions();
    std::string layoutName = refinedLayout.layoutName();
    uint32 interpOrder     = refinedLayout.order();

    LogicalBox logic = refinedGCA.logicDecomposition[ibord];
    Box box          = refinedGCA.boxDecomposition[ibord];

    Point origin{box.x0, box.y0, box.z0};

    uint32 nbrCellx = logic.ix1 - logic.ix0;
    uint32 nbrCelly = logic.iy1 - logic.iy0;
    uint32 nbrCellz = logic.iz1 - logic.iz0;

    std::array<uint32, 3> nbrCells{{nbrCellx, nbrCelly, nbrCellz}};

    return GridLayout(dxdydz, nbrCells, nbDims, layoutName, origin, interpOrder);
}
