

#include <type_traits>

#include "gtest/gtest.h"

#include "grid/gridlayout.h"
#include "Plasmas/ions.h"


#if 1
TEST(IonTest, ionIsConstructible)
{
    ASSERT_FALSE( std::is_copy_constructible<Ions>::value );
}


TEST(IonTest, ionIsAssignCopy)
{
    ASSERT_FALSE( std::is_copy_assignable<Ions>::value );
}



TEST(IonTest, ionMoveCtorIsNoexcept)
{
   // ASSERT_TRUE( noexcept(Ions(std::declval<Ions>())) );
}




TEST(IonTest, ionConstructorNoThrow)
{
    GridLayout gl{ {0.1,0.1, 0.}, {100,100,0}, 2, "yee" };
    ASSERT_NO_THROW(Ions myIon{gl} );
}



TEST(IonTest, ionConstructorMoveLayout)
{
    GridLayout gl{ {0.1,0.1, 0.}, {100,100,0}, 2, "yee" };
    ASSERT_NO_THROW(Ions myIon{std::move(gl)} );
}



/*
TEST(IonTest, speciesGetterNonConst)
{
    GridLayout gl{ {0.1,0.1, 0.}, {100,100,0}, 2, "yee" };
    Ions myIon{std::move(gl)};
    ASSERT_ANY_THROW( myIon.species(12) );
}*/



#endif

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}








