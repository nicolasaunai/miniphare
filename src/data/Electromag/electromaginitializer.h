#ifndef ELECTROMAGINITIALIZER_H
#define ELECTROMAGINITIALIZER_H


#include <array>

#include "data/grid/gridlayout.h"
#include "data/vecfield/vecfield.h"
#include "utilities/types.h"


/**
 * @brief is passed to and Electromag constructor to initializer EM fields
 */
class ElectromagInitializer
{
private:
    std::unique_ptr<VectorFunction> electricField;
    std::unique_ptr<VectorFunction> magneticField;

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

    ElectromagInitializer(GridLayout const& layout, std::unique_ptr<VectorFunction> electricField,
                          std::unique_ptr<VectorFunction> magneticField, std::string electricName,
                          std::string magneticName);

    ElectromagInitializer(GridLayout const& layout, std::string electricName,
                          std::string magneticName);
};

#endif // ELECTROMAGINITIALIZER_H
