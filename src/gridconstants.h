#ifndef GRIDCONSTANTS_H
#define GRIDCONSTANTS_H

enum class Direction{ directionX, directionY, directionZ } ;

enum class LayoutType{ primal, dual } ;

enum class EMFieldType{ EVecField, BVecField } ;

enum class HybridQuantity{ Bx, By, Bz, Ex, Ey, Ez, rho, V, P };

enum class OhmTerm{ Ideal, Pressure, Resistivity, HyperResistivity };



#endif // GRIDCONSTANTS_H
