#include"../gl.hpp"
#include<cppp/vector.hpp>
namespace sgl{
    class MonochromeRectDrawer{
        GLBuffer vbo;
        VAO vao;
        mutable Program prog;
        public:
            MonochromeRectDrawer();
            void rect(cppp::fvec2 topleft,cppp::fvec2 dims,cppp::fvec4 color) const;
    };
}
