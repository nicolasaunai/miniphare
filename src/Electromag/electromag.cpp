
#include "electromag.h"
#include "vecfield/vecfield.h"




Electromag::Electromag(uint32 nx, uint32 ny, uint32 nz, std::string name)
    :E_(nx, ny, nz, "E"+name), B_(nx, ny, nz, "B"+name)
{

}
