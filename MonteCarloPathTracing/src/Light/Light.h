
#ifndef GQY_LIGHT_H
#define GQY_LIGHT_H

#include <memory>

#include "Math/Math.h"
#include "Model/Mesh.h"
#include "Material/Material.h"

namespace Monte_carlo_path_tracing
{
    class Light : public Hittable
    {
    public:
        Light() : area(0.0), m_mesh(nullptr) { }
        Light(const std::shared_ptr<Mesh>& _mesh, const Color& _radiance) : m_mesh(_mesh), light_material(std::make_shared <Light_material>(_radiance))
        {
            vec3 E1 = m_mesh->point_B() - m_mesh->point_A();
            vec3 E2 = m_mesh->point_C() - m_mesh->point_A();
            area = cross(E1, E2).length() / 2.0;
        }
        ~Light() = default;

        inline const Point& center() const
        {
            return m_center;
        }

        inline const vec3& normal() const
        {
            return m_normal;
        }

        inline void calculate(double _alpha, double _beta, double _gama)
        {
            double sum = _alpha + _beta + _gama;
            m_center = (_alpha * m_mesh->point_A() + _beta * m_mesh->point_B() + _gama * m_mesh->point_C()) / sum;
            m_normal = unit_vector((_alpha * m_mesh->normal_A() + _beta * m_mesh->normal_B() + _gama * m_mesh->normal_C()) / sum);
        }

        virtual bool hit(const Ray& _ray, const double& _t_min, const double& _t_max, Hit_record& _record) const override;
        virtual bool create_bounding_box(Bounding_box& _output_box) const override;

    public:
        std::shared_ptr<Light_material> light_material;
        double area;

    private:
        std::shared_ptr<Mesh> m_mesh;
        Point m_center;
        vec3 m_normal;
    };
}
#endif // !GQY_LIGHT_H
