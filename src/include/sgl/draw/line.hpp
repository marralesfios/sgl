#include"../gl.hpp"
#include<cppp/vector.hpp>
namespace sgl{
    class LineDrawer{
        GLBuffer vbo;
        VAO vao;
        Program prog;
        public:
            LineDrawer();
            void line(cppp::fvec2 start,cppp::fvec3 startc,cppp::fvec2 end,cppp::fvec3 endc);
    };
}
