
#ifndef GQY_BVH_H
#define GQY_BVH_H

#include "Hittable/Hittable.h"
#include "Model/Mesh.h"
#include "Log/Log.h"
#include "Core/Core.h"

namespace Monte_carlo_path_tracing
{
    class BVH_node : public Hittable
    {
    public:
        BVH_node() = default;
        BVH_node(std::vector<std::shared_ptr<Mesh>>& _meshes)
        {
            create(_meshes, 0, _meshes.size());
        }
        BVH_node(std::vector<std::shared_ptr<Mesh>>& _meshes, size_t _start, size_t _end)
        {
            create(_meshes, _start, _end);
        }
        ~BVH_node() = default;

        void create(std::vector<std::shared_ptr<Mesh>>& _meshes, size_t _start, size_t _end);
        virtual bool hit(const Ray& _ray, const double& _t_min, const double& _t_max, Hit_record& _record) const override;
        virtual bool create_bounding_box(Bounding_box& _output_box) const override;

    private:
        std::shared_ptr<Hittable> m_left;
        std::shared_ptr<Hittable> m_right;
        Bounding_box m_bounding_box;
    };

    inline bool box_compare(const std::shared_ptr<Hittable>& _first, const std::shared_ptr<Hittable>& _second, int _axis)
    {
        Bounding_box box_a, box_b;
        GQY_MONTE_CARLO_PATH_TRACING_ASSERT(_first->create_bounding_box(box_a), "无包围盒可创建");
        GQY_MONTE_CARLO_PATH_TRACING_ASSERT(_second->create_bounding_box(box_b), "无包围盒可创建");
        return box_a.minimum[_axis] < box_b.minimum[_axis];
    }

    bool box_x_compare(const std::shared_ptr<Hittable>& _first, const std::shared_ptr<Hittable>& _second);
    bool box_y_compare(const std::shared_ptr<Hittable>& _first, const std::shared_ptr<Hittable>& _second);
    bool box_z_compare(const std::shared_ptr<Hittable>& _first, const std::shared_ptr<Hittable>& _second);
}

#endif // !GQY_BVH_H
