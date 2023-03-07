
#include <memory>
#include <algorithm>

#include "BVH.h"

namespace Monte_carlo_path_tracing
{
    void BVH_node::create(std::vector<std::shared_ptr<Mesh>>& _meshes, size_t _start, size_t _end)
    {
        std::vector<std::shared_ptr<Mesh>>& objects = _meshes;
        int axis = random_int(0, 2);
        auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;
        size_t object_span = _end - _start;

        if (object_span == 1)
        {
            m_left = m_right = objects[_start];
        }
        else if (object_span == 2)
        {
            if (comparator(objects[_start], objects[_start + 1]))
            {
                m_left = objects[_start];
                m_right = objects[_start + 1];
            }
            else
            {
                m_left = objects[_start + 1];
                m_right = objects[_start];
            }
        }
        else
        {
            std::sort(objects.begin() + _start, objects.begin() + _end, comparator);

            auto mid = _start + object_span / 2;
            m_left = std::make_shared<BVH_node>(objects, _start, mid);
            m_right = std::make_shared<BVH_node>(objects, mid, _end);
        }

        Bounding_box box_left, box_right;
        GQY_MONTE_CARLO_PATH_TRACING_ASSERT(m_left->create_bounding_box(box_left), "无包围盒可创建");
        GQY_MONTE_CARLO_PATH_TRACING_ASSERT(m_right->create_bounding_box(box_right), "无包围盒可创建");
        m_bounding_box = surrounding_box(box_left, box_right);
    }

    bool BVH_node::hit(const Ray& _ray, const double& _t_min, const double& _t_max, Hit_record& _record) const
    {
        if (!m_bounding_box.hit(_ray, _t_min, _t_max))
        {
            return false;
        }

        bool hit_left = m_left->hit(_ray, _t_min, _t_max, _record);
        bool hit_right = m_right->hit(_ray, _t_min, hit_left ? _record.time : _t_max, _record);

        return hit_left || hit_right;
    }

    bool BVH_node::create_bounding_box(Bounding_box& _output_box) const
    {
        _output_box = m_bounding_box;
        return true;
    }

    bool box_x_compare(const std::shared_ptr<Hittable>& _first, const std::shared_ptr<Hittable>& _second)
    {
        return box_compare(_first, _second, 0);
    }

    bool box_y_compare(const std::shared_ptr<Hittable>& _first, const std::shared_ptr<Hittable>& _second)
    {
        return box_compare(_first, _second, 1);
    }

    bool box_z_compare(const std::shared_ptr<Hittable>& _first, const std::shared_ptr<Hittable>& _second)
    {
        return box_compare(_first, _second, 2);
    }
}