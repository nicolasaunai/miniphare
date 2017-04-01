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
private:
    std::array<HybridQuantity, 3> Eqty_;
    std::array<HybridQuantity, 3> Bqty_;

public:

    // somehow we need to initialize data_
    //
    ElectromagDiagnostic()
        : FieldDiagnostic{"EM"},
          Eqty_{ {HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez} },
          Bqty_{ {HybridQuantity::Bx, HybridQuantity::By, HybridQuantity::Bz} }
    { }


    virtual void compute(Hierarchy const& hierarchy) final;


    virtual ~ElectromagDiagnostic() = default;
};



#endif // ELECTROMAGDIAGNOSTIC_H
