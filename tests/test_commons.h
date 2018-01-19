#ifndef TEST_COMMONS_H
#define TEST_COMMONS_H

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "utilities/types.h"

#include <fstream>
#include <vector>


::testing::AssertionResult AreVectorsEqual(std::vector<uint32> const& expected_vector,
                                           std::vector<uint32> const& actual_vector);

::testing::AssertionResult AreVectorsEqual(const std::vector<double>& expected_vector,
                                           const std::vector<double>& actual_vector,
                                           double precision);

#endif // TEST_COMMONS_H
