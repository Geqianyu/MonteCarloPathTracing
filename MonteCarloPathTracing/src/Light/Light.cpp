
#include "Light.h"

namespace Monte_carlo_path_tracing
{
    bool Light::hit(const Ray& _ray, const double& _t_min, const double& _t_max, Hit_record& _record) const
    {
        return true;
    }

    bool Light::create_bounding_box(Bounding_box& _output_box) const
    {
        return true;
    }
}
