#pragma once
#include"commons.hpp"
#include<cppp/string.hpp>
#include<SDL3/SDL.h>
#include<memory>
namespace sgl{
    struct s_delete_win{
        static void operator()(SDL_Window* win){
            SDL_DestroyWindow(win);
        }
    };
    // Thread safety: main thread
    class Window{
        std::unique_ptr<SDL_Window,s_delete_win> p;
        SDL_GLContext ctx;
        public:
            Window(const cppp::str& title,std::uint32_t w,std::uint32_t h);
            void flip(){
                if(!SDL_GL_SwapWindow(p.get())) detail::error();
            }
            void start_input(){
                if(!SDL_StartTextInput(p.get())) detail::error();
            }
            void set_fullscreen(bool fs){
                if(!SDL_SetWindowFullscreen(p.get(),fs)) detail::error();
            }
    };
}
