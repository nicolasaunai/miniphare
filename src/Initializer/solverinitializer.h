#ifndef SOLVERINITIALIZER_H
#define SOLVERINITIALIZER_H

#include <vector>

#include "types.h"
#include <string>
#include "BoundaryConditions/boundary_conditions.h"

/**
 * @brief SolverInitializer is needed for Solver construction
 *  so that each Species has its own Projector/Interpolator
 *
 * It contains all necessary information o
 *
 *
 *
 */
struct SolverInitializer
{
    std::string  pusherType ;
    std::vector<uint32>  interpolationOrders ;
    std::unique_ptr<BoundaryCondition> boundaryCondition_;
};



#endif // SOLVERINITIALIZER_H
