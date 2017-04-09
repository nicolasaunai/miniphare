#ifndef EMDIAGNOSTIC_H
#define EMDIAGNOSTIC_H

#include "Diagnostics/fielddiagnostic.h"


class EMDiagnostic : public FieldDiagnostic
{
private:

public:
    EMDiagnostic(uint32 id) : FieldDiagnostic{id, "EMDiag"}{}

};





#endif // EMDIAGNOSTIC_H
