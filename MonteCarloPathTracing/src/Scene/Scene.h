
#ifndef GQY_SCENE_H
#define GQY_SCENE_H

#include <memory>

#include "Image/Image.h"
#include "Math/Math.h"
#include "Camera/Camera.h"
#include "Model/Obj.h"

namespace Monte_carlo_path_tracing
{
    class Scene
    {
    public:
        Scene() = default;
        Scene(std::string _file_direction, int _spp);
        ~Scene() = default;

        void render();

    private:
        Color shading(const Ray& _ray, const Hit_record& _rec);

    private:
        std::shared_ptr<Camera> m_camera;
        std::shared_ptr<Obj> m_obj;
        std::shared_ptr<Image> m_image;
        int m_spp{ 0 };
    };
}

#endif // !GQY_SCENE_H
