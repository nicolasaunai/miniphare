

#include <type_traits>

#include "gtest/gtest.h"
#include "Plasmas/ions.h"



TEST(IonTest, ionConstructorTest)
{
    ASSERT_FALSE( std::is_copy_constructible<Ions>::value );
}


TEST(IonTest, ionAssignCopy)
{
    ASSERT_FALSE( std::is_copy_assignable<Ions>::value );
}


TEST(IonTest, ionMoveCtorIsNoexcept)
{
    ASSERT_TRUE( noexcept(Ions(std::declval<Ions>())) );
}






int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}








