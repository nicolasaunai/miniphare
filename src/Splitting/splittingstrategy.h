#ifndef SPLITTINGSTRATEGY_H
#define SPLITTINGSTRATEGY_H

#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "utilityphare.h"

#include "Plasmas/particles.h"
#include "grid/gridlayout.h"



class SplittingStrategy
{
public:
    SplittingStrategy(std::string name, uint32 nbpts);

    virtual ~SplittingStrategy() = default;

    std::string const& name() const { return splitMethod_; }

    void split1D(const Particle& mother, std::vector<Particle>& childParticles) const;

    static void normalizeMotherPosition1D(const GridLayout& coarseLayout,
                                          const GridLayout& refinedLayout,
                                          const uint32 refinementRatio, const Particle& mother,
                                          Particle& normalizedMother);

protected:
    std::string splitMethod_;

    uint32 nbpts_;

    std::vector<int32> child_icellx_;
    std::vector<float> child_deltax_;

    std::vector<double> child_weights_;

    double wtot_;
};

#endif // SPLITTINGSTRATEGY_H
