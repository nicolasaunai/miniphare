#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "utilities/types.h"

#include <fstream>
#include <vector>


void printTable(std::vector<double> const& table, const std::string& name);

std::string GetHybridQtyName(uint32 iqty);


#endif // TEST_UTILS_H
