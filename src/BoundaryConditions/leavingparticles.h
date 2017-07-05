#ifndef LEAVINGPARTICLES_H
#define LEAVINGPARTICLES_H

#include <array>
#include <vector>

#include "grid/gridlayout.h"
#include "types.h"


struct StartEndIndices
{
    uint32 firstCellIndex;
    uint32 lastCellIndex;
};



/**
 * @brief The LeavingParticles class is used to detect and store leaving particles
 *
 * This class is intended to be used in the Pusher. Right at the end of the move
 * phase, the pusher must call storeIfLeaving. This function will detect if the
 * particle is leaving the patch and if yes store its index. The particle index
 * is stored in a vector associated to the boundary through which the particle is
 * leaving. Boundary conditions will later loop on these vectors and apply a specific
 * boundary condition.
 */
class LeavingParticles
{
private:
    static const uint32 nbDirections = 3;

public:
    //! store start and end cells index in each direction
    std::array<StartEndIndices, nbDirections> startEndIndices;

    //! for each direction store the index of particles leaving at min edge
    std::vector<std::vector<uint32>> particleIndicesAtMin;

    //! for each direction store the index of particles leaving at the max edge
    std::vector<std::vector<uint32>> particleIndicesAtMax;

    LeavingParticles(GridLayout const& layout)
        : particleIndicesAtMin(layout.nbDimensions())
        , particleIndicesAtMax(layout.nbDimensions())
    {
        startEndIndices[0].firstCellIndex
            = layout.physicalStartIndex(QtyCentering::primal, Direction::X);
        startEndIndices[0].lastCellIndex
            = layout.physicalEndIndex(QtyCentering::primal, Direction::X) - 1;

        // if 2D or 3D, X already done, do Y
        if (layout.nbDimensions() > 1)
        {
            startEndIndices[1].firstCellIndex
                = layout.physicalStartIndex(QtyCentering::primal, Direction::Y);
            startEndIndices[1].lastCellIndex
                = layout.physicalEndIndex(QtyCentering::primal, Direction::Y) - 1;
        }

        // if 3D, X and Y already done, do Z
        if (layout.nbDimensions() == 3)
        {
            startEndIndices[2].firstCellIndex
                = layout.physicalStartIndex(QtyCentering::primal, Direction::Z);
            startEndIndices[2].lastCellIndex
                = layout.physicalEndIndex(QtyCentering::primal, Direction::Z) - 1;
        }

        // hard coded pre-allocation
        const uint32 estimateLeavingParticles = 1000;
        for (uint32 iDim = 0; iDim < layout.nbDimensions(); ++iDim)
        {
            particleIndicesAtMin[iDim].reserve(estimateLeavingParticles);
            particleIndicesAtMax[iDim].reserve(estimateLeavingParticles);
        }
    }


    //! this should be used at the begining of the move phase
    void cleanBuffers()
    {
        // loop on all directions
        for (auto&& buffer : particleIndicesAtMin)
        {
            buffer.clear();
        }

        for (auto&& buffer : particleIndicesAtMax)
        {
            buffer.clear();
        }
    }


    inline void storeIfLeaving(uint32 icell, uint32 ipart, uint32 direction)
    {
        if (icell > startEndIndices[direction].lastCellIndex)
        {
            particleIndicesAtMax[direction].push_back(ipart);
        }
        else if (icell < startEndIndices[direction].firstCellIndex)
        {
            particleIndicesAtMin[direction].push_back(ipart);
        }
    }
};


#endif // LEAVINGPARTICLES_H
