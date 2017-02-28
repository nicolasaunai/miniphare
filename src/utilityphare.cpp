#include "utilityphare.h"



/* below are just stupid functions to make this initializer work
   these functions are intended to be passed to the fluid initializer
   we have to imagin that in reality these functions would come from other
   modules. For instance, the PythonIntializerFactory would read these functions
   from python scripts..
*/

double densityProton1(double x, double y, double z)
{
    (void) x;
    (void) y;
    (void) z;

    return 1.;
}


double densityProton2(double x, double y, double z)
{
    (void) x;
    (void) y;
    (void) z;
    return 0.25;
}


double thermalSpeedProton1(double x, double y, double z)
{
    (void) x;
    (void) y;
    (void) z;
    return 0.25;
}


double thermalSpeedProton2(double x, double y, double z)
{
    (void) x;
    (void) y;
    (void) z;
    return 0.25;
}


std::array<double,3> bulkVelocityProton1(double x, double y, double z)
{
    std::array<double,3> vec;
    (void) x;
    (void) y;
    (void) z;
    vec[0] = 0.;
    vec[1] = 0.;
    vec[2] = 0.;
    return vec;
}

std::array<double,3> bulkVelocityProton2(double x, double y, double z)
{
    std::array<double,3> vec;
    (void) x;
    (void) y;
    (void) z;
    vec[0] = 0.;
    vec[1] = 0.;
    vec[2] = 0.;
    return vec;
}


std::array<double,3>  magneticField(double x, double y, double z)
{
    std::array<double,3> vec;
    (void) x;
    (void) y;
    (void) z;
    vec[0] = 1.;
    vec[1] = 0.;
    vec[2] = 0.;
    return vec;
}


std::array<double,3> electricField(double x, double y, double z)
{
    std::array<double,3> vec;
    (void) x;
    (void) y;
    (void) z;
    vec[0] = 0.;
    vec[1] = 0.;
    vec[2] = 0.;
    return vec;
}
/* -------------------------- end of hard coded functions --------------------- */



