#include<sgl/draw/line.hpp>
namespace sgl{
    using namespace std::literals;
    LineDrawer::LineDrawer() : prog(u8"#version 460 core\nlayout(location=0)in vec2 p;layout(location=1)in vec3 c;layout(location=0)out vec3 C;void main(){C=c;gl_Position=vec4(p,0.0,1.0);}"sv,u8"#version 460 core\nlayout(location=0)in vec3 c;out vec4 C;void main(){C=vec4(c,1.);}"sv){
        vbo.allocate_static(10*sizeof(float),GLBuffer::StaticConfig::UPLOAD);
        vao.add_buffer(vbo,0,0,5*sizeof(float));
        vao.set_attr<float>(0,0,2,0,false);
        vao.set_attr<float>(1,0,3,2*sizeof(float),false);
    }
    void LineDrawer::line(cppp::fvec2 start,cppp::fvec3 startc,cppp::fvec2 end,cppp::fvec3 endc) const{
        std::array vertices{
            start.x(),start.y(),startc.x(),startc.y(),startc.z(),
            end.x(),end.y(),endc.x(),endc.y(),endc.z()
        };
        vbo.upload(vertices.data(),vertices.size()*sizeof(float));
        vao.use();
        prog.use();
        glDrawArrays(GL_LINE_STRIP,0,2);
    }
}
