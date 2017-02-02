#ifndef SOLVERINITIALIZER_H
#define SOLVERINITIALIZER_H

#include <vector>

#include "types.h"
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
    uint32 nbrSpecies ;
    std::string  pusherType ;

    std::vector<uint32>  interpolationOrders ;

    std::vector< std::pair<Edge, std::string> > fieldBCType ;

    std::vector< std::string > particleBCType ;
};



#endif // SOLVERINITIALIZER_H
