#ifndef SOLVERINITIALIZER_H
#define SOLVERINITIALIZER_H

#include <array>
#include <vector>

#include "types.h"

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
    uint32 nbrSpecies ;
    std::string  pusherType ;

    std::vector<uint32>  interpolationOrders ;

    std::array<std::string, 6> fieldBCType ;
    std::array<std::string, 6> particleBCType ;
};



#endif // SOLVERINITIALIZER_H
