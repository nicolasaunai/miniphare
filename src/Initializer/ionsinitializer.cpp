#include "ionsinitializer.h"


IonsInitializer::IonsInitializer(uint32 nbrSpecies)
    : particleInitializers_(nbrSpecies),
      masses_(nbrSpecies), names_(nbrSpecies)

{

}
