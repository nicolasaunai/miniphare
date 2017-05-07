#include "test_commons.h"




::testing::AssertionResult AreVectorsEqual(const std::vector<double>& expected_vector,
                                           const std::vector<double>& actual_vector,
                                           double precision)
{
    ::testing::AssertionResult failure = ::testing::AssertionFailure();

    uint32 errorNbr = 0;

    if (expected_vector.size() != actual_vector.size())
    {
        ::testing::AssertionResult sizeFailure = ::testing::AssertionFailure();

        sizeFailure << "expected_vector.size() != actual_vector.size()";

        return sizeFailure;
    }

    for (uint32 ik = 0; ik < actual_vector.size(); ++ik)
    {
        if (fabs(expected_vector[ik] - actual_vector[ik]) > precision)
        {
            failure << "\n";
            failure << "expected[" << ik << "] = " << expected_vector[ik];
            failure << "    ";
            failure << "actual  [" << ik << "] = " << actual_vector[ik];
            failure << "\n";
            ++errorNbr;
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
