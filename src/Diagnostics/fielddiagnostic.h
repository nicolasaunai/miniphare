#ifndef FIELDDIAGNOSTIC_H
#define FIELDDIAGNOSTIC_H

#include <vector>
#include <array>

#include "Field/field.h"
#include "grid/gridlayout.h"
#include "vecfield/vecfield.h"
#include "Diagnostics/diagnostics.h"


class FieldDiagnostic : public Diagnostic
{

protected:

    void fillDiagData1D_(Field const& field, GridLayout const& layout, DiagPack& pack);
    void fillDiagData2D_(Field const& field, GridLayout const& layout, DiagPack& pack);
    void fillDiagData3D_(Field const& field, GridLayout const& layout, DiagPack& pack);

    void addVecField_(VecField const& vecField, GridLayout const& layout);

    FieldDiagnostic(std::string diagName)
        : Diagnostic{diagName} {}

};




#endif // FIELDDIAGNOSTIC_H
