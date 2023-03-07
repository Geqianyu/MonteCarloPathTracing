
#ifndef GQY_HITTABLE_H
#define GQY_HITTABLE_H

#include <memory>
#include "Ray/Ray.h"
#include "BVH/BoundingBox.h"
#include "Material/Material.h"

namespace Monte_carlo_path_tracing
{
    //------------------------------------------- Hit_record --------------------------------------------
    struct Hit_record
    {
        inline void set_face_normal(const Ray& _ray, const vec3& _outward_normal)
        {
            front_face = dot(_ray.direction(), _outward_normal) > 0;
            normal = front_face ? -_outward_normal : _outward_normal;
        }

        Point position;
        vec3 normal;
        std::shared_ptr<Material> material_ptr;
        double time = 0.0;
        Texture_coordinate texture_coord;
        bool front_face = false;
    };
    //---------------------------------------------------------------------------------------------------

    //-------------------------------------------- Hittable ---------------------------------------------
    class Hittable
    {
    public:
        Hittable() = default;
        ~Hittable() = default;

        virtual bool hit(const Ray& _ray, const double& _t_min, const double& _t_max, Hit_record& _record) const = 0;
        virtual bool create_bounding_box(Bounding_box& _output_box) const = 0;
    };
    //---------------------------------------------------------------------------------------------------
}

#endif // !GQY_HITTABLE_H
