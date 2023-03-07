
#include "Material.h"
#include "Hittable/Hittable.h"

namespace Monte_carlo_path_tracing
{
    //----------------------------------------- Light_material ------------------------------------------
    bool Light_material::scatter(const Ray& _ray_in, const Hit_record& _rec, Color& _attenuation, Ray& _scattered_ray) const
    {
        _attenuation = m_radiance;
        return false;
    }
    //---------------------------------------------------------------------------------------------------

    //-------------------------------------- Dielectric_material ----------------------------------------
    bool Dielectric_material::scatter(const Ray& _ray_in, const Hit_record& _rec, Color& _attenuation, Ray& _scattered_ray) const
    {
        vec3 scattered_ray_direction;
        double refraction_ratio = _rec.front_face ? m_Ni : (1.0 / m_Ni);
        double cos_theta = fmin(dot(-_ray_in.direction(), _rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        if (cannot_refract || schlick_coefficient(cos_theta, refraction_ratio) > random_double())
        {
            // 镜面反射
            scattered_ray_direction = reflect(_ray_in.direction(), _rec.normal);
            _scattered_ray = Ray(_rec.position, scattered_ray_direction, Ray_type::Specular);
            _attenuation = Color(1.0, 1.0, 1.0);
        }
        else
        {
            // 折射
            scattered_ray_direction = refract(_ray_in.direction(), _rec.normal, refraction_ratio);
            _scattered_ray = Ray(_rec.position, scattered_ray_direction, Ray_type::Transmission);
            _attenuation = Color(1.0, 1.0, 1.0);
        }
        return true;
    }
    //---------------------------------------------------------------------------------------------------

    //---------------------------------------- Glossy_material ------------------------------------------
    bool Glossy_material::scatter(const Ray& _ray_in, const Hit_record& _rec, Color& _attenuation, Ray& _scattered_ray) const
    {
        vec3 scattered_ray_direction;
        Color Kd = _rec.material_ptr->get_Kd(_rec.texture_coord);
        Color Ks = _rec.material_ptr->get_Ks(_rec.texture_coord);
        if (Kd.length() / (Kd.length() + Ks.length()) > random_double())
        {
            // 漫反射
            scattered_ray_direction = importance_sample(_rec.normal, Sample_type::Diffuse);
            _scattered_ray = Ray(_rec.position, scattered_ray_direction, Ray_type::Diffuse);
            _attenuation = Kd * dot(scattered_ray_direction, _rec.normal);
        }
        else
        {
            // 镜面反射
            scattered_ray_direction = importance_sample(reflect(_ray_in.direction(), _rec.normal), Sample_type::Specular, _rec.material_ptr->get_Ns());
            _scattered_ray = Ray(_rec.position, scattered_ray_direction, Ray_type::Specular);
            _attenuation = Ks;
        }
        return true;
    }

    vec3 Glossy_material::importance_sample(const vec3& _direction, Sample_type _sample_type, double _Ns) const
    {
        double phi = random_double() * 2.0 * pi;
        double theta = 0.0;
        switch (_sample_type)
        {
        case Sample_type::Specular:
            theta = std::acos(std::pow(random_double(), 1 / (m_Ns + 1)));
            break;
        case Sample_type::Diffuse:
            theta = std::asin(std::sqrt(random_double()));
            break;
        default:
            break;
        }
        vec3 sample(std::sin(theta) * std::cos(phi), std::cos(theta), std::sin(theta) * std::sin(phi));
        vec3 front = std::fabs(_direction.x) > std::fabs(_direction.y) ? unit_vector(vec3(_direction.z, 0, -_direction.x)) : unit_vector(vec3(0, -_direction.z, -_direction.y));
        vec3 right = cross(_direction, front);
        return unit_vector(sample.x * right + sample.y * _direction + sample.z * front);
    }
    //---------------------------------------------------------------------------------------------------
}