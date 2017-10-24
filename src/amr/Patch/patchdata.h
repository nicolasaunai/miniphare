#ifndef PATCHDATA_H
#define PATCHDATA_H

#include "core/Solver/solver.h"


#include "data/Electromag/electromag.h"
#include "initializer/initializerfactory.h"
#include "data/Plasmas/electrons.h"
#include "data/Plasmas/ions.h"
#include "data/grid/gridlayout.h"

#include "utilities/types.h"




/**
 * @brief The PatchData class encapsulate data for a Patch necessary for
 * the Solver to solve the hybrid system of equations.
 * A PatchData is a heavy object since it contains all Ions etc. Therefore
 * it is a move only object.
 */
class PatchData
{
private:
    Electromag EMfields_;
    Solver solver_;
    Ions ions_;
    Electrons electrons_;
    // TODO: Private implementation to avoid a pointer here
    std::unique_ptr<BoundaryCondition> boundaryCondition_;

public:
    PatchData(InitializerFactory const& initFactory);

    PatchData(PatchData const& source) = delete;
    PatchData& operator=(PatchData const& source) = delete;

    PatchData(PatchData&& source) = default;
    PatchData& operator=(PatchData&& source) = default;

    void initPatchPhysicalDomain();

    uint32 population() const;

    Electromag& EMfields() { return EMfields_; }
    Electromag const& EMfields() const { return EMfields_; }

    Ions& ions() { return ions_; }
    Ions const& ions() const { return ions_; }

    BoundaryCondition* boundaryCondition() { return boundaryCondition_.get(); }

    void solveStep();
};

#endif // PATCHDATA_H
