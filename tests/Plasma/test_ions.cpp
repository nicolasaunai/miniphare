

#include <type_traits>

#include "gtest/gtest.h"

#include "grid/gridlayout.h"
#include "Plasmas/ions.h"



TEST(IonTest, ionIsConstructible)
{
    ASSERT_FALSE( std::is_copy_constructible<Ions>::value );
}


TEST(IonTest, ionIsAssignCopy)
{
    ASSERT_FALSE( std::is_copy_assignable<Ions>::value );
}


#if 0
TEST(IonTest, ionMoveCtorIsNoexcept)
{
   // ASSERT_TRUE( noexcept(Ions(std::declval<Ions>())) );
}
#endif


/*
TEST(IonTest, ionConstructorNoThrow)
{
    GridLayout gl{ {0.1,0.1, 0.}, {100,100,0}, 2, "yee" };
    ASSERT_NO_THROW(Ions myIon{gl} );
}*/



TEST(IonTest, ionConstructorMoveLayout)
{
    GridLayout gl{ {0.1,0.1, 0.}, {100,100,0}, 2, "yee" };
    ASSERT_NO_THROW(Ions myIon{std::move(gl)} );
}



int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}








