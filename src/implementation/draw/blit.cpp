#include<sgl/draw/blit.hpp>
#include<sgl/draw/impl/buffer_creation.hpp>
#include<cppp/string.hpp>
namespace sgl{
    detail::BlitterBase::BlitterBase(){
        create_buffer_2_2_4(vao,vbo);
    }
    ProgramBlitter::ProgramBlitter(cppp::sv v,cppp::sv f) : p(v,f){}
    void ProgramBlitter::blit_abs(const Texture<TextureType::TWO_D>& tex,cppp::fvec2 pos,cppp::fvec2 size) const{
        float l = pos.x();
        float r = l + size.x();
        float t = pos.y();
        float b = t - size.y();
        std::array vertices{
            r,b,1.0f,1.0f,
            r,t,1.0f,0.0f,
            l,b,0.0f,1.0f,
            l,t,0.0f,0.0f
        };
        vbo.upload(vertices.data(),vertices.size()*sizeof(float));
        vao.use();
        p.use();
        glActiveTexture(GL_TEXTURE0);
        tex.bind();
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    }
    using namespace std::literals;
    SimpleBlitter::SimpleBlitter() : ProgramBlitter(u8"#version 460 core\nlayout(location=0)in vec2 p;layout(location=1)in vec2 t;layout(location=0)out vec2 T;void main(){T=t;gl_Position=vec4(p,0.,1.);}"sv,u8"#version 460 core\nout vec4 c;layout(location=0)in vec2 t;layout(location=0)uniform sampler2D i;void main(){c=texture(i,t);}"sv){}
}
