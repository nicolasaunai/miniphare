#ifndef PARTICLESELECTOR_H
#define PARTICLESELECTOR_H

#include "utilityphare.h"

#include "Plasmas/particles.h"
#include "grid/gridlayoutdefs.h"
#include "particleutilities.h"
#include "types.h"

/**
 * @brief The ParticleSelector class is an interface, it mainly
 * provides an operator on a Particle.
 * This operator tests whether the particle belongs to
 * an arbitrary geometrical domain.
 *
 * The most simple and evident domain being a
 * 1D, 2D or 3D cuboid.
 *
 * In the future, other concrete implementations might be
 * required.
 *
 */
class ParticleSelector
{
public:
    ParticleSelector() {}

    virtual bool pick(Particle const& particle) const = 0;

    virtual ~ParticleSelector() = default;
};




/**
 * @brief The isInBox class is a concrete implementation of
 * ParticleSelector.
 * The operator tests whether the particle belongs a 1D,
 * 2D or 3D cuboid.
 *
 */
class IsInBoxSelector : public ParticleSelector
{
private:
    GridLayout referenceLayout_;
    Box targetBox_;

public:
    IsInBoxSelector(GridLayout const& layout, Box const& box)
        : referenceLayout_{layout}
        , targetBox_{box}
    {
    }


    inline bool pick(Particle const& particle) const override
    {
        Point particlePosition = getPosition(particle, referenceLayout_);
        return pointInBox(particlePosition, targetBox_);
    }


    virtual ~IsInBoxSelector() {}
};




#endif // PARTICLESELECTOR_H
