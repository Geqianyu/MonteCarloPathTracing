
#ifndef GQY_OBJ_H
#define GQY_OBJ_H

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <regex>

#include "Math/Math.h"
#include "Material/Material.h"
#include "Light/Light.h"
#include "BVH/BVH.h"
#include "Mesh.h"

namespace Monte_carlo_path_tracing
{
    class Obj
    {
    public:
        Obj() = default;
        Obj(const Obj& _obj) : m_vs(_obj.m_vs), m_vns(_obj.m_vns), m_vts(_obj.m_vts), m_meshes(_obj.m_meshes), m_materials(_obj.m_materials) { }
        Obj(const std::string& _file_path, const std::unordered_map<std::string, Color>& _radiances)
        {
            load_obj(_file_path, _radiances);
            create_bvh();
        }
        ~Obj() = default;

        void load_obj(const std::string& _file_path, const std::unordered_map<std::string, Color>& _radiances);

        inline const std::vector<Point>& vs() const
        {
            return m_vs;
        }

        inline const std::vector<vec3>& vns() const
        {
            return m_vns;
        }

        inline const std::vector<Texture_coordinate>& vts() const
        {
            return m_vts;
        }

        inline const BVH_node& bvh() const
        {
            return m_bvh;
        }

        inline const std::vector<std::shared_ptr<Light>>& lights() const
        {
            return m_lights;
        }

    private:
        void load_mtl(const std::string& _file_path, const std::unordered_map<std::string, Color>& _radiances);

        inline void create_bvh()
        {
            m_bvh.create(m_meshes, 0, m_meshes.size());
            GQY_MONTE_CARLO_PATH_TRACING_INFO("BVH 创建成功");
        }

        inline bool is_light(const std::string& _current_material)
        {
            std::regex pattern("^Light.*");
            std::smatch match_result;
            return std::regex_match(_current_material, match_result, pattern);
        }

    public:
        std::vector<Point> m_vs;                                                          /* 顶点 */
        std::vector<vec3> m_vns;                                                          /* 顶点法向量 */
        std::vector<Texture_coordinate> m_vts;                                            /* 顶点纹理坐标 */
        std::vector<std::shared_ptr<Mesh>> m_meshes;                                      /* 保存三角形的索引 */
        std::vector<std::shared_ptr<Light>> m_lights;                                     /* 光源 */
        std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;           /* 材质 */
        BVH_node m_bvh;                                                                   /* 包围盒层级结构 */
    };
}

#endif // !GQY_OBJ_H
