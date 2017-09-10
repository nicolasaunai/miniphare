#ifndef PARTICLETESTS_H
#define PARTICLETESTS_H

#include "types.h"

#include "Plasmas/particles.h"
#include "grid/gridlayout.h"


Point getPosition(Particle const& part, GridLayout const& layout);


void particleChangeLayout(GridLayout const& praLayout, GridLayout const& patchLayout,
                               Particle const& part, Particle& newPart);

void removeParticles(std::vector<uint32> leavingIndexes, std::vector<Particle>& particleArray);

#endif // PARTICLETESTS_H
