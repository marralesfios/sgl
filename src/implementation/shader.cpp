#include<sgl/gl.hpp>
namespace sgl{
    void Shader::load(cppp::sv s){
        const char* data = reinterpret_cast<const char*>(s.data());
        std::int32_t len = static_cast<std::int32_t>(s.size());
        glShaderSource(handle,1,&data,&len);
        glCompileShader(handle);
        
        std::int32_t success;
        glGetShaderiv(handle,GL_COMPILE_STATUS,&success);
        if(success==GL_FALSE){
            std::int32_t loglen;
            glGetShaderiv(handle,GL_INFO_LOG_LENGTH,&loglen);
            std::unique_ptr<char[]> str{new char[loglen]};
            glGetShaderInfoLog(handle,loglen,nullptr,str.get());
            throw std::runtime_error(str.get());
        }
    }
    void Program::link(){
        glLinkProgram(handle);
        std::int32_t success;
        glGetProgramiv(handle,GL_LINK_STATUS,&success);
        if(success==GL_FALSE){
            std::int32_t loglen;
            glGetProgramiv(handle,GL_INFO_LOG_LENGTH,&loglen);
            std::unique_ptr<char[]> str{new char[loglen]};
            glGetProgramInfoLog(handle,loglen,nullptr,str.get());
            throw std::runtime_error(str.get());
        }
    }
}
