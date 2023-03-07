
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Texture.h"
#include "Core/Core.h"
#include "Log/Log.h"

namespace Monte_carlo_path_tracing
{
    //------------------------------------------ Solid_color --------------------------------------------
    Color Solid_color::value(const Texture_coordinate& _texture_coord) const
    {
        return m_color;
    }
    //---------------------------------------------------------------------------------------------------

    //----------------------------------------- Image_texture -------------------------------------------
    Image_texture::Image_texture(std::string _texture_file_name)
    {
        stbi_set_flip_vertically_on_load(true);
        m_data = stbi_load(_texture_file_name.c_str(), &m_width, &m_height, &m_channels, 0);
        GQY_MONTE_CARLO_PATH_TRACING_ASSERT(m_data, _texture_file_name + " 打开失败");
    }

    Image_texture::~Image_texture()
    {
        if (m_data != nullptr)
        {
            stbi_image_free(m_data);
        }
    }

    Color Image_texture::value(const Texture_coordinate& _texture_coord) const
    {
        if (m_data == nullptr)
        {
            return Color(0, 0, 0);
        }

        // 线性插值
        double x_coord = _texture_coord.u * m_width;
        double y_coord = _texture_coord.v * m_height;
        int x_max = static_cast<int>(x_coord + 0.5);
        int y_max = static_cast<int>(y_coord + 0.5);
        int x_min = x_max - 1;
        int y_min = y_max - 1;
        double x_min_weight = static_cast<double>(x_max) + 0.5 - x_coord;
        double x_max_weight = 1.0 - x_min_weight;
        double y_min_weight = static_cast<double>(y_max) + 0.5 - y_coord;
        double y_max_weight = 1.0 - y_min_weight;
        x_max = (x_max + m_width) % m_width;
        x_min = (x_min + m_width) % m_width;
        y_max = (y_max + m_height) % m_height;
        y_min = (y_min + m_height) % m_height;
        Color left_bottom = Color(static_cast<double>(m_data[m_channels * y_min * m_width + m_channels * x_min]) / 255.0, static_cast<double>(m_data[m_channels * y_min * m_width + m_channels * x_min + 1]) / 255.0, static_cast<double>(m_data[m_channels * y_min * m_width + m_channels * x_min + 2]) / 255.0);
        Color right_bottom = Color(static_cast<double>(m_data[m_channels * y_min * m_width + m_channels * x_max]) / 255.0, static_cast<double>(m_data[m_channels * y_min * m_width + m_channels * x_max + 1]) / 255.0, static_cast<double>(m_data[m_channels * y_min * m_width + m_channels * x_max + 2]) / 255.0);
        Color left_top = Color(static_cast<double>(m_data[m_channels * y_max * m_width + m_channels * x_min]) / 255.0, static_cast<double>(m_data[m_channels * y_max * m_width + m_channels * x_min + 1]) / 255.0, static_cast<double>(m_data[m_channels * y_max * m_width + m_channels * x_min + 2]) / 255.0);
        Color right_top = Color(static_cast<double>(m_data[m_channels * y_max * m_width + m_channels * x_max]) / 255.0, static_cast<double>(m_data[m_channels * y_max * m_width + m_channels * x_max + 1]) / 255.0, static_cast<double>(m_data[m_channels * y_max * m_width + m_channels * x_max + 2]) / 255.0);
        Color pixel = y_min_weight * (x_min_weight * left_bottom + x_max_weight * right_bottom) + y_max_weight * (x_min_weight * left_top + x_max_weight * right_top);

        // gama 矫正
        pixel.r = std::pow(pixel.r, 2.2);
        pixel.g = std::pow(pixel.g, 2.2);
        pixel.b = std::pow(pixel.b, 2.2);
        return pixel;
    }
    //---------------------------------------------------------------------------------------------------
}