
#include "Image.h"

namespace Monte_carlo_path_tracing
{
    void Image::write_color(std::vector<Color> _colors)
    {
        // 像素的颜色
        Color pixel_color;

        // 颜色总计
        for (Color color : _colors)
        {
            pixel_color += color;
        }

        // SPP
        double size = static_cast<double>(_colors.size());

        // 计算最终颜色
        pixel_color /= size;

        // gama矫正
        double r = std::pow(pixel_color.r, 1 / 2.2);
        double g = std::pow(pixel_color.g, 1 / 2.2);
        double b = std::pow(pixel_color.b, 1 / 2.2);

        // 将像素颜色写入文件
        m_file << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
            << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
            << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
    }
}