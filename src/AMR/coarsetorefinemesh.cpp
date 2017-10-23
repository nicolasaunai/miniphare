
#include "AMR/coarsetorefinemesh.h"




void getClosestGridNode1D(Point const& point, GridLayout const& targetLayout,
                          QtyCentering const& fieldCtr, uint32& iNode)
{
    double x  = point.x;
    double x0 = targetLayout.origin().x;
    double dx = targetLayout.dx();

    double nbrGhosts = static_cast<double>(targetLayout.nbrGhostNodes(fieldCtr));
    double delta     = 0.;

    if (fieldCtr == QtyCentering::dual)
        delta = 0.5;

    iNode = static_cast<uint32>(std::round((x - x0) / dx + nbrGhosts - delta));
}



Field buildAlignedChildField1D(Field const& childField, GridLayout const& childLayout,
                               uint32 const& nbrNodesTarget, uint32 const& refineRatio)
{
    QtyCentering fieldCtr = childLayout.fieldCentering(childField, Direction::X);


    std::array<uint32, 3> nxyz = {{nbrNodesTarget, 1, 1}};
    Field alignedField(AllocSizeT(nxyz[0], nxyz[1], nxyz[2]), childField.hybridQty(),
                       childField.name() + "_aligned");

    if (fieldCtr == QtyCentering::primal)
    {
        uint32 iChild = childLayout.physicalStartIndex(childField, Direction::X);
        for (uint32 ik = 0; ik < nxyz[0]; ++ik)
        {
            alignedField(ik) = childField(iChild);

            iChild += refineRatio;
        }
    }
    else
    // if childField is defined on a dual mesh
    // we need to average points to get alignement with
    // the parent dual mesh nodes
    {
        uint32 iChild = childLayout.physicalStartIndex(childField, Direction::X);
        for (uint32 ik = 0; ik < nxyz[0]; ++ik)
        {
            double averagedField = 0.;
            for (uint32 iav = 0; iav < refineRatio; ++iav)
                averagedField += childField(iChild + iav);

            averagedField /= static_cast<double>(refineRatio);

            alignedField(ik) = averagedField;

            // TODO: replace by refinementRatio
            iChild += refineRatio;
        }
    }



    return alignedField;
}


/* ----------------------------------------------------------------------------

                      Field interpolations at refined nodes

   ---------------------------------------------------------------------------- */



/**
 * @brief The fieldAtRefinedNodes1D method is used to interpolate the fields
 * from a coarse patch into a refined patch, in the region where the two patches
 * overlap.
 *
 * This algorithm is quite similar to fieldAtParticle1D(...) method,
 * the particleField variable used in the latter method is replaced
 * by the variable refinedField in the present method.
 * But we perform the same kind of operations, we want to know the
 * interpolated value of a field at a specific point (.i.e. particle position)
 * using the coarse grid information.
 *
 *
 * @param interp
 * @param parentLayout
 * @param Eparent
 * @param newLayout
 * @param newE
 */
void fieldAtRefinedNodes1D(Interpolator& interp, GridLayout const& coarseLayout,
                           VecField const& Fcoarse, GridLayout const& refinedLayout,
                           VecField& Frefined)
{
    uint32 idirX = static_cast<uint32>(Direction::X);
    uint32 idirY = static_cast<uint32>(Direction::Y);
    uint32 idirZ = static_cast<uint32>(Direction::Z);

    Field const& Fx = Fcoarse.component(idirX);
    Field const& Fy = Fcoarse.component(idirY);
    Field const& Fz = Fcoarse.component(idirZ);

    std::array<std::reference_wrapper<Field const>, 3> FxyzCoarse = {{Fx, Fy, Fz}};

    Field& FxNew = Frefined.component(idirX);
    Field& FyNew = Frefined.component(idirY);
    Field& FzNew = Frefined.component(idirZ);

    std::array<std::reference_wrapper<Field>, 3> FxyzRefined = {{FxNew, FyNew, FzNew}};

    double dx_refined = refinedLayout.dx();
    double dx_coarse  = coarseLayout.dx();

    double refinement = dx_refined / dx_coarse;

    // (1) ==> Number of cells between coarse and refined layout origin points
    double newOriginReducedOnCoarse
        = std::fabs(refinedLayout.origin().x - coarseLayout.origin().x) / dx_coarse;

    // (2) ==> Number of cells between 1st ghost point and coarse layout origin
    double delta_originCoarse
        = static_cast<double>(refinedLayout.nbrGhostNodes(QtyCentering::primal));

    // (1) + (2)
    newOriginReducedOnCoarse += delta_originCoarse;


    // loop on Electric field components
    for (uint32 ifield = 0; ifield < FxyzCoarse.size(); ++ifield)
    {
        Field const& coarseField = FxyzCoarse[ifield];
        Field& refinedField      = FxyzRefined[ifield];

        uint32 iStart = refinedLayout.ghostStartIndex(refinedField, Direction::X);
        uint32 iEnd   = refinedLayout.ghostEndIndex(refinedField, Direction::X);

        uint32 iphysStart = refinedLayout.physicalStartIndex(refinedField, Direction::X);

        // we might interpolate the parent field
        // from a primal or a dual mesh
        auto centering = coarseLayout.fieldCentering(coarseField, Direction::X);

        // (3) ==> Number of cells on the refined grid when the
        //         field is stored on the dual mesh
        double dual_offset = 0.;
        if (centering == QtyCentering::dual)
        {
            switch (coarseLayout.order())
            {
                // TODO: check if it works at 3rd order
                case 3: dual_offset = -0.5 * refinement; break;

                default: dual_offset = 0.5 * refinement; break;
            }
        }

        // loop on new field indexes
        for (uint32 ix = iStart; ix <= iEnd; ++ix)
        {
            // ix coordinate can be seen as the Particle position of
            // the method fieldAtParticle1D(...)
            //
            // Compute the reduced coordinate on the parent GridLayout

            // (4) ==> Number of physical cells on the refined grid
            double delta = (static_cast<int32>(ix) - static_cast<int32>(iphysStart)) * refinement;

            // coord is a reduced coordinate on the parent primal GridLayout
            // it represents the number of cells relatively to the primal mesh
            // ==> We compute the sum (1)+(2)+(3)+(4)
            double coord = newOriginReducedOnCoarse + dual_offset + delta;

            // nodes from the parent layout now contribute
            // to the node: ix, located on the new layout
            refinedField(ix) = interp(coord, coarseField, centering);
        }
    }
}



void fieldAtRefinedNodes2D(Interpolator& interp, GridLayout const& coarseLayout,
                           VecField const& Fcoarse, GridLayout const& refinedLayout,
                           VecField& Frefined)
{
    (void)interp;
    (void)coarseLayout;
    (void)Fcoarse;
    (void)refinedLayout;
    (void)Frefined;
    throw std::runtime_error("NOT IMPLEMENTED");
}



void fieldAtRefinedNodes3D(Interpolator& interp, GridLayout const& coarseLayout,
                           VecField const& Fcoarse, GridLayout const& refinedLayout,
                           VecField& Frefined)
{
    (void)interp;
    (void)coarseLayout;
    (void)Fcoarse;
    (void)refinedLayout;
    (void)Frefined;
    throw std::runtime_error("NOT IMPLEMENTED");
}


void fieldAtRefinedNodes(Interpolator& interpolator, GridLayout const& coarseLayout,
                         Electromag const& parentElectromag, GridLayout const& refinedLayout,
                         ElectromagInitializer& eminit)
{
    switch (coarseLayout.nbDimensions())
    {
        case 1:
            fieldAtRefinedNodes1D(interpolator, coarseLayout, parentElectromag.getE(),
                                  refinedLayout, eminit.E_);
            fieldAtRefinedNodes1D(interpolator, coarseLayout, parentElectromag.getB(),
                                  refinedLayout, eminit.B_);
            break;
        case 2:
            fieldAtRefinedNodes2D(interpolator, coarseLayout, parentElectromag.getE(),
                                  refinedLayout, eminit.E_);
            fieldAtRefinedNodes2D(interpolator, coarseLayout, parentElectromag.getB(),
                                  refinedLayout, eminit.B_);
            break;
        case 3:
            fieldAtRefinedNodes3D(interpolator, coarseLayout, parentElectromag.getE(),
                                  refinedLayout, eminit.E_);
            fieldAtRefinedNodes3D(interpolator, coarseLayout, parentElectromag.getB(),
                                  refinedLayout, eminit.B_);
            break;
        default: throw std::runtime_error("wrong dimensionality");
    }
}
