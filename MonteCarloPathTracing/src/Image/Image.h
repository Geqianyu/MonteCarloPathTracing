
#ifndef GQY_IMAGE_H
#define GQY_IMAGE_H

#include <string>
#include <fstream>
#include <vector>

#include "Core/Core.h"
#include "Log/Log.h"
#include "Math/Math.h"

namespace Monte_carlo_path_tracing
{
    class Image
    {
    public:
        Image(std::string _file_name, int _image_width, int _image_height)
        {
            m_file_path = _file_name;

            // 以 写 方式打开文件，若文件不存在则会创建该文件
            m_file.open(_file_name.c_str(), std::ios_base::out);

            // 如果文件打开失败则输出文件打开失败
            GQY_MONTE_CARLO_PATH_TRACING_ASSERT(m_file.is_open(), _file_name + " 打开失败");

            // 文件打开成功则写入文件头部信息
            m_file << "P3\n" << _image_width << " " << _image_height << " \n255\n";
        }

        ~Image()
        {
            m_file.close();
        }

        void write_color(std::vector<Color> _colors);

        inline std::string path()
        {
            return m_file_path;
        }

    private:
        std::fstream m_file;
        std::string m_file_path;
    };
}

#endif // !GQY_IMAGE_H
