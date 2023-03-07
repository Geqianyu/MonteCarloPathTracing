
#ifndef GQY_RAY_H
#define GQY_RAY_H

#include "Math/Math.h"

namespace Monte_carlo_path_tracing
{

    //-------------------------------------------- Ray_type ---------------------------------------------
    enum class Ray_type
    {
        None = 0,
        Diffuse,
        Specular,
        Transmission
    };
    //---------------------------------------------------------------------------------------------------

    //---------------------------------------------- Ray ------------------------------------------------
    class Ray
    {
    public:
        Ray(Ray_type _ray_type = Ray_type::None) : m_ray_type(_ray_type) { }
        Ray(const Point& _origin, const vec3& _direction, Ray_type _ray_type = Ray_type::None) : m_origin(_origin), m_direction(unit_vector(_direction)), m_ray_type(_ray_type) { }
        ~Ray() = default;

        inline Point origin() const
        {
            return m_origin;
        }

        inline vec3 direction() const
        {
            return m_direction;
        }

        inline Ray_type type() const
        {
            return m_ray_type;
        }

        inline Point at(double _time)
        {
            return m_origin + _time * m_direction;
        }

    private:
        Point m_origin;
        vec3 m_direction;
        Ray_type m_ray_type;
    };
    //---------------------------------------------------------------------------------------------------
}

#endif // !GQY_RAY_H
