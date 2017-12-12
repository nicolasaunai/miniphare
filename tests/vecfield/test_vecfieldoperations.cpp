#include <array>
#include <memory>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"


#include "data/vecfield/vecfieldoperations.h"
#include "test_vecfieldoperations.h"

#include "test_commons.h"


void fillComponent_(VecField& vf, UniformVectorFunction& function);

void print(basicVecFieldParams const& inputs);
void print(getVariationParams const& inputs);
void print(timeInterpolationParams const& inputs);

void buildVecFields(basicVecFieldParams const& inputs, VecField& vft1, VecField& vft2,
                    VecField& expected);



basicVecFieldParams timeAverageExamples[] = {

    basicVecFieldParams(AllocSizeT{100, 1, 1}, HybridQuantity::V, {{"vft1", "vft2", "vf_avg"}},
                        {{1., 10., 100.}}, {{2., 40., 50.}}, {{1.5, 25., 75.}}),
    basicVecFieldParams(AllocSizeT{100, 1, 1}, HybridQuantity::V, {{"vft1", "vft2", "vf_avg"}},
                        {{-10., -100., -1.e9}}, {{10., 100., 1.e9}}, {{0., 0., 0.}})};


// --------------------------------------------------------
//
//           TIME AVERAGE TEST
//
// --------------------------------------------------------
void test_timeAverage::SetUp()
{
    inputs = GetParam();
    print(inputs);

    VecField vft1, vft2;

    buildVecFields(inputs, vft1, vft2, expected_average);

    UniformVectorFunction unifFunc_t1(inputs.uniformValues_t1);
    UniformVectorFunction unifFunc_t2(inputs.uniformValues_t2);
    UniformVectorFunction unifFunc_avg(inputs.unif_expected);

    // Now, we fill VecField objects using uniform vector Functions
    fillComponent_(vft1, unifFunc_t1);
    fillComponent_(vft2, unifFunc_t2);
    fillComponent_(expected_average, unifFunc_avg);

    // Build actual time averaged vecfield
    actual_average = VecField(inputs.xComponent, inputs.xComponent, inputs.xComponent,
                              {{inputs.componentType, inputs.componentType, inputs.componentType}},
                              "actual_vf");

    // Now, we use timeAverage(...) method to compute the actual VecField
    timeAverage(vft1, vft2, actual_average);
}





// --------------------------------------------------------
//               INSTANTIATE
//               TEST CASES
// --------------------------------------------------------
TEST_P(test_timeAverage, timeAverage)
{
    EXPECT_TRUE(AreVecfieldsEqual(expected_average, actual_average, precision));
}



INSTANTIATE_TEST_CASE_P(timeAverageTest, test_timeAverage, testing::ValuesIn(timeAverageExamples));



int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}




void buildVecFields(basicVecFieldParams const& inputs, VecField& vft1, VecField& vft2,
                    VecField& expected)
{
    // build input VecFields at t1 and t2
    vft1 = VecField(inputs.xComponent, inputs.xComponent, inputs.xComponent,
                    {{inputs.componentType, inputs.componentType, inputs.componentType}},
                    inputs.vf_names[0]);

    vft2 = VecField(inputs.xComponent, inputs.xComponent, inputs.xComponent,
                    {{inputs.componentType, inputs.componentType, inputs.componentType}},
                    inputs.vf_names[1]);

    // build expected VecField
    expected = VecField(inputs.xComponent, inputs.xComponent, inputs.xComponent,
                        {{inputs.componentType, inputs.componentType, inputs.componentType}},
                        inputs.vf_names[2]);
}


void fillComponent_(VecField& vf, UniformVectorFunction& function)
{
    // parameters are useless for uniform function
    std::array<double, 3> compo_value = function(0., 0., 0.);

    for (uint32 icompo = 0; icompo < 3; ++icompo)
    {
        Field& compo = vf.component(icompo);

        for (uint32 ix = 0; ix < compo.shape()[0]; ++ix)
            for (uint32 iy = 0; iy < compo.shape()[1]; ++iy)
                for (uint32 iz = 0; iz < compo.shape()[2]; ++iz)
                    compo(ix, iy, iz) = compo_value[icompo];
    }
}



void print(basicVecFieldParams const& inputs)
{
    std::cout << "Dimension in each direction : " << inputs.xComponent.nx_ << ", "
              << inputs.xComponent.ny_ << ", " << inputs.xComponent.nz_ << "\n"
              << "Component type : " << static_cast<int32>(inputs.componentType) << " \n"
              << " vecfield name at t1 = " << inputs.vf_names[0] << "\n"
              << " vecfield name at t2 = " << inputs.vf_names[1] << "\n"
              << " Uniform value at t1 = " << inputs.uniformValues_t1[0] << ", "
              << inputs.uniformValues_t1[1] << ", " << inputs.uniformValues_t1[2] << "\n"
              << " Uniform value at t2 = " << inputs.uniformValues_t2[0] << ", "
              << inputs.uniformValues_t2[1] << ", " << inputs.uniformValues_t2[2] << "\n"
              << " Expected uniform value = " << inputs.unif_expected[0] << ", "
              << inputs.unif_expected[1] << ", " << inputs.unif_expected[2] << "\n"
              << std::endl;
}


