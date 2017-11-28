#ifndef PARTICLESELECTORFACTORY_H
#define PARTICLESELECTORFACTORY_H

#include <memory>
#include <string>

#include "utilities/particleselector.h"


class ParticleSelectorFactory
{
public:
    static std::unique_ptr<ParticleSelector>
    createParticleSelector(std::string selectorType, std::vector<double> const& selectorParams)
    {
        std::unique_ptr<ParticleSelector> selector   = nullptr;

        if (selectorType == "spaceBox")
        {
            Box selectionBox{selectorParams[0], selectorParams[1]};

            selector = std::unique_ptr<ParticleSelector>{new IsInBoxSelector{selectionBox}};
        }

        if (selectorType == "spaceAndEnergyBox")
        {
            Box selectionBox{selectorParams[0], selectorParams[1]};

            // TODO: create space and energy selector
            selector = std::unique_ptr<ParticleSelector>{new IsInBoxSelector{selectionBox}};
        }

        return selector;
    }
};


#endif // PARTICLESELECTORFACTORY_H
