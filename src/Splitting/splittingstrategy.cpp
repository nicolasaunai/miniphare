
#include "Splitting/splittingstrategy.h"




SplittingStrategy::SplittingStrategy(std::string name, uint32 nbpts)
    : splitMethod_{name}
    , nbpts_{nbpts}
    , child_icellx_{std::vector<int32>(nbpts_, 0)}
    , child_deltax_{std::vector<float>(nbpts_, 0)}
    , child_weights_{std::vector<double>(nbpts_, 0)}
    , wtot_{0.}
{
}



void SplittingStrategy::split1D(const Particle& mother, std::vector<Particle>& childParticles) const
{
    for (uint32 ik = 0; ik < nbpts_; ++ik)
    {
        int32 icellx = mother.icell[0] + child_icellx_[ik];
        float deltax = mother.delta[0] + child_deltax_[ik];

        // Auto-correction needed when
        // deltax > 1 or deltax < 0
        float icor = std::floor(deltax);
        deltax -= icor;
        icellx += icor;

        double weight = mother.weight * child_weights_[ik] / wtot_;

        Particle newBorn(weight, mother.charge, {{icellx, mother.icell[1], mother.icell[2]}},
                         {{deltax, mother.delta[1], mother.delta[2]}}, mother.v);

        childParticles.push_back(std::move(newBorn));
    }
}



void SplittingStrategy::normalizeMotherPosition(const GridLayout& coarseLayout,
                                                const GridLayout& refinedLayout,
                                                const Particle& mother, Particle& normalizedMother)
{
    normalizedMother = mother;


    int32 nbrGhostsLn = static_cast<int32>(coarseLayout.nbrGhostNodes(QtyCentering::primal));

    // Components of the vector oriented
    // from the origin of the refined layout
    // to mother particle
    double compo_x = (coarseLayout.origin().x_ - refinedLayout.origin().x_)
                     + (mother.icell[0] - nbrGhostsLn + mother.delta[0]) * coarseLayout.dx();

    double compo_y = (coarseLayout.origin().y_ - refinedLayout.origin().y_)
                     + (mother.icell[1] - nbrGhostsLn + mother.delta[1]) * coarseLayout.dy();

    double compo_z = (coarseLayout.origin().z_ - refinedLayout.origin().z_)
                     + (mother.icell[2] - nbrGhostsLn + mother.delta[2]) * coarseLayout.dz();

    int32 nbrGhostsLnplus1 = static_cast<int32>(refinedLayout.nbrGhostNodes(QtyCentering::primal));

    double compo_x_odx = compo_x / refinedLayout.dx();
    double compo_y_ody = compo_y / refinedLayout.dy();
    double compo_z_odz = compo_z / refinedLayout.dz();

    switch (coarseLayout.nbDimensions())
    {
        case 3:
            normalizedMother.icell[2]
                = nbrGhostsLnplus1 + static_cast<int32>(std::floor(compo_z_odz));

            normalizedMother.delta[2] = static_cast<float>(compo_z_odz - std::floor(compo_z_odz));

        case 2:
            normalizedMother.icell[1]
                = nbrGhostsLnplus1 + static_cast<int32>(std::floor(compo_y_ody));

            normalizedMother.delta[1] = static_cast<float>(compo_y_ody - std::floor(compo_y_ody));

        case 1:
            normalizedMother.icell[0]
                = nbrGhostsLnplus1 + static_cast<int32>(std::floor(compo_x_odx));

            normalizedMother.delta[0] = static_cast<float>(compo_x_odx - std::floor(compo_x_odx));
            break;

        default: throw std::runtime_error("normalizeMotherPosition: wrong dimensionality");
    }
}
