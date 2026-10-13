#include<sgl/draw/line.hpp>
namespace sgl{
    using namespace std::literals;
    LineDrawer::LineDrawer() : prog(u8"#version 460 core\nlayout(location=0)in vec2 p;layout(location=1)in vec4 c;layout(location=0)out vec4 C;void main(){C=c;gl_Position=vec4(p,0.,1.);}"sv,u8"#version 460 core\nlayout(location=0)in vec4 c;out vec4 C;void main(){C=c;}"sv){
        vbo.allocate_static(12*sizeof(float),GLBuffer::StaticConfig::UPLOAD);
        vao.add_buffer(vbo,0,0,6*sizeof(float));
        vao.set_attr<float>(0,0,2,0,false);
        vao.set_attr<float>(1,0,4,2*sizeof(float),false);
    }
    void LineDrawer::line(cppp::fvec2 start,cppp::fvec4 startc,cppp::fvec2 end,cppp::fvec4 endc) const{
        std::array vertices{
            start.x(),start.y(),startc.x(),startc.y(),startc.z(),startc.w(),
            end.x(),end.y(),endc.x(),endc.y(),endc.z(),endc.w()
        };
        vbo.upload(vertices.data(),vertices.size()*sizeof(float));
        vao.use();
        prog.use();
        glDrawArrays(GL_LINE_STRIP,0,2);
    }
}
