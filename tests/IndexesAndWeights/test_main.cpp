#include "test_indexes.h"
#include "test_weightsum.h"
#include "test_exactweights.h"



int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


