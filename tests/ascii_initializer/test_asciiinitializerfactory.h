#ifndef TEST_ASCIIINITIALIZERFACTORY_H
#define TEST_ASCIIINITIALIZERFACTORY_H

#include <array>
#include <memory>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "utilities/types.h"

class test_asciiinitializerfactory
{
public:
    test_asciiinitializerfactory();
};

::testing::AssertionResult AreVectorsEqual(std::vector<uint32> const& expected_vector,
                                           std::vector<uint32> const& actual_vector)
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
        if (expected_vector[ik] != actual_vector[ik])
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


#endif // TEST_ASCIIINITIALIZERFACTORY_H
