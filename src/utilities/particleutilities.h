#ifndef PARTICLETESTS_H
#define PARTICLETESTS_H

#include "types.h"

#include "data/Plasmas/particles.h"
#include "data/grid/gridlayout.h"


Point getParticlePosition(Particle const& part, GridLayout const& layout);

Point getParticlePosition(Particle const& part, Point const& origin, int32 nbrGhosts,
                          std::array<double, 3> gridSpacing);


void particleChangeLayout(GridLayout const& praLayout, GridLayout const& patchLayout,
                          Particle const& part, Particle& newPart);

void removeParticles(std::vector<uint32> leavingIndexes, std::vector<Particle>& particleArray);

#endif // PARTICLETESTS_H
