#include "particleutilities.h"
#include "types.h"
#include <algorithm>



Point getParticlePosition(Particle const& part, GridLayout const& layout)
{
    double dx = layout.dx();
    double dy = layout.dy();
    double dz = layout.dz();

    Box box = layout.getBox();

    double x0 = box.x0;
    double y0 = box.y0;
    double z0 = box.z0;

    int32 nbrGhosts = static_cast<int32>(layout.nbrGhostNodes(QtyCentering::primal));

    double posx = (part.icell[0] - nbrGhosts + static_cast<double>(part.delta[0])) * dx + x0;
    double posy = (part.icell[1] - nbrGhosts + static_cast<double>(part.delta[1])) * dy + y0;
    double posz = (part.icell[2] - nbrGhosts + static_cast<double>(part.delta[2])) * dz + z0;

    return Point{posx, posy, posz};
}


Point getParticlePosition(Particle const& part, Point const& origin, int32 nbrGhosts,
                  std::array<double, 3> gridSpacing)
{
    double dx = gridSpacing[0];
    double dy = gridSpacing[1];
    double dz = gridSpacing[2];

    double x0 = origin.x;
    double y0 = origin.y;
    double z0 = origin.z;

    double posx = (part.icell[0] - nbrGhosts + static_cast<double>(part.delta[0])) * dx + x0;
    double posy = (part.icell[1] - nbrGhosts + static_cast<double>(part.delta[1])) * dy + y0;
    double posz = (part.icell[2] - nbrGhosts + static_cast<double>(part.delta[2])) * dz + z0;

    return Point{posx, posy, posz};
}



/**
 * @brief removeParticles
 * All particles indexed in leavingIndexes are removed from
 * particleArray
 */
void removeParticles(std::vector<uint32> leavingIndexes, std::vector<Particle>& particleArray)
{
    auto size = particleArray.size();

    /* The idea of the algorithm is to loop over the leaving indexes
    * and swap each leaving particle with the last one of the particle array
    * if the last particle is also one to be moved then update the index list
    * with its new location in the particle array*/

    for (std::size_t iLeav = 0; iLeav < leavingIndexes.size(); ++iLeav)
    {
        // swap the leaving particle with the last one of the array
        // remove the last element of the array
        // decrement its size by one.
        particleArray[leavingIndexes[iLeav]] = particleArray[size - 1];
        particleArray.pop_back();
        size--;

        // the particle at position 'size' has been moved to position
        // leavingIndexes[iLeav]
        // if its former index (size) was in leavingIndexes
        // then leavingIndexes has to be updated to the new location
        // for the particle to be removed.
        auto it = std::find(leavingIndexes.begin(), leavingIndexes.end(), size);
        if (it != leavingIndexes.end())
        {
            *it = leavingIndexes[iLeav];

            // we have to maintain the index array sorted
            // but only have to sort the part over which we have not looped yet
            std::sort(leavingIndexes.begin() + static_cast<long int>(iLeav) + 1,
                      leavingIndexes.end());
        }
    }
}



void particleChangeLayout(GridLayout const& gcaLayout, GridLayout const& patchLayout,
                          Particle const& part, Particle& newPart)
{
    newPart = part;

    auto nbrGhostsgca   = static_cast<int32>(gcaLayout.nbrGhostNodes(QtyCentering::primal));
    auto nbrGhostsPatch = static_cast<int32>(patchLayout.nbrGhostNodes(QtyCentering::primal));

    std::array<double, 3> deltaOrigins;
    deltaOrigins[0] = gcaLayout.origin().x - patchLayout.origin().x;
    deltaOrigins[1] = gcaLayout.origin().y - patchLayout.origin().y;
    deltaOrigins[2] = gcaLayout.origin().z - patchLayout.origin().z;


    std::array<double, 3> oldNormalizedPos;
    for (uint32 i           = 0; i <= 2; ++i)
        oldNormalizedPos[i] = static_cast<double>(part.icell[i] + part.delta[i]);

    // Components of the vector oriented from the origin of the refined layout
    // to mother particle
    std::array<double, 3> newNormalizedPos;
    auto gcadxyz = gcaLayout.dxdydz();

    for (uint32 i           = 0; i <= 2; ++i)
        newNormalizedPos[i] = deltaOrigins[i] + (oldNormalizedPos[i] - nbrGhostsgca) * gcadxyz[i];


    auto newPosition = newNormalizedPos;
    auto patchdxyz   = patchLayout.dxdydz();

    for (uint32 i = 0; i <= 2; ++i)
        newPosition[i] /= patchdxyz[i];


    auto delta
        = [](double position) { return static_cast<float>(position - std::floor(position)); };

    switch (gcaLayout.nbDimensions())
    {
        case 3:
            newPart.icell[2] = nbrGhostsPatch + static_cast<int32>(std::floor(newPosition[2]));
            newPart.delta[2] = delta(newPosition[2]);

        case 2:
            newPart.icell[1] = nbrGhostsPatch + static_cast<int32>(std::floor(newPosition[1]));
            newPart.delta[1] = delta(newPosition[1]);

        case 1:
            newPart.icell[0] = nbrGhostsPatch + static_cast<int32>(std::floor(newPosition[0]));
            newPart.delta[0] = delta(newPosition[0]);
            break;

        default: throw std::runtime_error("normalizeMotherPosition: wrong dimensionality");
    }
}
