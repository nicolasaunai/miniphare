
#include "utilities/types.h"

#include "test_commons.h"


bool isShapeEqual(Field const& expected, Field const& actual);

::testing::AssertionResult AreVecfieldsEqual(VecField const& expected_vecfield,
                                             VecField const& actual_vecfield, double precision)
{
    ::testing::AssertionResult failure = ::testing::AssertionFailure();

    uint32 errorNbr = 0;



    for (uint32 iCompo = 0; iCompo < 3; ++iCompo)
    {
        Field const& expected = expected_vecfield.component(iCompo);
        Field const& actual   = actual_vecfield.component(iCompo);

        bool shapeEqual = isShapeEqual(expected, actual);

        if (!shapeEqual)
        {
            ::testing::AssertionResult sizeFailure = ::testing::AssertionFailure();
            sizeFailure << "expected and actual Fields have different shapes";
            return sizeFailure;
        }

        auto it_exp = expected.begin();
        auto it_act = actual.begin();
        for (it_exp = expected.begin(); it_exp != expected.end(); it_exp++, it_act++)
        {
            if (fabs(*it_exp - *it_act) > precision)
            {
                failure << "\n";
                failure << "expected = " << *it_exp << "    ";
                failure << "actual   = " << *it_act << "\n";
                ++errorNbr;
            }
        }
    }


    if (errorNbr > 0)
    {
        // Error Summary
        failure << "\nTotal number of differences = " << errorNbr << "\n";

        return failure;
    }

    return testing::AssertionSuccess();
}



bool isShapeEqual(Field const& expected, Field const& actual)
{
    bool shapeEqual = true;

    std::vector<uint32> expected_shape = expected.shape();
    std::vector<uint32> actual_shape   = actual.shape();

    if (actual_shape.empty())
        shapeEqual = false;

    if (expected_shape.size() != actual_shape.size())
        shapeEqual = false;

    if (shapeEqual)
    {
        for (uint32 icompo = 0; icompo < expected_shape.size(); ++icompo)
        {
            if (expected_shape[icompo] != actual_shape[icompo])
                shapeEqual = false;
        }
    }

    return shapeEqual;
}
