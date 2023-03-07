
#ifndef GQY_MATERIAL_H
#define GQY_MATERIAL_H

#include <memory>

#include "Math/Math.h"
#include "Ray/Ray.h"
#include "Texture.h"

namespace Monte_carlo_path_tracing
{
    // 声明存在一个 Hit_record 的 struct 存在
    struct Hit_record;

    //---------------------------------------- Sample_type ----------------------------------------------
    enum class Sample_type
    {
        Specular = 0,
        Diffuse
    };
    //---------------------------------------------------------------------------------------------------

    //------------------------------------------- Material ----------------------------------------------
    class Material
    {
    public:
        Material() = default;
        ~Material() = default;

        //----------------------------------------------- set -----------------------------------------------
        inline void set_Ks(const Color& _Ks)
        {
            m_Ks = std::make_shared<Solid_color>(_Ks);
        }

        inline void set_Ks(const std::string& _file_path)
        {
            m_Ks = std::make_shared<Image_texture>(_file_path);
        }

        inline void set_Kd(const Color& _Kd)
        {
            m_Kd = std::make_shared<Solid_color>(_Kd);
        }

        inline void set_Kd(const std::string& _file_path)
        {
            m_Kd = std::make_shared<Image_texture>(_file_path);
        }

        inline void set_Ns(const double& _Ns)
        {
            m_Ns = _Ns;
        }

        inline void set_Ni(const double& _Ni)
        {
            m_Ni = _Ni;
        }
        //---------------------------------------------------------------------------------------------------

        //----------------------------------------------- get -----------------------------------------------
        inline Color get_Ks(const Texture_coordinate& _texture_coord) const
        {
            return m_Ks->value(_texture_coord);
        }

        inline Color get_Kd(const Texture_coordinate& _texture_coord) const
        {
            return m_Kd->value(_texture_coord);
        }

        inline double get_Ns() const
        {
            return m_Ns;
        }

        inline double get_Ni() const
        {
            return m_Ni;
        }
        //---------------------------------------------------------------------------------------------------

        virtual bool scatter(const Ray& _ray_in, const Hit_record& _rec, Color& _attenuation, Ray& _scattered_ray) const = 0;

    protected:
        std::shared_ptr<Texture> m_Kd;
        std::shared_ptr<Texture> m_Ks;
        double m_Ns{ 1.0 };
        double m_Ni{ 1.0 };
    };
    //---------------------------------------------------------------------------------------------------

    //----------------------------------------- Light_material ------------------------------------------
    class Light_material : public Material
    {
    public:
        Light_material() = default;
        Light_material(const Color& _radiance) : m_radiance(_radiance) { }
        ~Light_material() = default;

        inline void set_radiance(const Color& _radiance)
        {
            m_radiance = _radiance;
        }

        inline const Color& get_radiance() const
        {
            return m_radiance;
        }

        virtual bool scatter(const Ray& _ray_in, const Hit_record& _rec, Color& _attenuation, Ray& _scattered_ray) const override;

    private:
        Color m_radiance;
    };
    //---------------------------------------------------------------------------------------------------

    //-------------------------------------- Dielectric_material ----------------------------------------
    class Dielectric_material : public Material
    {
    public:
        Dielectric_material() = default;
        ~Dielectric_material() = default;

        virtual bool scatter(const Ray& _ray_in, const Hit_record& _rec, Color& _attenuation, Ray& _scattered_ray) const override;

    private:
        // Schlick 近似 菲涅尔 系数
        inline double schlick_coefficient(double _cosine, double _refraction_ratio) const
        {
            double r0 = (1 - _refraction_ratio) / (1 + _refraction_ratio);
            r0 *= r0;
            return r0 + (1 - r0) * std::pow((1 - _cosine), 5);
        }
    };
    //---------------------------------------------------------------------------------------------------

    //---------------------------------------- Glossy_material ------------------------------------------
    class Glossy_material : public Material
    {
    public:
        Glossy_material() = default;
        ~Glossy_material() = default;

        virtual bool scatter(const Ray& _ray_in, const Hit_record& _rec, Color& _attenuation, Ray& _scattered_ray) const override;

    private:
        vec3 importance_sample(const vec3& _direction, Sample_type _sample_type, double _Ns = 0.0) const;
    };
    //---------------------------------------------------------------------------------------------------
}

#endif // !GQY_MATERIAL_H
