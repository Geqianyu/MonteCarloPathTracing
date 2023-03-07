
#include <tinyxml2.h>
#include <regex>

#include "Scene.h"

namespace Monte_carlo_path_tracing
{
    Scene::Scene(std::string _file_direction, int _spp) : m_spp(_spp)
    {
        size_t file_index = _file_direction.find_last_of("/");
        std::string xml_file_path = _file_direction + _file_direction.substr(file_index, _file_direction.length() - file_index) + ".xml";
        std::string obj_file_path = _file_direction + _file_direction.substr(file_index, _file_direction.length() - file_index) + ".obj";
        std::string result_path = "result" + _file_direction.substr(file_index, _file_direction.length() - file_index) + ".ppm";
        std::unordered_map<std::string, Color> radiances;
        std::regex pattern("-[0-9]+(.[0-9]+)*|[0-9]+(.[0-9]+)*");

        tinyxml2::XMLDocument doc;
        doc.LoadFile(xml_file_path.c_str());
        tinyxml2::XMLElement* camera_element = doc.FirstChildElement("camera");
        tinyxml2::XMLElement* light_element = doc.FirstChildElement("light");
        std::string eye(camera_element->FirstChildElement("eye")->Attribute("value"));
        std::string lookat(camera_element->FirstChildElement("lookat")->Attribute("value"));
        std::string up(camera_element->FirstChildElement("up")->Attribute("value"));
        std::string fovy(camera_element->FirstChildElement("fovy")->Attribute("value"));
        std::string width(camera_element->FirstChildElement("width")->Attribute("value"));
        std::string height(camera_element->FirstChildElement("height")->Attribute("value"));

        std::sregex_iterator eye_value(eye.begin(), eye.end(), pattern);
        double x = std::stod(eye_value->str());
        ++eye_value;
        double y = std::stod(eye_value->str());
        ++eye_value;
        double z = std::stod(eye_value->str());
        Point camera_position(x, y, z);

        std::sregex_iterator lookat_value(lookat.begin(), lookat.end(), pattern);
        x = std::stod(lookat_value->str());
        ++lookat_value;
        y = std::stod(lookat_value->str());
        ++lookat_value;
        z = std::stod(lookat_value->str());
        Point camera_lookat(x, y, z);

        std::sregex_iterator up_value(up.begin(), up.end(), pattern);
        x = std::stod(up_value->str());
        ++up_value;
        y = std::stod(up_value->str());
        ++up_value;
        z = std::stod(up_value->str());
        vec3 camera_up(x, y, z);

        int width_value = std::stoi(width);
        int height_value = std::stoi(height);
        double fovy_value = std::stod(fovy);
        m_camera = std::make_shared<Camera>(camera_position, camera_lookat, camera_up, fovy_value, width_value, height_value);
        GQY_MONTE_CARLO_PATH_TRACING_INFO("创建相机成功");

        while (light_element != nullptr)
        {
            std::string light_name(light_element->Attribute("mtlname"));
            std::string light_radiance(light_element->Attribute("radiance"));
            std::sregex_iterator radiance_value(light_radiance.begin(), light_radiance.end(), pattern);
            double r = std::stod(radiance_value->str());
            ++radiance_value;
            double g = std::stod(radiance_value->str());
            ++radiance_value;
            double b = std::stod(radiance_value->str());
            Color radiance(r, g, b);
            radiances.insert(std::pair<std::string, Color>(light_name, radiance));
            light_element = light_element->NextSiblingElement();
        }
        GQY_MONTE_CARLO_PATH_TRACING_INFO(xml_file_path + " 加载成功");

        m_obj = std::make_shared<Obj>(obj_file_path, radiances);

        m_image = std::make_shared<Image>(result_path, width_value, height_value);

        GQY_MONTE_CARLO_PATH_TRACING_INFO("场景加载成功");
    }

    void Scene::render()
    {
        GQY_MONTE_CARLO_PATH_TRACING_INFO("开始渲染 ...");
        int width = m_camera->width();
        int height = m_camera->height();
        for (int j = 0; j < height; ++j)
        {
            for (int i = 0; i < width; ++i)
            {
                std::vector<Color> color(m_spp);
                #pragma omp parallel for
                for (int k = 0; k < m_spp; ++k)
                {
                    Ray ray = m_camera->get_ray(i, j);
                    Hit_record record;
                    if (m_obj->bvh().hit(ray, 0, infinity, record))
                    {
                        color.at(k) = shading(ray, record);
                    }
                }
                m_image->write_color(color);
            }
            GQY_MONTE_CARLO_PATH_TRACING_TRACE("{0}%", static_cast<double>(j) / height * 100.0);
        }
        GQY_MONTE_CARLO_PATH_TRACING_INFO("渲染完成，结果保存在: " + m_image->path());
    }

    Color Scene::shading(const Ray& _ray_in, const Hit_record& _rec)
    {
        Ray scattered_ray;
        Color attenuation;
        // 如果照射在光源上直接返回
        if (!_rec.material_ptr->scatter(_ray_in, _rec, attenuation, scattered_ray))
        {
            return attenuation;
        }

        // 直接光照，直接光照只对漫反射部分有贡献
        Color directional_color;
        int light_size = static_cast<int>(m_obj->lights().size());
        std::vector<Color> local_directional_color(light_size);
        #pragma omp parallel for
        for (int index = 0; index < light_size; ++index)
        {
            double alpha = random_double();
            double beta = random_double();
            double gama = random_double();
            m_obj->lights()[index]->calculate(alpha, beta, gama);
            Point light_position = m_obj->lights()[index]->center();
            vec3 light_normal = m_obj->lights()[index]->normal();
            vec3 directional_ray_direction = unit_vector(light_position - _rec.position);
            Ray light_ray(_rec.position, directional_ray_direction);
            Hit_record directional_illumination_record;
            if (dot(light_normal, directional_ray_direction) < 0
                && dot(_rec.normal, directional_ray_direction) > 0
                && m_obj->bvh().hit(light_ray, sigma, infinity, directional_illumination_record)
                && (light_ray.at(directional_illumination_record.time) - light_position).near_zero())
            {
                Color f_r = _rec.material_ptr->get_Kd(_rec.texture_coord) / pi;
                local_directional_color[index] = m_obj->lights()[index]->light_material->get_radiance() * std::abs(dot(light_normal, directional_ray_direction)) * std::abs(dot(unit_vector(_rec.normal), directional_ray_direction)) * m_obj->lights()[index]->area / directional_illumination_record.time / directional_illumination_record.time * f_r;
            }
        }
        for (size_t index = 0; index < m_obj->lights().size(); index++)
        {
            directional_color += local_directional_color[index];
        }

        // 间接光照
        Color indirectional_color;
        if (Russian_Roulette())
        {
            Hit_record record;
            if (m_obj->bvh().hit(scattered_ray, sigma, infinity, record))
            {
                Ray temp_scattered_ray;
                Color temp_attenuation;
                if (record.material_ptr->scatter(scattered_ray, record, temp_attenuation, temp_scattered_ray))
                {
                    indirectional_color = attenuation * shading(scattered_ray, record) / Russian_roulette_probability;
                }
                else if (scattered_ray.type() == Ray_type::Diffuse)
                {
                    return directional_color;
                }
                else if (!record.front_face)
                {
                    indirectional_color = attenuation * temp_attenuation;
                }
            }
        }
        return directional_color + indirectional_color;
    }
}