
#include <vector>

#include <gtest/gtest.h>

#include "Field/field.h"
#include "types.h"
#include "vecfield/vecfield.h"

#if 0
class Species
{
public:
    Field rho_;
    VecField flux_;

    Species(uint32 nx, uint32 ny, uint32 nz):rho_(nx*ny*nz,12),flux_(nx,ny,nz,12)
};







int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif
