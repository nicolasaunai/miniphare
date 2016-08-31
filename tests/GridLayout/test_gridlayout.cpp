

#include <iostream>
//#include  "gridlayout.h"




/*

  GridLayout SPECIFICATION
  ------------------------

  1 - General properties

    1.1 - does not assume a particular layout
    1.2 - behaves the same independently of the origin of coordinates


  2 - Construction:

    2.2 - GridLayout should be movable
    2.3 - GridLayout should be copyiable
        -


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


#include "gtest/gtest.h"

#include <cmath>

 double square_root (const double val)
{
    return std::sqrt(val);
}

void truc()
{
    throw std::runtime_error("ta race");
}


TEST(SquareRootTest, PositiveNos) {




    EXPECT_EQ (18.0, square_root (324.0));
    EXPECT_EQ (25.4, square_root (645.16));
    EXPECT_EQ (50.332, square_root (2533.310224));
}



TEST (SquareRootTest, ZeroAndNegativeNos) {
    ASSERT_EQ (0.0, square_root (0.0));
    ASSERT_EQ (-1, square_root (-22.0));
}



TEST(exceptTest, except1)
{
    EXPECT_ANY_THROW(truc());
    ASSERT_THROW(truc(), std::runtime_error);
}





int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}




















