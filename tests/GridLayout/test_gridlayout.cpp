





/*

  GridLayout SPECIFICATION
  ------------------------

  1 - General properties

    1.1 - does not assume a particular layout
    1.2 - behaves the same independently of the origin of coordinates


  2 - Construction:

    2.2 - GridLayout should be movable
    2.3 - GridLayout should be copyiable
    2.4 - Implementation Factory throws an exception if unknown layout and
          nbDims < 1 and > 3



  3 - nx(), ny(), nz() methods:

    3.1 - takes no argument
    3.2 - returns the number of elements allocated for any Field array on the layout
    3.3 - returns 1 in invariant directions (e.g. ny() and nz() both return 1 for 1D runs)
    3.4 - never return a value < 0
    3.5 - returns a value holding in unsigned 32 bits integer
    3.6 - is callable for const and non const objects
    3.7 - is inlined
    3.8 - return is, in the concerned direction, > (ghost/physical)(Start/End)Index()


  4 - dx(), dy(), dz() methods:

    4.1 - takes no argument
    4.2 - returns the size of a computational cell in the required direction
    4.3 - returns double precision number
    4.4 - returns 0 in invariant directions
    4.5 - returns is always >= 0
    4.6 - is callable from const and non const objects
    4.7 - assumes any start frfor grid coordinates not starting at 0
    4.8 - is inlined


  5 - odx(), ody(), odz() methods:

    5.5  - returns 1/dx, 1/dy, 1/dz
    5.6  - throws an exception if used in an invariant direction
    5.7  - return double precision number
    5.8  - is callable from const and non const objects
    5.9  - is inlined
    5.10 - returns >0 values.


  6 - physicalStartIndex()

    6.1 takes 2 arguments, a const Field and a uint32
    6.2 throws an exception if Field is unknown, i.e. not in :
        - Ex, Ey, Ez, Bx, By, Bz, rho (species), v (species), Jx, Jy, Jz (species)
          Pxx, Pyy, Pzz, Pxy, Pyz, Pzz (species)
    6.3 throws an exception if the direction is not among:
        - GridLayout::directionX, GridLayout::directionY, GridLayout::directionZ
    6.4 should be callable from const and non const objects
    6.5 return >0 value
    6.6 - returns a value holding in unsigned 32 bits integer
    6.7 return value is < physicalEndIndex(), > ghostStartIndex(), < ghostEndIndex()



  7 - physicalEndIndex()

    7.1 .. 7.6 are identical to 6.1..6.6
    7.7 return value is > physicalStartIndex(), is > ghostStartIndex(), is < ghostEndIndex()



  8 - ghostStartIndex()

    8.1..8.6 are identical to 6.1..6.6
    8.7 - return value is > physicalStartIndex(), is < ghostEndIndex(), is < physicalEndIndex()


  9 - ghostEndIndex()

    9.1..9.6 are identical to 6.1..6.6
    9.7 - return value is > physicalStartIndex(), is > ghostStartIndex(), is > physicalEndIndex()




 10 - deriv()

    10.1 - takes three arguments, a const Field&, a direction, and a destination Field&
    10.2 - calculate the derivative of the const Field along the direction and stores it in the dest field.
    10.3 - throws an exception if the field is unknown (cf. 6.2)
    10.4 - same as 6.3
    10.5 - Throws an exception if source and destination fields have incompatible shapes
    10.6 - is callable from const and non const objects


 11 - nbDimensions()

    11.1 - throw an exception if return value is not 1, 2 or 3
    11.2 - return value : if 1, ody() and odz() should throw, if 2 odz() should throw
    11.3 - return a 32 bits unsigned integer
    11.4 - can be called from const and non const objects



*/

#include <string>
#include <iostream>
#include <array>
#include <string>

#include "gtest/gtest.h"


#include "grid/gridlayout.h"
#include "grid/gridlayoutimplfactory.h"
#include "grid/gridlayoutimplyee.h"
#include "types.h"



/* ----------------------------------------------------------------------------
 *
 *                            GridLayoutFactory TEST
 *
 * SPECIFICATION:
 * -------------
 *
 *  - Implementation Factory throws an exception if unknown layout and
 *          nbDims < 1 and > 3
 *
 * ---------------------------------------------------------------------------- */


struct GridLayoutImplFactoryParams
{
    uint32 nbDims;
    std::string implTypeName;

    GridLayoutImplFactoryParams(uint32 dims, std::string const& name):nbDims{dims},implTypeName{name}{}
    friend std::ostream& operator<<(std::ostream& os, GridLayoutImplFactoryParams const& params)
    {
        os << "nbDims = " << params.nbDims << " ; implTypeName = " << params.implTypeName;
        return os;
    }
};



class GridLayoutImplFactoryTest : public ::testing::TestWithParam<GridLayoutImplFactoryParams>
{
};



TEST_P(GridLayoutImplFactoryTest, factoryParamTests)
{
    GridLayoutImplFactoryParams inputs = GetParam();
    std::cout << inputs  << std::endl;
    std::string toto = inputs.implTypeName;
    ASSERT_ANY_THROW(GridLayoutImplFactory::createGridLayoutImpl(inputs.nbDims, inputs.implTypeName));
}


GridLayoutImplFactoryParams factoryInputs[] = {
    GridLayoutImplFactoryParams(-2,"yee"),
//    GridLayoutImplFactoryParams(2269666877636610,"yee"), // TODO Why does this trigger an execption?
    //GridLayoutImplFactoryParams(-5000000000,"yee"),
    GridLayoutImplFactoryParams(4200000000,"yee"),
    GridLayoutImplFactoryParams(4200000000,"wrong"),
    GridLayoutImplFactoryParams(1,"yee "),
    GridLayoutImplFactoryParams(1,"YEE"),
    GridLayoutImplFactoryParams(1,"Yee"),
    GridLayoutImplFactoryParams(1," yee")
};


INSTANTIATE_TEST_CASE_P(BulkTest, GridLayoutImplFactoryTest, testing::ValuesIn(factoryInputs));

 /* ---------------------------------------------------------------------------- */






/* ----------------------------------------------------------------------------
 *
 *                            GridLayout TEST
 *
 * SPECIFICATION:
 * -------------
 *
 *  - Constructor throws if :
 *      - nbDims = 1 and (dy != 0 || dz !=0 || ny != 1 || nz != 1)
 *      - nbDims = 2 and (dz !=0  || nz != 1)
 *
 *  - Copy construction is defined and copies all members OK
 *  - Move construction is defined and moves all members OK
 *
 * ---------------------------------------------------------------------------- */

#if 0
class GridLayoutFixture : public testing::Test
{
public:
    GridLayout gl;
};


struct GridLayoutParams
{
    std::array<double,3> dxdydz;
    std::array<uint32,3> fieldSizes;
    std::string layoutName;
    uint32 nbDims;
};


class GridLayoutConstructorTest: public ::testing::TestWithParam<GridLayoutParams>
{

};



TEST_P(GridLayoutConstructorTest, ConstructorTest)
{
    GridLayoutParams inputs = GetParam();
    ASSERT_ANY_THROW( GridLayout(inputs.dxdydz, inputs.fieldSizes, inputs.nbDims, inputs.layoutName) );
}


GridLayoutImplFactoryParams factoryInputs[] = {
    GridLayoutImplFactoryParams(-2,"yee"),
    GridLayoutImplFactoryParams(-8589934590,"yee"), // TODO Why does this trigger an execption?
    GridLayoutImplFactoryParams(-5000000000,"yee"),
    GridLayoutImplFactoryParams(4200000000,"yee"),
    GridLayoutImplFactoryParams(4200000000,"wrong"),
    GridLayoutImplFactoryParams(1,"yee "),
    GridLayoutImplFactoryParams(1,"YEE"),
    GridLayoutImplFactoryParams(1,"Yee"),
    GridLayoutImplFactoryParams(1," yee")
};

INSTANTIATE_TEST_CASE_P(BulkTest, GridLayoutImplFactoryTest, testing::ValuesIn(factoryInputs));

#endif
/* ---------------------------------------------------------------------------- */








int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}




















