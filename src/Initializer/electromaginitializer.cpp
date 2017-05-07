#include <sstream>

#include "electromaginitializer.h"

// Set dirX,dirY,dirZ for all this file
const uint32 dirX = 0;
const uint32 dirY = 1;
const uint32 dirZ = 2;

ElectromagInitializer::ElectromagInitializer(GridLayout const &layout, VectorFunction electricField,
                                             VectorFunction magneticField, std::string electricName,
                                             std::string magneticName)
    : electricField{electricField}
    , magneticField{magneticField}
    , E_{layout.allocSize(HybridQuantity::Ex),
         layout.allocSize(HybridQuantity::Ey),
         layout.allocSize(HybridQuantity::Ez),
         {{HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez}},
         electricName}
    , B_{layout.allocSize(HybridQuantity::Bx),
         layout.allocSize(HybridQuantity::By),
         layout.allocSize(HybridQuantity::Bz),
         {{HybridQuantity::Bx, HybridQuantity::By, HybridQuantity::Bz}},
         magneticName}

{
    switch (layout.nbDimensions())
    {
        case 1: init1D_(layout); break;
        case 2: init2D_(layout); break;
        case 3: init3D_(layout); break;
        default:
            std::stringstream errorMessage;
            errorMessage << "Error,wrong dimensionality at " << __FILE__ << "(" << __LINE__ << ")";
            std::runtime_error(errorMessage.str().c_str());
    }
}




ElectromagInitializer::ElectromagInitializer(GridLayout const &layout, std::string electricName,
                                             std::string magneticName)
    : electricField{nullptr}
    , magneticField{nullptr}
    , E_{layout.allocSize(HybridQuantity::Ex),
         layout.allocSize(HybridQuantity::Ey),
         layout.allocSize(HybridQuantity::Ez),
         {{HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez}},
         electricName}
    , B_{layout.allocSize(HybridQuantity::Bx),
         layout.allocSize(HybridQuantity::By),
         layout.allocSize(HybridQuantity::Bz),
         {{HybridQuantity::Bx, HybridQuantity::By, HybridQuantity::Bz}},
         magneticName}

{
}




void ElectromagInitializer::init1D_(GridLayout const &layout)
{
    for (uint32 iComponent = 0; iComponent < NBR_COMPO; ++iComponent)
    {
        Field &Ei = E_.component(iComponent);
        init1D_Ecomponent_(layout, Ei, iComponent);
    }
    for (uint32 iComponent = 0; iComponent < NBR_COMPO; ++iComponent)
    {
        Field &Bi = B_.component(iComponent);
        init1D_Bcomponent_(layout, Bi, iComponent);
    }
}




void ElectromagInitializer::init2D_(GridLayout const &layout)
{
    for (uint32 iComponent = 0; iComponent < NBR_COMPO; ++iComponent)
    {
        Field &Ei = E_.component(iComponent);
        init2D_Ecomponent_(layout, Ei, iComponent);
    }

    for (uint32 iComponent = 0; iComponent < NBR_COMPO; ++iComponent)
    {
        Field &Bi = B_.component(iComponent);
        init2D_Bcomponent_(layout, Bi, iComponent);
    }
}




void ElectromagInitializer::init3D_(GridLayout const &layout)
{
    for (uint32 iComponent = 0; iComponent < NBR_COMPO; ++iComponent)
    {
        Field &Ei = E_.component(iComponent);
        init3D_Ecomponent_(layout, Ei, iComponent);
    }

    for (uint32 iComponent = 0; iComponent < NBR_COMPO; ++iComponent)
    {
        Field &Bi = B_.component(iComponent);
        init3D_Bcomponent_(layout, Bi, iComponent);
    }
}




void ElectromagInitializer::init1D_Ecomponent_(GridLayout const &layout, Field &Ei,
                                               uint32 iComponent)
{
    Point origin = layout.origin();

    // ELECTRIC FIELD ----------------
    uint32 iStart{layout.ghostStartIndex(Ei, Direction::X)};
    uint32 iEnd{layout.ghostEndIndex(Ei, Direction::X)};

    for (uint32 ix = iStart; ix <= iEnd; ++ix)
    {
        Point coord = layout.fieldNodeCoordinates(Ei, origin, ix, 0, 0);
        std::array<double, NBR_COMPO> E = electricField(coord.x_, origin.y_, origin.z_);
        Ei(ix) = E[iComponent];
    }
}




void ElectromagInitializer::init1D_Bcomponent_(GridLayout const &layout, Field &Bi,
                                               uint32 iComponent)
{
    Point origin = layout.origin();

    // MAGNETIC FIELD ----------------
    uint32 iStart{layout.ghostStartIndex(Bi, Direction::X)};
    uint32 iEnd{layout.ghostEndIndex(Bi, Direction::X)};

    for (uint32 ix = iStart; ix <= iEnd; ++ix)
    {
        Point coord = layout.fieldNodeCoordinates(Bi, origin, ix, 0, 0);
        std::array<double, NBR_COMPO> B = magneticField(coord.x_, origin.y_, origin.z_);
        Bi(ix) = B[iComponent];
    }
}




void ElectromagInitializer::init2D_Ecomponent_(GridLayout const &layout, Field &Ei,
                                               uint32 iComponent)
{
    Point origin = layout.origin();

    // ELECTRIC FIELD ----------------
    std::array<uint32, 2> iStart{
        {layout.ghostStartIndex(Ei, Direction::X), layout.ghostStartIndex(Ei, Direction::Y)}};
    std::array<uint32, 2> iEnd{
        {layout.ghostEndIndex(Ei, Direction::X), layout.ghostEndIndex(Ei, Direction::Y)}};

    for (uint32 ix = iStart[dirX]; ix <= iEnd[dirX]; ++ix)
    {
        for (uint32 iy = iStart[dirY]; iy <= iEnd[dirY]; ++iy)
        {
            Point coord = layout.fieldNodeCoordinates(Ei, origin, ix, iy, 0);
            std::array<double, NBR_COMPO> E = electricField(coord.x_, coord.y_, origin.z_);
            Ei(ix, iy)                      = E[iComponent];
        }
    }
}




void ElectromagInitializer::init2D_Bcomponent_(GridLayout const &layout, Field &Bi,
                                               uint32 iComponent)
{
    Point origin = layout.origin();

    // MAGNETIC FIELD ----------------
    std::array<uint32, 2> iStart{
        {layout.ghostStartIndex(Bi, Direction::X), layout.ghostStartIndex(Bi, Direction::Y)}};
    std::array<uint32, 2> iEnd{
        {layout.ghostEndIndex(Bi, Direction::X), layout.ghostEndIndex(Bi, Direction::Y)}};
    for (uint32 ix = iStart[dirX]; ix <= iEnd[dirX]; ++ix)
    {
        for (uint32 iy = iStart[dirY]; iy <= iEnd[dirY]; ++iy)
        {
            Point coord = layout.fieldNodeCoordinates(Bi, origin, ix, iy, 0);
            std::array<double, NBR_COMPO> B = magneticField(coord.x_, coord.y_, origin.z_);
            Bi(ix, iy)                      = B[iComponent];
        }
    }
}




void ElectromagInitializer::init3D_Ecomponent_(GridLayout const &layout, Field &Ei,
                                               uint32 iComponent)
{
    Point origin = layout.origin();

    // ELECTRIC FIELD ----------------
    std::array<uint32, 3> iStart{{layout.ghostStartIndex(Ei, Direction::X),
                                  layout.ghostStartIndex(Ei, Direction::Y),
                                  layout.ghostStartIndex(Ei, Direction::Z)}};
    std::array<uint32, 3> iEnd{{layout.ghostEndIndex(Ei, Direction::X),
                                layout.ghostEndIndex(Ei, Direction::Y),
                                layout.ghostEndIndex(Ei, Direction::Z)}};

    for (uint32 ix = iStart[dirX]; ix <= iEnd[dirX]; ++ix)
    {
        for (uint32 iy = iStart[dirY]; iy <= iEnd[dirY]; ++iy)
        {
            for (uint32 iz = iStart[dirZ]; iz <= iEnd[dirZ]; ++iz)
            {
                Point coord = layout.fieldNodeCoordinates(Ei, origin, ix, iy, iz);
                std::array<double, NBR_COMPO> E = electricField(coord.x_, coord.y_, coord.z_);
                Ei(ix, iy, iz) = E[iComponent];
            }
        }
    }
}




void ElectromagInitializer::init3D_Bcomponent_(GridLayout const &layout, Field &Bi,
                                               uint32 iComponent)
{
    Point origin = layout.origin();

    // MAGNETIC FIELD ----------------
    std::array<uint32, 3> iStart{{layout.ghostStartIndex(Bi, Direction::X),
                                  layout.ghostStartIndex(Bi, Direction::Y),
                                  layout.ghostStartIndex(Bi, Direction::Z)}};
    std::array<uint32, 3> iEnd{{layout.ghostEndIndex(Bi, Direction::X),
                                layout.ghostEndIndex(Bi, Direction::Y),
                                layout.ghostEndIndex(Bi, Direction::Z)}};

    for (uint32 ix = iStart[dirX]; ix <= iEnd[dirX]; ++ix)
    {
        for (uint32 iy = iStart[dirY]; iy <= iEnd[dirY]; ++iy)
        {
            for (uint32 iz = iStart[dirZ]; iz <= iEnd[dirZ]; ++iz)
            {
                Point coord = layout.fieldNodeCoordinates(Bi, origin, ix, iy, iz);
                std::array<double, NBR_COMPO> B = magneticField(coord.x_, coord.y_, coord.z_);
                Bi(ix, iy, iz) = B[iComponent];
            }
        }
    }
}
