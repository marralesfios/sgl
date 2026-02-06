#include<sgl/window.hpp>
#include<sgl/commons.hpp>
#include<glad/glad.h>
namespace sgl{
    static SDL_Window* create_win(const cppp::str& title,std::uint32_t w,std::uint32_t h){
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,6);
        return SDL_CreateWindow(reinterpret_cast<const char*>(title.c_str()),static_cast<int>(w),static_cast<int>(h),SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    }
    Window::Window(const cppp::str& title,std::uint32_t w,std::uint32_t h) : p(create_win(title,w,h)){
        if(!p) detail::error();
        ctx = SDL_GL_CreateContext(p.get());
        
        if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress))) throw std::runtime_error("glad can't load OpenGL!");
    }
}
