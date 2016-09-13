#ifndef PARTICLELOADER_H
#define PARTICLELOADER_H


#include <vector>

#include "particles.h"
#include "grid/gridlayout.h"


class ParticleLoader
{

public:
    ParticleLoader();

    void loadParticles(std::vector<Particle>& particleArray,
                       GridLayout const& layout);

private:

    struct CellCenterPosition
    {
        double x,y,z;
    };

    std::vector<CellCenterPosition> cellPos_;

};

#endif // PARTICLELOADER_H
