#ifndef GRIDCONSTANTS_H
#define GRIDCONSTANTS_H

enum class Direction{ directionX, directionY, directionZ } ;

enum class EMFieldType{ EVecField, BVecField } ;

enum class HybridQuantity{ Bx, By, Bz, Ex, Ey, Ez, rho, V, P, Ohm, derived, none };

enum class OhmTerm{ Ideal, Pressure, Resistivity, HyperResistivity };

enum class DerivedEMField{ dxEz, dxEy, none };


#endif // GRIDCONSTANTS_H
