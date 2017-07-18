#ifndef PARTICLETESTS_H
#define PARTICLETESTS_H

#include "types.h"

#include "Plasmas/particles.h"
#include "Plasmas/virtualparticle.h"
#include "grid/gridlayout.h"


bool isInRefinedBox(GridLayout const& refinedLayout, VirtualParticle const& child);


bool isInSpecifiedBox(GridLayout const& particleLayout, Particle const& child,
                      GridLayout const& targetLayout);


void recomputeParticlePosition(GridLayout const& praLayout, GridLayout const& patchLayout,
                               Particle const& part, Particle& newPart);

void removeParticles(std::vector<uint32> const& leavingIndexes,
                     std::vector<Particle>& particleArray);

#endif // PARTICLETESTS_H
