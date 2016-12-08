#ifndef TEST_COMMONS_H
#define TEST_COMMONS_H


#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "types.h"

#include <fstream>
#include <vector>


void printTable( std::vector<double> const & table, const std::string & name ) ;



::testing::AssertionResult AreVectorsEqual(
        const std::vector<double> & expected_vector,
        const std::vector<double> & actual_vector  ,
        double precision  ) ;



#endif // TEST_COMMONS_H
