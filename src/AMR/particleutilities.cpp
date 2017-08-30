#include "particleutilities.h"

#include <algorithm>


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



void recomputeParticlePosition(GridLayout const& praLayout, GridLayout const& patchLayout,
                               Particle const& part, Particle& newPart)
{
    newPart = part;

    int32 nbrGhostspra = static_cast<int32>(praLayout.nbrGhostNodes(QtyCentering::primal));

    // Components of the vector oriented
    // from the origin of the refined layout
    // to mother particle
    double compo_x
        = (praLayout.origin().x_ - patchLayout.origin().x_)
          + (static_cast<int32>(part.icell[0]) - nbrGhostspra + part.delta[0]) * praLayout.dx();

    double compo_y
        = (praLayout.origin().y_ - patchLayout.origin().y_)
          + (static_cast<int32>(part.icell[1]) - nbrGhostspra + part.delta[1]) * praLayout.dy();

    double compo_z
        = (praLayout.origin().z_ - patchLayout.origin().z_)
          + (static_cast<int32>(part.icell[2]) - nbrGhostspra + part.delta[2]) * praLayout.dz();

    int32 nbrGhostsPatch = static_cast<int32>(patchLayout.nbrGhostNodes(QtyCentering::primal));

    double compo_x_odx = compo_x / patchLayout.dx();
    double compo_y_ody = compo_y / patchLayout.dy();
    double compo_z_odz = compo_z / patchLayout.dz();

    switch (praLayout.nbDimensions())
    {
        case 3:
            newPart.icell[2]
                = static_cast<uint32>(nbrGhostsPatch + static_cast<int32>(std::floor(compo_z_odz)));

            newPart.delta[2] = static_cast<float>(compo_z_odz - std::floor(compo_z_odz));

        case 2:
            newPart.icell[1]
                = static_cast<uint32>(nbrGhostsPatch + static_cast<int32>(std::floor(compo_y_ody)));

            newPart.delta[1] = static_cast<float>(compo_y_ody - std::floor(compo_y_ody));

        case 1:
            newPart.icell[0]
                = static_cast<uint32>(nbrGhostsPatch + static_cast<int32>(std::floor(compo_x_odx)));

            newPart.delta[0] = static_cast<float>(compo_x_odx - std::floor(compo_x_odx));
            break;

        default: throw std::runtime_error("normalizeMotherPosition: wrong dimensionality");
    }
}
