#pragma once
#include"../../gl.hpp"
namespace sgl::detail{
    inline void create_buffer_2_2_4(VAO& vao,GLBuffer& vbo){
        vbo.allocate_static(16*sizeof(float),GLBuffer::StaticConfig::UPLOAD);
        vao.add_buffer(vbo,0,0,4*sizeof(float));
        vao.set_attr<float>(0,0,2,0,false);
        vao.set_attr<float>(1,0,2,2*sizeof(float),false);
    }
}
