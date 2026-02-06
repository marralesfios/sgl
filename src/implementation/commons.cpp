#include<sgl/commons.hpp>
#include<SDL3/SDL.h>
#include<stdexcept>
namespace sgl{
    namespace detail{
        void error(){
            throw std::runtime_error(SDL_GetError());
        }
    }
}
