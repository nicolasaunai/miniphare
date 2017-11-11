#ifndef INITMODEL_H
#define INITMODEL_H


#include <memory>
#include <vector>

#include <data/Plasmas/particleinitializer.h>
#include <utilities/types.h>


/**
 * @brief Interface for an InitModel.
 */
class InitModel
{
public:
    virtual uint32 nbParticlesPerCell(uint32 speciesIndex) const                           = 0;
    virtual std::vector<std::unique_ptr<ParticleInitializer>> particleInitializers() const = 0;
    virtual std::unique_ptr<VectorFunction> electricFunction() const                       = 0;
    virtual std::unique_ptr<VectorFunction> magneticFunction() const                       = 0;

    virtual std::vector<uint32> nbrsParticlesPerCell() const = 0;

    virtual ~InitModel() = 0;
};

#endif // INITMODEL_H
