
#include "BoundingBox.h"

namespace Monte_carlo_path_tracing
{
    Bounding_box surrounding_box(Bounding_box _box_1, Bounding_box _box_2)
    {
        Point small((std::min)(_box_1.minimum.x, _box_2.minimum.x), (std::min)(_box_1.minimum.y, _box_2.minimum.y), (std::min)(_box_1.minimum.z, _box_2.minimum.z));
        Point big((std::max)(_box_1.maximum.x, _box_2.maximum.x), (std::max)(_box_1.maximum.y, _box_2.maximum.y), (std::max)(_box_1.maximum.z, _box_2.maximum.z));
        return Bounding_box(small, big);
    }
}