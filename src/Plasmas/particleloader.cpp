#include "particleloader.h"


ParticleLoader::ParticleLoader()
{

}



void ParticleLoader::loadParticles(std::vector<Particle>& particleArray,
                                   GridLayout const& layout)
{
    std::vector<CellCenterPosition> cellPos_ (10);//= layout.fieldNodeCoordinate(HybridQuantity::ChargeDensity);

    std::vector<double> f(10);

   for (auto cell : cellPos_)
   {

        distribution.draw()
   }
}
