#pragma once
#include"../gl.hpp"
#include<filesystem>
namespace sgl{
    Texture<TextureType::TWO_D> load_image_stbi(std::filesystem::path file,bool rgba);
}
