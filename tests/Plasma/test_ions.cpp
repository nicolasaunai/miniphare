

#include <type_traits>

#include "gtest/gtest.h"

#include "Plasmas/ions.h"
#include "grid/gridlayout.h"


#if 0
TEST(IonTest, ionIsConstructible)
{
    ASSERT_FALSE( std::is_copy_constructible<Ions>::value );
}


TEST(IonTest, ionIsAssignCopy)
{
    ASSERT_FALSE( std::is_copy_assignable<Ions>::value );
}




#endif

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
