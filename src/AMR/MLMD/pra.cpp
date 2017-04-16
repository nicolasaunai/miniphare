#include "pra.h"





uint32 PRAHalfWidth(GridLayout const& layout)
{
    return layout.nbrGhostCells(QtyCentering::primal) ;
}





/**
 * @brief MLMDInitializerFactory::definePRA1Dlimits_ is in charge of
 * computing indexes - and the associated coordinates - defining a PRA
 * in a prescribed direction.
 * A PRA can be defined knowing an inner and an outer box
 * (see https://hephaistos.lpp.polytechnique.fr/redmine/projects/hyb-par/wiki/FieldBCs#PRA-and-the-non-connexe-topology-issue)
 *
 *
 * @param layout
 * @param direction
 * @param nbrMaxGhost
 */
PRALimits getPRAlimits(GridLayout const & layout,
                        Direction direction,
                        uint32 praHalfWidth)
{
    PRALimits limits;
    auto& innerLimitsIndexes = limits.innerLimitsIndexes;
    auto& outerLimitsIndexes = limits.outerLimitsIndexes;
    auto& innerLimitsCoords = limits.innerLimitsCoords;
    auto& outerLimitsCoords = limits.outerLimitsCoords;
    uint32 start=0, end=1;
    uint32 physicalStartIndex;
    uint32 physicalEndIndex;
    double origin = layout.origin().getCoord(static_cast<uint32>(direction));


    physicalStartIndex = layout.physicalStartIndex(QtyCentering::primal, direction);
    physicalEndIndex   = layout.physicalEndIndex(QtyCentering::primal, direction);

    innerLimitsIndexes[start] = physicalStartIndex + praHalfWidth ;
    innerLimitsIndexes[end]   = physicalEndIndex   - praHalfWidth ;
    outerLimitsIndexes[start] = physicalStartIndex - praHalfWidth ;
    outerLimitsIndexes[end]   = physicalEndIndex   + praHalfWidth ;
    innerLimitsCoords[start]  = innerLimitsIndexes[0] * layout.dx() + origin;
    innerLimitsCoords[end]    = innerLimitsIndexes[1] * layout.dx() + origin;
    outerLimitsCoords[start]  = outerLimitsIndexes[0] * layout.dx() + origin;
    outerLimitsCoords[end]    = outerLimitsIndexes[1] * layout.dx() + origin;

    return limits;
}








/**
 * @brief MLMDInitializerFactory::buildPRABoundaryLayout_ has to
 * return a valid GridLayout corrsponding to a part of a complete PRA.
 * In 1D, we have 2 possible boundaries.
 * In 2D, we have 4 possible boundaries.
 * In 3D, we have 6 possible boundaries.
 *
 * @param refinedPRA
 * @param ibord
 * @return
 */
GridLayout buildPRABoundaryLayout(PRA const& refinedPRA,
                                   uint32 ibord,
                                   GridLayout const& refinedLayout)
{
    std::array<double,3> dxdydz = refinedLayout.dxdydz();
    uint32 nbDims = refinedLayout.nbDimensions() ;
    std::string layoutName = refinedLayout.layoutName() ;
    uint32 interpOrder = refinedLayout.order() ;

    LogicalBox logic = refinedPRA.logicDecomposition[ibord] ;
    Box box = refinedPRA.boxDecomposition[ibord] ;

    Point origin{box.x0, box.y0, box.z0} ;

    uint32 nbrCellx = logic.ix1 - logic.ix0 ;
    uint32 nbrCelly = logic.iy1 - logic.iy0 ;
    uint32 nbrCellz = logic.iz1 - logic.iz0 ;

    std::array<uint32,3> nbrCells{ {nbrCellx, nbrCelly, nbrCellz} } ;

    return  GridLayout(dxdydz, nbrCells,
                       nbDims, layoutName,
                       origin, interpOrder);
}








/**
 * @brief MLMDInitializerFactory::buildPRA1D_ returns a simple
 * 1D PRA made of 2 segments
 *
 * @param layout
 * @return
 */
PRA buildPRA1D( GridLayout const & layout )
{
    std::array<uint32, 3> PRAwidth{ { 2*PRAHalfWidth(layout), 0, 0 } } ;

    PRALimits limits = getPRAlimits(layout, Direction::X, PRAHalfWidth(layout));

    std::vector<LogicalBox> logicBoxes = { {limits.outerLimitsIndexes[0],
                                            limits.innerLimitsIndexes[0]},
                                           {limits.innerLimitsIndexes[1],
                                            limits.outerLimitsIndexes[1]}};

    std::vector<Box> boxes = { {limits.outerLimitsCoords[0],
                                limits.innerLimitsCoords[0]},
                               {limits.innerLimitsCoords[1],
                                limits.outerLimitsCoords[1]}};

    return PRA{PRAwidth, logicBoxes, boxes} ;
}








/**
 * @brief MLMDInitializerFactory::buildPRA2D_ returns a 2D PRA
 * made of 4 rectangle areas
 *
 *
 * @param layout
 * @return
 */
PRA buildPRA2D(GridLayout const& layout)
{

    std::array<uint32, 3> PRAwidth{{2*PRAHalfWidth(layout), 2*PRAHalfWidth(layout), 0}};

    PRALimits xLimits = getPRAlimits(layout, Direction::X, PRAHalfWidth(layout));
    PRALimits yLimits = getPRAlimits(layout, Direction::Y, PRAHalfWidth(layout));

    // Build logic (primal) decomposition
    // TODO: provide a link to redmine with a drawing
    // to check rapidly the points
                                            // box at x=xmin
    std::vector<LogicalBox> logicBoxes = { {xLimits.outerLimitsIndexes[0],
                                            xLimits.innerLimitsIndexes[0],
                                            yLimits.outerLimitsIndexes[0],
                                            yLimits.outerLimitsIndexes[1]
                                           },
                                           // box at x=xmax
                                           {xLimits.innerLimitsIndexes[1],
                                            xLimits.outerLimitsIndexes[1],
                                            yLimits.outerLimitsIndexes[0],
                                            yLimits.outerLimitsIndexes[1]
                                           },
                                           // box at y=ymin
                                           {xLimits.innerLimitsIndexes[0],
                                            xLimits.innerLimitsIndexes[1],
                                            yLimits.outerLimitsIndexes[0],
                                            yLimits.innerLimitsIndexes[0]
                                           },
                                           // box at y=ymax
                                           {xLimits.innerLimitsIndexes[0],
                                            xLimits.innerLimitsIndexes[1],
                                            yLimits.innerLimitsIndexes[1],
                                            yLimits.outerLimitsIndexes[1]
                                           }
                                         };

                                // box at x=xmin
    std::vector<Box> boxes = { {xLimits.outerLimitsCoords[0],
                                xLimits.innerLimitsCoords[0],
                                yLimits.outerLimitsCoords[0],
                                yLimits.outerLimitsCoords[1]
                               },
                               // box at x=xmax
                               {xLimits.innerLimitsCoords[1],
                                xLimits.outerLimitsCoords[1],
                                yLimits.outerLimitsCoords[0],
                                yLimits.outerLimitsCoords[1]
                               },
                               // box at y=ymin
                               {xLimits.innerLimitsCoords[0],
                                xLimits.innerLimitsCoords[1],
                                yLimits.outerLimitsCoords[0],
                                yLimits.innerLimitsCoords[0]
                               },
                               // box at y=ymax
                               {xLimits.innerLimitsCoords[0],
                                xLimits.innerLimitsCoords[1],
                                yLimits.innerLimitsCoords[1],
                                yLimits.outerLimitsCoords[1]
                               }
                            };

    return PRA{PRAwidth, logicBoxes, boxes} ;
}






/**
 * @brief MLMDInitializerFactory::buildPRA3D_ returns a 3D PRA
 * made of 6 cuboid volumes
 *
 *
 * @param layout
 * @return
 */
PRA buildPRA3D(GridLayout const & layout)
{
    std::array<uint32, 3> PRAwidth{{2*PRAHalfWidth(layout), 2*PRAHalfWidth(layout), 2*PRAHalfWidth(layout)}};

    PRALimits xLimits = getPRAlimits(layout, Direction::X, PRAHalfWidth(layout));
    PRALimits yLimits = getPRAlimits(layout, Direction::Y, PRAHalfWidth(layout));
    PRALimits zLimits = getPRAlimits(layout, Direction::Z, PRAHalfWidth(layout));

    // Build logic (primal) decomposition
    // TODO: 3D generalization
    // TODO: provide a link to redmine with a drawing
    // to check rapidly the points
    std::vector<LogicalBox> logicBoxes;
    std::vector<Box> boxes;

    return PRA{PRAwidth, logicBoxes, boxes} ;
}









/**
 * @brief MLMDInitializerFactory::buildPRA_ has to return a valid
 * PRA depending on the dimension.
 * In 1D, the PRA splits into 2 segments.
 * In 2D, the PRA splits into 4 rectangle areas.
 * In 3D, the PRA splits into 6 cuboid volumes.
 *
 * @param layout
 * @return
 */
PRA buildPRA(GridLayout const& layout)
{
    switch (layout.nbDimensions())
    {
    case 1:
        return buildPRA1D(layout);

    case 2:
        return buildPRA2D(layout);

    case 3:
        return buildPRA3D(layout);

    default:
        throw std::runtime_error("wrong dimensionality");
    }
}





