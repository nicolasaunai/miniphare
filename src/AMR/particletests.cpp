#include "particletests.h"




bool isInSpecifiedBox(GridLayout const& partLayout, Particle const& child,
                      GridLayout const& targetLayout)
{
    Box targetBox{targetLayout.getBox()};
    Box partBox{partLayout.getBox()};

    double dx = partLayout.dx();
    double dy = partLayout.dy();
    double dz = partLayout.dz();

    double part_x0 = partBox.x0;
    double part_y0 = partBox.y0;
    double part_z0 = partBox.z0;

    int32 nbrGhosts = static_cast<int32>(partLayout.nbrGhostNodes(QtyCentering::primal));

    double posx = (static_cast<int32>(child.icell[0]) - nbrGhosts + child.delta[0]) * dx + part_x0;
    double posy = (static_cast<int32>(child.icell[1]) - nbrGhosts + child.delta[1]) * dy + part_y0;
    double posz = (static_cast<int32>(child.icell[2]) - nbrGhosts + child.delta[2]) * dz + part_z0;

    double xlower = targetBox.x0;
    double xupper = targetBox.x1;
    double ylower = targetBox.y0;
    double yupper = targetBox.y1;
    double zlower = targetBox.z0;
    double zupper = targetBox.z1;

    // return true if the particle is in the box
    return (posx >= xlower && posx <= xupper && posy >= ylower && posy <= yupper && posz >= zlower
            && posz <= zupper);
}




bool isInRefinedBox(GridLayout const& refinedLayout, VirtualParticle const& child)
{
    Box refinedBox{refinedLayout.getBox()};

    double dx = refinedLayout.dx();
    double dy = refinedLayout.dy();
    double dz = refinedLayout.dz();

    double x0 = refinedBox.x0;
    double y0 = refinedBox.y0;
    double z0 = refinedBox.z0;

    int32 nbrGhosts = static_cast<int32>(refinedLayout.nbrGhostNodes(QtyCentering::primal));

    double posx = static_cast<double>(child.icell[0] - nbrGhosts + child.delta[0]) * dx + x0;
    double posy = static_cast<double>(child.icell[1] - nbrGhosts + child.delta[1]) * dy + y0;
    double posz = static_cast<double>(child.icell[2] - nbrGhosts + child.delta[2]) * dz + z0;

    double xlower = refinedBox.x0;
    double xupper = refinedBox.x1;
    double ylower = refinedBox.y0;
    double yupper = refinedBox.y1;
    double zlower = refinedBox.z0;
    double zupper = refinedBox.z1;

    // return true if the particle is in the box
    return (posx >= xlower && posx <= xupper && posy >= ylower && posy <= yupper && posz >= zlower
            && posz <= zupper);
}
