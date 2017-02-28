#ifndef LEAVINGPARTICLES_H
#define LEAVINGPARTICLES_H

#include <vector>
#include <array>

#include "types.h"
#include "grid/gridlayout.h"


struct StartEndIndices
{
    uint32 firstCellIndex;
    uint32 lastCellIndex;
};


class LeavingParticles
{
private:
    static const uint32 nbDirections = 3;

public:
    std::array<StartEndIndices, nbDirections> startEndIndices;
    std::vector< std::vector<uint32> > particleIndicesAtMin;
    std::vector< std::vector<uint32> > particleIndicesAtMax;

    LeavingParticles(GridLayout const& layout)
        : particleIndicesAtMin(layout.nbDimensions() ),
          particleIndicesAtMax(layout.nbDimensions() )
    {
#if 1
        startEndIndices[0].firstCellIndex = layout.physicalStartIndex(QtyCentering::primal,
                                                                       Direction::X);
        startEndIndices[0].lastCellIndex = layout.physicalEndIndex(QtyCentering::primal,
                                                                       Direction::X) - 1;

        // if 2D or 3D, X already done, do Y
        if (layout.nbDimensions() > 1)
        {
            startEndIndices[1].firstCellIndex = layout.physicalStartIndex(QtyCentering::primal,
                                                                           Direction::Y);
            startEndIndices[1].lastCellIndex  = layout.physicalEndIndex(QtyCentering::primal,
                                                                         Direction::Y) - 1;
        }

        // if 3D, X and Y already done, do Z
        if (layout.nbDimensions() == 3)
        {
            startEndIndices[2].firstCellIndex = layout.physicalStartIndex(QtyCentering::primal,
                                                                           Direction::Z);
            startEndIndices[3].lastCellIndex  = layout.physicalEndIndex(QtyCentering::primal,
                                                                         Direction::Z)-1;
        }
#endif
    }


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
        if (icell  > startEndIndices[direction].lastCellIndex)
        {
            particleIndicesAtMax[direction].push_back(ipart);
        }
        else if (icell < startEndIndices[direction].lastCellIndex)
        {
            particleIndicesAtMin[direction].push_back(ipart);
        }
    }

};


#endif // LEAVINGPARTICLES_H
