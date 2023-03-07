
#ifndef GQY_MESH_H
#define GQY_MESH_H

#include <vector>
#include <memory>
#include <regex>

#include "Math/Math.h"
#include "Material/Material.h"
#include "Hittable/Hittable.h"
#include "BVH/BoundingBox.h"

namespace Monte_carlo_path_tracing
{
    typedef int indices_type[3];

    class Mesh : public Hittable
    {
    public:
        Mesh() = default;
        Mesh(const Mesh& _mesh) : m_vertices_indices{ _mesh.m_vertices_indices[0], _mesh.m_vertices_indices[1], _mesh.m_vertices_indices[2] }, m_normals_indices{ _mesh.m_normals_indices[0], _mesh.m_normals_indices[1], _mesh.m_normals_indices[2] }, m_textures_indices{ _mesh.m_textures_indices[0], _mesh.m_textures_indices[1], _mesh.m_textures_indices[2] }, m_E1(_mesh.m_E1), m_E2(_mesh.m_E2), m_material(_mesh.m_material), m_vs(_mesh.m_vs), m_vns(_mesh.m_vns), m_vts(_mesh.m_vts) { }
        Mesh(const std::string& _line_string, const std::shared_ptr<Material>& _material, const std::vector<Point>& _vs, const std::vector<vec3>& _vns, const std::vector<Texture_coordinate>& _vts) : m_material(_material), m_vs(_vs), m_vns(_vns), m_vts(_vts)
        {
            std::regex regex_pattern("[0-9]+\\/[0-9]+\\/[0-9]+");
            std::regex sub_regex_pattern("[0-9]+");
            std::sregex_iterator value(_line_string.begin(), _line_string.end(), regex_pattern);
            std::sregex_iterator end;
            int index = 0;
            for (; value != end; ++value, ++index)
            {
                std::string value_string = value->str();
                std::sregex_iterator sub_value(value_string.begin(), value_string.end(), sub_regex_pattern);
                m_vertices_indices[index] = std::stoi(sub_value->str()) - 1;
                ++sub_value;
                m_textures_indices[index] = std::stoi(sub_value->str()) - 1;
                ++sub_value;
                m_normals_indices[index] = std::stoi(sub_value->str()) - 1;
            }
            m_E1 = _vs.at(m_vertices_indices[1]) - _vs.at(m_vertices_indices[0]);
            m_E2 = _vs.at(m_vertices_indices[2]) - _vs.at(m_vertices_indices[0]);
        }
        ~Mesh() = default;

        inline Point point_A() const
        {
            return m_vs.at(m_vertices_indices[0]);
        }

        inline Point point_B() const
        {
            return m_vs.at(m_vertices_indices[1]);
        }

        inline Point point_C() const
        {
            return m_vs.at(m_vertices_indices[2]);
        }

        inline vec3 normal_A() const
        {
            return m_vns.at(m_normals_indices[0]);
        }

        inline vec3 normal_B() const
        {
            return m_vns.at(m_normals_indices[1]);
        }

        inline vec3 normal_C() const
        {
            return m_vns.at(m_normals_indices[2]);
        }

        virtual bool hit(const Ray& _ray, const double& _t_min, const double& _t_max, Hit_record& _record) const override;
        virtual bool create_bounding_box(Bounding_box& _output_box) const override;

    private:
        indices_type m_vertices_indices{ 0, 0, 0 };                         /* 顶点索引 */
        indices_type m_normals_indices{ 0, 0, 0 };                          /* 法向量索引 */
        indices_type m_textures_indices{ 0, 0, 0 };                         /* uv坐标索引 */
        vec3 m_E1;                                                          /* 第 一 条边 */
        vec3 m_E2;                                                          /* 第 二 条边 */
        const std::shared_ptr<Material> m_material;                         /* 材质 */
        const std::vector<Point>& m_vs;                                     /* 顶点 */
        const std::vector<vec3>& m_vns;                                     /* 法向量 */
        const std::vector<Texture_coordinate>& m_vts;                       /* uv坐标 */
    };
}

#endif // !GQY_MESH_H
