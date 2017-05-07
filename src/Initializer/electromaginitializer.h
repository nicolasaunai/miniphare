#ifndef ELECTROMAGINITIALIZER_H
#define ELECTROMAGINITIALIZER_H


#include <array>

#include "grid/gridlayout.h"
#include "types.h"
#include "vecfield/vecfield.h"

class ElectromagInitializer
{
private:
    // using VectorFunction = void   (*) (double x, double y, double z, std::array<double,3> vec);
    VectorFunction electricField;
    VectorFunction magneticField;

    void init1D_(GridLayout const& layout);
    void init2D_(GridLayout const& layout);
    void init3D_(GridLayout const& layout);

    void init1D_Ecomponent_(GridLayout const& layout, Field& Ei, uint32 iComponent);
    void init1D_Bcomponent_(GridLayout const& layout, Field& Bi, uint32 iComponent);

    void init2D_Ecomponent_(GridLayout const& layout, Field& Ei, uint32 iComponent);
    void init2D_Bcomponent_(GridLayout const& layout, Field& Bi, uint32 iComponent);

    void init3D_Ecomponent_(GridLayout const& layout, Field& Ei, uint32 iComponent);
    void init3D_Bcomponent_(GridLayout const& layout, Field& Bi, uint32 iComponent);

public:
    VecField E_;
    VecField B_;
    std::string nameE, nameB;

    ElectromagInitializer(GridLayout const& layout, VectorFunction electricField,
                          VectorFunction magneticField, std::string electricName,
                          std::string magneticName);

    ElectromagInitializer(GridLayout const& layout, std::string electricName,
                          std::string magneticName);
};

#endif // ELECTROMAGINITIALIZER_H
