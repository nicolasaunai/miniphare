
#include "test_commons.h"

#include "test_splitting1part.h"



std::vector<Split1PartParams> getSplit1ParticleParamsFromFile() ;


class Split1ParticleTest: public ::testing::TestWithParam<Split1PartParams>
{
public:
    Split1PartParams inputs;


    ~Split1ParticleTest() = default ;

    void SetUp()
    {
        inputs = GetParam();


    }

};




TEST_P(Split1ParticleTest, shapes)
{


}



INSTANTIATE_TEST_CASE_P(Split1ParticleTest, Split1ParticleTest,
                        testing::ValuesIn( getSplit1ParticleParamsFromFile() ) );

