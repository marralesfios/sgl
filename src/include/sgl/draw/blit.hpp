#pragma once
#include"../gl.hpp"
#include<cppp/vector.hpp>
#include<concepts>
namespace sgl{
    namespace detail{
        class BlitterBase{
            protected:
                mutable GLBuffer vbo;
                VAO vao;
                BlitterBase();
                template<typename D>
                void blit_rel(this const D& self,const Texture<TextureType::TWO_D>& tex,cppp::fvec2 pos,cppp::fvec2 size_per_px){
                    self.blit_abs(tex,pos,size_per_px*cppp::fvec2(tex.size()));
                }
        };
    }
    template<typename T>
    concept Blitter = std::derived_from<T,detail::BlitterBase> && requires(const T& b,const Texture<TextureType::TWO_D>& tex,cppp::fvec2 pos,cppp::fvec2 size){
        {b.blit_abs(tex,pos,size)};
        {b.blit_rel(tex,pos,size)};
    };
    class ProgramBlitter : public detail::BlitterBase{
        Program p;
        public:
            ProgramBlitter(cppp::sv v,cppp::sv f);
            void blit_abs(const Texture<TextureType::TWO_D>& tex,cppp::fvec2 pos,cppp::fvec2 size) const;
    };
    class SimpleBlitter : public ProgramBlitter{
        Program p;
        public:
            SimpleBlitter();
            using BlitterBase::blit_rel;
    };
}
