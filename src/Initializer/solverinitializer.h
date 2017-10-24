#ifndef SOLVERINITIALIZER_H
#define SOLVERINITIALIZER_H

#include <vector>

#include "core/BoundaryConditions/boundary_conditions.h"
#include "utilities/types.h"
#include <string>

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
    std::string pusherType;
    std::vector<uint32> interpolationOrders;
};



#endif // SOLVERINITIALIZER_H
