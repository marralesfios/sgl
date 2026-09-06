#include<sgl/draw/rect.hpp>
namespace sgl{
    using namespace std::literals;
    MonochromeRectDrawer::MonochromeRectDrawer() : prog(u8"#version 460 core\nlayout(location=0)in vec2 p;layout(location=1)uniform vec2 X;layout(location=2)uniform vec2 W;void main(){gl_Position=vec4(p*W+X,0.,1.);}"sv,u8"#version 460 core\nlayout(location=0)uniform vec4 c;out vec4 C;void main(){C=c;}"sv){
        std::array vertices{
            1.0f,1.0f,
            1.0f,0.0f,
            0.0f,1.0f,
            0.0f,0.0f
        };
        vbo.allocate_static(vertices.size()*sizeof(float),GLBuffer::StaticConfig::NONE,vertices.data());
        vao.add_buffer(vbo,0,0,2*sizeof(float));
        vao.set_attr<float>(0,0,2,0,false);
    }
    void MonochromeRectDrawer::rect(cppp::fvec2 topleft,cppp::fvec2 dims,cppp::fvec4 color) const{
        vao.use();
        prog.set_uniform(0,color);
        prog.set_uniform(1,topleft);
        prog.set_uniform(2,dims);
        prog.use();
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    }
}
