#ifndef TEST_COMMONS_H
#define TEST_COMMONS_H

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "data/vecfield/vecfield.h"

::testing::AssertionResult AreVecfieldsEqual(VecField const& expected_vecfield,
                                             VecField const& actual_vecfield, double precision);


#endif // TEST_COMMONS_H
