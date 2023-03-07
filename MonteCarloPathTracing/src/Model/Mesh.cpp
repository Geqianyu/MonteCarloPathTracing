
#include "Mesh.h"

namespace Monte_carlo_path_tracing
{
    bool Mesh::hit(const Ray& _ray, const double& _t_min, const double& _t_max, Hit_record& _record) const
    {
        vec3 S = _ray.origin() - m_vs.at(m_vertices_indices[0]);
        vec3 S1 = cross(_ray.direction(), m_E2);
        vec3 S2 = cross(S, m_E1);
        vec3 result = 1.0 / dot(S1, m_E1) * vec3(dot(S2, m_E2), dot(S1, S), dot(S2, _ray.direction()));
        if (result.y < 0.0 || result.z < 0.0 || (1.0 - result.y - result.z) < 0.0)
        {
            return false;
        }
        if (result.y + result.z > 1.0)
        {
            return false;
        }
        if (result.x < _t_min || result.x > _t_max)
        {
            return false;
        }

        double index = 1.0f - result.y - result.z;
        vec3 outward_normal = unit_vector(index * m_vns.at(m_normals_indices[0]) + result.y * m_vns.at(m_normals_indices[1]) + result.z * m_vns.at(m_normals_indices[2]));
        _record.time = result.x;
        _record.position = index * m_vs.at(m_vertices_indices[0]) + result.y * m_vs.at(m_vertices_indices[1]) + result.z * m_vs.at(m_vertices_indices[2]);
        _record.texture_coord = index * m_vts.at(m_textures_indices[0]) + result.y * m_vts.at(m_textures_indices[1]) + result.z * m_vts.at(m_textures_indices[2]);
        _record.material_ptr = m_material;
        _record.set_face_normal(_ray, outward_normal);

        return true;
    }

    bool Mesh::create_bounding_box(Bounding_box& _output_box) const
    {
        Point small(infinity, infinity, infinity);
        Point big(-infinity, -infinity, -infinity);
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                small[j] = fmin(m_vs.at(m_vertices_indices[i])[j], small[j] - sigma);
                big[j] = fmax(m_vs.at(m_vertices_indices[i])[j], big[j] + sigma);
            }
        }
        _output_box = Bounding_box(small, big);
        return true;
    }
}