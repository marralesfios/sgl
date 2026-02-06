#include<sgl/draw/sdf.hpp>
#include<sgl/draw/impl/buffer_creation.hpp>
namespace sgl{
    using namespace std::literals;
    SDFRenderer::SDFRenderer() : p(u8"#version 460 core\nlayout(location=0)in vec2 p;layout(location=1)in vec2 t;layout(location=0)out vec2 T;void main(){T=t;gl_Position=vec4(p,0.,1.);}"sv,u8"#version 460 core\nout vec4 C;layout(location=0)in vec2 t;layout(location=0)uniform sampler2D i;layout(location=1)uniform vec3 c;void main(){C=vec4(c,smoothstep(.495,.505,texture(i,t).r));}"sv){
        create_buffer_2_2_4(vao,vbo);
    }
    void SDFRenderer::draw_stretched(const Texture<TextureType::TWO_D>& sdf,cppp::fvec2 pos,cppp::fvec2 size,cppp::fvec3 color) const{
        p.set_uniform(1,color);
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
        sdf.bind();
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    }
}
