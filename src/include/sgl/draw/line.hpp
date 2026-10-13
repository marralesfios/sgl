#include"../gl.hpp"
#include<cppp/vector.hpp>
namespace sgl{
    class LineDrawer{
        mutable GLBuffer vbo;
        VAO vao;
        Program prog;
        public:
            LineDrawer();
            void line(cppp::fvec2 start,cppp::fvec4 startc,cppp::fvec2 end,cppp::fvec4 endc) const;
    };
}
