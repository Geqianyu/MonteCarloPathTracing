
#include <fstream>
#include <regex>

#include "Obj.h"
#include "Log/Log.h"
#include "Core/Core.h"

namespace Monte_carlo_path_tracing
{
    void Obj::load_obj(const std::string& _file_path, const std::unordered_map<std::string, Color>& _radiances)
    {
        std::ifstream obj_file(_file_path);
        GQY_MONTE_CARLO_PATH_TRACING_ASSERT(obj_file.is_open(), "无法打开文件 " + _file_path);

        std::string line_string, current_material;
        std::vector<std::string> vs_string, vns_string, vts_string, materials_string;
        std::unordered_map<std::string, std::vector<std::string>> meshes_map;
        std::regex pattern("\\S+");
        while (std::getline(obj_file, line_string))
        {
            std::sregex_iterator value(line_string.begin(), line_string.end(), pattern);
            std::sregex_iterator end;
            if (value == end)
            {
                continue;
            }
            std::string key = value->str();
            ++value;
            if (key == "v")
            {
                double x = std::stod(value->str());
                ++value;
                double y = std::stod(value->str());
                ++value;
                double z = std::stod(value->str());
                m_vs.push_back(Point(x, y, z));
            }
            else if (key == "vt")
            {
                double u = std::stod(value->str());
                ++value;
                double v = std::stod(value->str());
                Texture_coordinate texture_coord(u, v);
                while (texture_coord.u > 1.0)
                {
                    texture_coord.u -= 1.0;
                }
                while (texture_coord.v > 1.0)
                {
                    texture_coord.v -= 1.0;
                }
                while (texture_coord.u < 0.0)
                {
                    texture_coord.u += 1.0;
                }
                while (texture_coord.v < 0.0)
                {
                    texture_coord.v += 1.0;
                }
                m_vts.push_back(texture_coord);
            }
            else if (key == "vn")
            {
                double x = std::stod(value->str());
                ++value;
                double y = std::stod(value->str());
                ++value;
                double z = std::stod(value->str());
                m_vns.push_back(vec3(x, y, z));
            }
            else if (key == "f")
            {
                std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(line_string, m_materials[current_material], m_vs, m_vns, m_vts);
                m_meshes.push_back(mesh);
                if (is_light(current_material))
                {
                    m_lights.push_back(std::make_shared<Light>(mesh, _radiances.at(current_material)));
                }
            }
            else if (key == "mtllib")
            {
                std::string mtl_file_path = _file_path.substr(0, _file_path.find_last_of('/')) + '/' + value->str();
                load_mtl(mtl_file_path, _radiances);
            }
            else if (key == "usemtl")
            {
                current_material = value->str();
            }
        }
        GQY_MONTE_CARLO_PATH_TRACING_INFO(_file_path + "加载成功");
    }

    void Obj::load_mtl(const std::string& _mtl_file_path, const std::unordered_map<std::string, Color>& _radiances)
    {
        std::ifstream mtl_file(_mtl_file_path);
        GQY_MONTE_CARLO_PATH_TRACING_ASSERT(mtl_file.is_open(), "无法打开文件 " + _mtl_file_path);

        std::regex pattern("\\S+");
        std::string line_string, current_material;
        std::getline(mtl_file, line_string);
        while (!line_string.empty())
        {
            std::sregex_iterator value(line_string.begin(), line_string.end(), pattern);
            std::sregex_iterator end;
            if (value != end)
            {
                std::string key = value->str();
                ++value;
                if (key == "newmtl")
                {
                    current_material = value->str();

                    std::getline(mtl_file, line_string);
                    std::sregex_iterator Kd_value(line_string.begin(), line_string.end(), pattern);
                    ++Kd_value;
                    double r = std::stod(Kd_value->str());
                    ++Kd_value;
                    double g = std::stod(Kd_value->str());
                    ++Kd_value;
                    double b = std::stod(Kd_value->str());
                    Color material_Kd(r, g, b);

                    std::getline(mtl_file, line_string);
                    std::sregex_iterator Ks_value(line_string.begin(), line_string.end(), pattern);
                    ++Ks_value;
                    r = std::stod(Ks_value->str());
                    ++Ks_value;
                    g = std::stod(Ks_value->str());
                    ++Ks_value;
                    b = std::stod(Ks_value->str());
                    Color material_Ks(r, g, b);

                    std::getline(mtl_file, line_string);
                    std::sregex_iterator Ns_value(line_string.begin(), line_string.end(), pattern);
                    ++Ns_value;
                    double Ns = std::stod(Ns_value->str());

                    std::getline(mtl_file, line_string);
                    std::sregex_iterator Ni_value(line_string.begin(), line_string.end(), pattern);
                    ++Ni_value;
                    double Ni = std::stod(Ni_value->str());

                    if (is_light(current_material))
                    {
                        m_materials.insert(std::pair<std::string, std::shared_ptr<Material>>(current_material, std::make_shared<Light_material>(_radiances.at(current_material))));
                    }
                    else
                    {
                        if (Ni - 1.0 > sigma)
                        {
                            Dielectric_material dielectric_material;
                            dielectric_material.set_Kd(material_Kd);
                            dielectric_material.set_Ks(material_Ks);
                            dielectric_material.set_Ni(Ni);
                            dielectric_material.set_Ns(Ns);
                            m_materials.insert(std::pair<std::string, std::shared_ptr<Material>>(current_material, std::make_shared<Dielectric_material>(dielectric_material)));
                        }
                        else
                        {
                            Glossy_material glossy_material;
                            glossy_material.set_Kd(material_Kd);
                            glossy_material.set_Ks(material_Ks);
                            glossy_material.set_Ni(Ni);
                            glossy_material.set_Ns(Ns);
                            m_materials.insert(std::pair<std::string, std::shared_ptr<Material>>(current_material, std::make_shared<Glossy_material>(glossy_material)));
                        }
                    }
                }
            }
            std::getline(mtl_file, line_string);
            std::sregex_iterator next_value(line_string.begin(), line_string.end(), pattern);
            if (next_value->str() == "map_Kd")
            {
                ++next_value;
                std::string map_file_path = _mtl_file_path.substr(0, _mtl_file_path.find_last_of('/')) + '/' + next_value->str();
                m_materials[current_material]->set_Kd(map_file_path);
                std::getline(mtl_file, line_string);
            }
        }
        GQY_MONTE_CARLO_PATH_TRACING_INFO(_mtl_file_path + "加载成功");
    }
}