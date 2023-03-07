
#ifndef GQY_TEXTURE_H
#define GQY_TEXTURE_H

#include <string>

#include "Math/Math.h"

namespace Monte_carlo_path_tracing
{
    //-------------------------------------------- Texture ----------------------------------------------
    class Texture
    {
    public:
        Texture() = default;
        ~Texture() = default;

        virtual Color value(const Texture_coordinate& _texture_coord) const = 0;
    };
    //---------------------------------------------------------------------------------------------------

    //------------------------------------------ Solid_color --------------------------------------------
    class Solid_color : public Texture
    {
    public:
        Solid_color() = default;
        Solid_color(const Color& _color) : m_color{ _color } { }
        Solid_color(double _red, double _green, double _blue) : m_color(_red, _green, _blue) { }
        ~Solid_color() = default;

        virtual Color value(const Texture_coordinate& _texture_coord) const override;

    private:
        Color m_color;
    };
    //---------------------------------------------------------------------------------------------------

    //----------------------------------------- Image_texture -------------------------------------------
    class Image_texture : public Texture
    {
    public:
        Image_texture() : m_data(nullptr), m_width(0), m_height(0), m_channels(0) { }
        Image_texture(std::string _texture_file_name);
        ~Image_texture();

        virtual Color value(const Texture_coordinate& _texture_coord) const override;

    private:
        unsigned char* m_data;
        int m_width, m_height;
        int m_channels;
    };
    //---------------------------------------------------------------------------------------------------
}

#endif // !GQY_TEXTURE_H
