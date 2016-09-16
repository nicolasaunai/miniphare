#ifndef PARTICLEINITIALIZERIMPL_H
#define PARTICLEINITIALIZERIMPL_H


#include <memory>
#include <vector>

#include "Plasmas/particles.h"


class ParticleInitializerImpl
{
public:
    ParticleInitializerImpl();

    std::unique_ptr<ParticleInitializerImpl> clone() const;


    virtual void loadParticles(std::vector<Particle>& particles) const = 0;


    using ScalarFunction = double (*) (double x, double y, double z);
    using VectorFunction = void   (*) (double x, double y, double z, std::array<double,3> vec);


private:

    virtual ParticleInitializerImpl* cloneImpl() const = 0;


};

#endif // PARTICLEINITIALIZERIMPL_H
