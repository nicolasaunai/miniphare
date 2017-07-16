#ifndef PATCHDATA_H
#define PATCHDATA_H




#include "Electromag/electromag.h"
#include "Initializer/initializerfactory.h"
#include "Plasmas/electrons.h"
#include "Plasmas/ions.h"
#include "Solver/solver.h"
#include "grid/gridlayout.h"
#include "types.h"




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

    // PatchData(double dt, const GridLayout& layout);

    PatchData(PatchData const& source) = delete;
    PatchData& operator=(PatchData const& source) = delete;

    PatchData(PatchData&& source) = default;
    PatchData& operator=(PatchData&& source) = default;

    void initPatchPhysicalDomain();


    uint32 population() const;

    Electromag const& EMfields() const { return EMfields_; }

    Solver const& solver() const { return solver_; }

    Ions& ions() { return ions_; }
    Ions const& ions() const { return ions_; }

    BoundaryCondition* boundaryCondition() { return boundaryCondition_.get(); }

    void solveStep();
};

#endif // PATCHDATA_H
