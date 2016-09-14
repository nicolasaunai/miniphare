#ifndef ELECTROMAGINITIALIZER_H
#define ELECTROMAGINITIALIZER_H


#include<array>


class ElectromagInitializer
{
protected:

    using VectorFunction = void   (*) (double x, double y, double z, std::array<double,3> vec);

public:

    ElectromagInitializer();

    VectorFunction electricField;
    VectorFunction magneticField;

};

#endif // ELECTROMAGINITIALIZER_H
