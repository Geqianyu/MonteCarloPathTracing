
#ifndef GQY_MATH_H
#define GQY_MATH_H

#include <cmath>
#include <memory>
#include <limits>
#include <random>

namespace Monte_carlo_path_tracing
{
    //---------------------------------------------- 常量 -----------------------------------------------
    extern const double infinity;                                   /* 无穷大 */
    extern const double sigma;                                      /* 误差 */
    extern const double Russian_roulette_probability;               /* 俄罗斯赌盘概率 */
    extern const double pi;                                         /* Π */
    //---------------------------------------------------------------------------------------------------

    //---------------------------------------------- 函数 -----------------------------------------------
    inline double degree_to_radians(double _degree)                                 /* 角度转弧度 */
    {
        return _degree * pi / 180.0;
    }

    inline double clamp(double _value, double _min_value, double _max_value)        /* 限制值的大小 */
    {
        if (_value < _min_value)
        {
            return _min_value;
        }
        if (_value > _max_value)
        {
            return _max_value;
        }
        return _value;
    }

    inline double random_double()                                                   /* 0-1 的随机数 */
    {
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);
        static std::mt19937 generator;
        return distribution(generator);
    }

    inline double random_double(double _min, double _max)                           /* _min 到 _max 的随机数 */
    {
        return _min + (_max - _min) * random_double();
    }

    inline int random_int(int _min, int _max)                                       /* _min 到 _max 的随机整数数 */
    {
        return static_cast<int>(random_double(static_cast<double>(_min), static_cast<double>(_max) + 1));
    }

    inline bool Russian_Roulette()                                                  /* 俄罗斯赌盘 */
    {
        return random_double() < Russian_roulette_probability;
    }
    //---------------------------------------------------------------------------------------------------

    //---------------------------------------------- vec2 -----------------------------------------------
    class vec2
    {
    public:
        vec2(double _u = 0.0, double _v = 0.0) : m_elements{ _u, _v } { }
        ~vec2() = default;

        inline vec2 operator - () const
        {
            return vec2(-u, -v);
        }

        inline vec2& operator = (const vec2& _vector)
        {
            for (int i = 0; i < 2; ++i)
            {
                m_elements[i] = _vector.m_elements[i];
            }
            return *this;
        }

        inline vec2& operator += (const vec2& _vector)
        {
            for (int i = 0; i < 2; ++i)
            {
                m_elements[i] += _vector.m_elements[i];
            }
            return *this;
        }

        inline vec2& operator *= (const double _scale)
        {
            for (int i = 0; i < 2; ++i)
            {
                m_elements[i] *= _scale;
            }
            return *this;
        }

        inline vec2& operator /= (const double _scale)
        {
            for (int i = 0; i < 2; ++i)
            {
                m_elements[i] /= _scale;
            }
            return *this;
        }

        inline double& operator [] (const int& _index)
        {
            return m_elements[_index];
        }

        inline const double& operator [] (const int& _index) const
        {
            return m_elements[_index];
        }

        inline friend vec2 operator + (const vec2& _vector1, const vec2& _vector2);
        inline friend vec2 operator - (const vec2& _vector1, const vec2& _vector2);
        inline friend vec2 operator * (const vec2& _vector1, const vec2& _vector2);
        inline friend vec2 operator * (double _scale, const vec2& _vector);
        inline friend vec2 operator * (const vec2& _vector, double _scale);
        inline friend vec2 operator / (const vec2& _vector, double _scale);

    public:
        double& u = m_elements[0];
        double& v = m_elements[1];

    private:
        double m_elements[2];
    };

    inline vec2 operator + (const vec2& _vector1, const vec2& _vector2)
    {
        return vec2(_vector1.m_elements[0] + _vector2.m_elements[0], _vector1.m_elements[1] + _vector2.m_elements[1]);
    }

    inline vec2 operator - (const vec2& _vector1, const vec2& _vector2)
    {
        return vec2(_vector1.m_elements[0] - _vector2.m_elements[0], _vector1.m_elements[1] - _vector2.m_elements[1]);
    }

    inline vec2 operator * (const vec2& _vector1, const vec2& _vector2)
    {
        return vec2(_vector1.m_elements[0] * _vector2.m_elements[0], _vector1.m_elements[1] * _vector2.m_elements[1]);
    }

    inline vec2 operator * (double _scale, const vec2& _vector)
    {
        return vec2(_vector.m_elements[0] * _scale, _vector.m_elements[1] * _scale);
    }

    inline vec2 operator * (const vec2& _vector, double _scale)
    {
        return _scale * _vector;
    }

    inline vec2 operator / (const vec2& _vector, double _scale)
    {
        return vec2(_vector.m_elements[0] / _scale, _vector.m_elements[1] / _scale);
    }
    //---------------------------------------------------------------------------------------------------

    //---------------------------------------------- vec3 -----------------------------------------------
    class vec3
    {
    public:
        vec3(double _x = 0.0, double _y = 0.0, double _z = 0.0) : m_elements{ _x, _y, _z } { }
        ~vec3() = default;

        inline double length() const
        {
            return std::sqrt(m_elements[0] * m_elements[0] + m_elements[1] * m_elements[1] + m_elements[2] * m_elements[2]);
        }

        inline bool near_zero() const
        {
            return !((m_elements[0] * m_elements[0] + m_elements[1] * m_elements[1] + m_elements[2] * m_elements[2]) > sigma);
        }

        inline vec3 operator - () const
        {
            return vec3(-m_elements[0], -m_elements[1], -m_elements[2]);
        }

        inline vec3& operator = (const vec3& _vector)
        {
            for (int i = 0; i < 3; ++i)
            {
                m_elements[i] = _vector.m_elements[i];
            }
            return *this;
        }

        inline vec3& operator += (const vec3& _vector)
        {
            for (int i = 0; i < 3; ++i)
            {
                m_elements[i] += _vector.m_elements[i];
            }
            return *this;
        }

        inline vec3& operator *= (const double _scale)
        {
            for (int i = 0; i < 3; ++i)
            {
                m_elements[i] *= _scale;
            }
            return *this;
        }

        inline vec3& operator /= (const double _scale)
        {
            for (int i = 0; i < 3; ++i)
            {
                m_elements[i] /= _scale;
            }
            return *this;
        }

        inline double& operator [] (const int& _index)
        {
            return m_elements[_index];
        }

        inline const double& operator [] (const int& _index) const
        {
            return m_elements[_index];
        }

        inline friend vec3 operator + (const vec3& _vector1, const vec3& _vector2);
        inline friend vec3 operator - (const vec3& _vector1, const vec3& _vector2);
        inline friend vec3 operator * (const vec3& _vector1, const vec3& _vector2);
        inline friend vec3 operator * (double _scale, const vec3& _vector);
        inline friend vec3 operator * (const vec3& _vector, double _scale);
        inline friend vec3 operator / (const vec3& _vector, double _scale);

        inline friend double dot(const vec3& _vector1, const vec3& _vector2);
        inline friend vec3 cross(const vec3& _vector1, const vec3& _vector2);

    public:
        double& x = m_elements[0];
        double& y = m_elements[1];
        double& z = m_elements[2];
        double& r = m_elements[0];
        double& g = m_elements[1];
        double& b = m_elements[2];

    private:
        double m_elements[3];
    };

    inline vec3 operator + (const vec3& _vector1, const vec3& _vector2)
    {
        return vec3(_vector1.m_elements[0] + _vector2.m_elements[0], _vector1.m_elements[1] + _vector2.m_elements[1], _vector1.m_elements[2] + _vector2.m_elements[2]);
    }

    inline vec3 operator - (const vec3& _vector1, const vec3& _vector2)
    {
        return vec3(_vector1.m_elements[0] - _vector2.m_elements[0], _vector1.m_elements[1] - _vector2.m_elements[1], _vector1.m_elements[2] - _vector2.m_elements[2]);
    }

    inline vec3 operator * (const vec3& _vector1, const vec3& _vector2)
    {
        return vec3(_vector1.m_elements[0] * _vector2.m_elements[0], _vector1.m_elements[1] * _vector2.m_elements[1], _vector1.m_elements[2] * _vector2.m_elements[2]);
    }

    inline vec3 operator * (double _scale, const vec3& _vector)
    {
        return vec3(_vector.m_elements[0] * _scale, _vector.m_elements[1] * _scale, _vector.m_elements[2] * _scale);
    }

    inline vec3 operator * (const vec3& _vector, double _scale)
    {
        return _scale * _vector;
    }

    inline vec3 operator / (const vec3& _vector, double _scale)
    {
        return vec3(_vector.m_elements[0] / _scale, _vector.m_elements[1] / _scale, _vector.m_elements[2] / _scale);
    }

    inline double dot(const vec3& _vector1, const vec3& _vector2)
    {
        return _vector1.m_elements[0] * _vector2.m_elements[0] + _vector1.m_elements[1] * _vector2.m_elements[1] + _vector1.m_elements[2] * _vector2.m_elements[2];
    }

    inline vec3 cross(const vec3& _vector1, const vec3& _vector2)
    {
        return vec3(_vector1.m_elements[1] * _vector2.m_elements[2] - _vector1.m_elements[2] * _vector2.m_elements[1], _vector1.m_elements[2] * _vector2.m_elements[0] - _vector1.m_elements[0] * _vector2.m_elements[2], _vector1.m_elements[0] * _vector2.m_elements[1] - _vector1.m_elements[1] * _vector2.m_elements[0]);
    }

    inline vec3 unit_vector(vec3 _vector)
    {
        return _vector / _vector.length();
    }

    inline vec3 reflect(const vec3& _input_vector, const vec3& _normal)
    {
        return _input_vector - 2 * dot(_input_vector, _normal) * _normal;
    }

    inline vec3 refract(const vec3& _input_vector, const vec3& _normal, double etai_over_etat)
    {
        double cos_theta = fmin(dot(_input_vector, _normal), 1.0);
        vec3 r_out_perp = etai_over_etat * (_input_vector + cos_theta * _normal);
        vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length() * r_out_perp.length())) * _normal;
        return r_out_perp + r_out_parallel;
    }

    inline vec3 random_unit_vector()
    {
        return unit_vector(vec3(random_double(), random_double(), random_double()));
    }
    //---------------------------------------------------------------------------------------------------

    using Point = vec3;
    using Color = vec3;
    using Texture_coordinate = vec2;
}

#endif // !GQY_MATH_
