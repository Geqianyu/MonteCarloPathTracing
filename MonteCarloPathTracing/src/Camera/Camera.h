
#ifndef GQY_CAMERA_H
#define GQY_CAMERA_H

#include "Math/Math.h"
#include "Ray/Ray.h"

namespace Monte_carlo_path_tracing
{
    class Camera
    {
    public:
        Camera(Point _position, Point _lookat, vec3 _up, double _fovy, int _width, int _height) : m_position(_position), m_direction(unit_vector(_lookat - _position)), m_fovy(degree_to_radians(_fovy)), m_width(_width), m_height(_height), m_aspect(static_cast<double>(_width) / _height)
        {
            m_right = unit_vector(cross(m_direction, _up));
            m_up = unit_vector(cross(m_right, m_direction));
            m_z = -m_direction;
            m_x = m_right;
            m_y = m_up;
            double tan_theta = std::tan(m_fovy / 2.0);
            m_up = tan_theta * m_up;
            m_right = tan_theta * m_aspect * m_right;
            m_start = m_position + m_direction + m_up - m_right;
            m_up *= 2.0;
            m_right *= 2.0;
        }
        ~Camera() = default;

        inline Ray get_ray(int _x, int _y) const
        {
            // (0, 0) 为 左上角 光线，沿 右 x 增加, 沿 下 y 增加
            double u = (static_cast<double>(_x) + random_double()) / m_width;
            double v = (static_cast<double>(_y) + random_double()) / m_height;
            Point lookat = m_start + u * m_right - v * m_up;
            return Ray(m_position, lookat - m_position);
        }

        inline int width()
        {
            return m_width;
        }

        inline int height()
        {
            return m_height;
        }

    private:
        Point m_position;                           /* 相机位置 */
        Point m_start;                              /* 视野起始点，左上角 */
        vec3 m_direction;                           /* z 轴反方向 */
        vec3 m_up;                                  /* y 轴正方向 */
        vec3 m_right;                               /* x 轴正 */
        vec3 m_x, m_y, m_z;                         /* x 轴, y 轴, z 轴 */
        double m_aspect;                            /* 宽 / 高 */
        double m_fovy;                              /* 是广角，弧度角 */
        int m_width;                                /* 宽度像素 */
        int m_height;                               /* 高度像素 */
    };
}

#endif // !GQY_CAMERA_H
