#ifndef PARTICLETESTS_H
#define PARTICLETESTS_H

#include "types.h"

#include "Plasmas/particles.h"
#include "Plasmas/virtualparticle.h"
#include "grid/gridlayout.h"


bool isInRefinedBox(GridLayout const& refinedLayout, VirtualParticle const& child);


bool isInSpecifiedBox(GridLayout const& particleLayout, Particle const& child,
                      GridLayout const& targetLayout);


#endif // PARTICLETESTS_H
