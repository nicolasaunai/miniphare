
#include "utilities/box.h"
#include "types.h"

bool pointInBox(Point const& point, Box const& box)
{
    return (point.x >= box.x0 && point.x <= box.x1 && point.y >= box.y0 && point.y <= box.y1
            && point.z >= box.z0 && point.z <= box.z1);
}
