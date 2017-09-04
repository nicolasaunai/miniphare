#include "particleutilities.h"
#include "types.h"
#include <algorithm>



Point getPosition(Particle const& part, GridLayout const& layout)
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


bool pointInBox(Point const& point, Box const& box)
{
    return (point.x_ >= box.x0 && point.x_ <= box.x1 && point.y_ >= box.y0 && point.y_ <= box.y1
            && point.z_ >= box.z0 && point.z_ <= box.z1);
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
