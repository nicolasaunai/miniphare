#ifndef ELECTROMAGDIAGNOSTIC_H
#define ELECTROMAGDIAGNOSTIC_H

#include <vector>

#include "hybridenums.h"
#include "AMR/hierarchy.h"
#include "fielddiagnostic.h"
#include "Electromag/electromag.h"



// an example of diagnostic for the electromagnetic field
// the class implement compute() and data()
// compute() will put EM data as a DiagData
// data() return the reference to this DiagData
class ElectromagDiagnostic : public FieldDiagnostic
{
public:

    // somehow we need to initialize data_
    //
    ElectromagDiagnostic()
        : FieldDiagnostic{"EM"}
    { }


    virtual void compute(Hierarchy const& hierarchy) final;


    virtual ~ElectromagDiagnostic() = default;
};



#endif // ELECTROMAGDIAGNOSTIC_H
