#include<sgl/ext/stbi.hpp>
#include<stb/stb_image.h>
#include<memory>
namespace sgl{
    Texture<TextureType::TWO_D> load_image_stbi(std::filesystem::path file,bool rgba){
        Texture<TextureType::TWO_D> tex;
        int x,y,c;
        stbi_set_flip_vertically_on_load(true);
        std::unique_ptr<std::uint8_t,decltype([](std::uint8_t* p) static{stbi_image_free(p);})> p{stbi_load(file.c_str(),&x,&y,&c,3+rgba)};
        if(!p) throw std::runtime_error("Can't load image!");
        tex.allocate_static(rgba?sgl::ImageFormat::RGBA8:sgl::ImageFormat::RGB8,static_cast<std::uint32_t>(x),static_cast<std::uint32_t>(y));
        tex.upload(p.get(),rgba?sgl::PixelFormat::RGBA:sgl::PixelFormat::RGB,0,0,static_cast<std::uint32_t>(x),static_cast<std::uint32_t>(y));
        return tex;
    }
}
