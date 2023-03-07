
#ifndef GQY_BOUNDING_BOX_H
#define GQY_BOUNDING_BOX_H

#include "Math/Math.h"
#include "Ray/Ray.h"

namespace Monte_carlo_path_tracing
{
    class Bounding_box
    {
    public:
        Bounding_box() = default;
        Bounding_box(const Point& _min, const Point& _max) : minimum(_min), maximum(_max) { }
        ~Bounding_box() = default;

        inline bool hit(const Ray& _ray, double _time_min, double _time_max) const
        {
            for (int i = 0; i < 3; ++i)
            {
                double inv_direction = 1.0 / _ray.direction()[i];
                double time_0 = (minimum[i] - _ray.origin()[i]) * inv_direction;
                double time_1 = (maximum[i] - _ray.origin()[i]) * inv_direction;
                if (inv_direction < 0.0)
                {
                    std::swap(time_0, time_1);
                }
                _time_min = time_0 > _time_min ? time_0 : _time_min;
                _time_max = time_1 < _time_max ? time_1 : _time_max;
                if (_time_max <= _time_min)
                {
                    return false;
                }
            }

            return true;
        }

    public:
        Point minimum;
        Point maximum;
    };

    Bounding_box surrounding_box(Bounding_box _box_1, Bounding_box _box_2);
}

#endif // !GQY_BOUNDING_BOX_H
