#ifndef FIELDPACK_H
#define FIELDPACK_H

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

#include "data/grid/gridlayoutdefs.h"
#include "utilities/types.h"


/**
 * @brief The FieldPack struct constains data and metadata for a field to be saved
 */
struct FieldPack
{
    std::vector<std::string> keys;
    std::unordered_map<std::string, std::vector<float> > data;
    std::unordered_map<std::string, std::array<uint32, 3> > nbrNodes;
    std::unordered_map<std::string, std::array<float, 3> > gridSpacing;
    std::unordered_map<std::string, std::array<QtyCentering,3> > centerings;
    std::unordered_map<std::string, uint32> nbrDimensions;
    std::unordered_map<std::string, Point> origin;
};




#endif // FIELDPACK_H
