#ifndef FIELDPACK_H
#define FIELDPACK_H

#include <array>
#include <vector>
#include <string>
#include <unordered_map>

#include "types.h"
#include "grid/gridlayoutdefs.h"


/**
 * @brief The FieldPack struct constains data and metadata for a field to be saved
 */
struct FieldPack
{
    std::unordered_map<std::string, std::vector<float> > data;
    std::array<uint32, 3> nbrNodes;
    std::array<QtyCentering,3> centerings;
    uint32 nbrDimensions;
    Point origin;
};





#endif // FIELDPACK_H
