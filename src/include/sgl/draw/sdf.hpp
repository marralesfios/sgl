#pragma once
#include"../gl.hpp"
#include<cppp/vector.hpp>
#include<concepts>
namespace sgl{
    class SDFRenderer{
        mutable GLBuffer vbo;
        VAO vao;
        mutable Program p;
        public:
            SDFRenderer();
            void draw_stretched(const Texture<TextureType::TWO_D>& sdf,cppp::fvec2 pos,cppp::fvec2 size,cppp::fvec3 color) const;
            void draw(const Texture<TextureType::TWO_D>& sdf,cppp::fvec2 pos,cppp::fvec2 size_per_px,cppp::fvec3 color) const{
                draw_stretched(sdf,pos,size_per_px*cppp::fvec2(sdf.size()),color);
            }
    };
}
