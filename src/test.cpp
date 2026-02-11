#include<cppp/template-string.hpp>
#include<sgl/ext/freetype.hpp>
#include<sgl/ext/stbi.hpp>
#include<cppp/tostring.hpp>
#include<cppp/rtl.hpp>
#include<sgl/sgl.hpp>
#include<print>
using namespace std::literals;
#define printattr(attr) glGetIntegerv(attr,&data); if(data<1048576) std::println(#attr " = {}"sv,data); else std::println(#attr " = {:.1f}Mi"sv,static_cast<float>(data)/1048576.0f)
void gldbg(){
    using namespace std::literals;
    GLint data;
    printattr(GL_MAX_TEXTURE_SIZE);
    printattr(GL_MAX_3D_TEXTURE_SIZE);
    printattr(GL_MAX_ELEMENTS_VERTICES);
    printattr(GL_MAX_ELEMENTS_INDICES);
    printattr(GL_MAX_CUBE_MAP_TEXTURE_SIZE);
    printattr(GL_MAX_TEXTURE_LOD_BIAS);
    printattr(GL_MAX_DRAW_BUFFERS);
    printattr(GL_MAX_VERTEX_ATTRIBS);
    printattr(GL_MAX_TEXTURE_IMAGE_UNITS);
    printattr(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS);
    printattr(GL_MAX_VERTEX_UNIFORM_COMPONENTS);
    printattr(GL_MAX_VARYING_FLOATS);
    printattr(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS);
    printattr(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
    printattr(GL_MAX_CLIP_DISTANCES);
    printattr(GL_MAX_ARRAY_TEXTURE_LAYERS);
    printattr(GL_MAX_PROGRAM_TEXEL_OFFSET);
    printattr(GL_MAX_VARYING_COMPONENTS);
    printattr(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS);
    printattr(GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS);
    printattr(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS);
    printattr(GL_MAX_RENDERBUFFER_SIZE);
    printattr(GL_MAX_COLOR_ATTACHMENTS);
    printattr(GL_MAX_SAMPLES);
    printattr(GL_MAX_TEXTURE_BUFFER_SIZE);
    printattr(GL_MAX_RECTANGLE_TEXTURE_SIZE);
    printattr(GL_MAX_VERTEX_UNIFORM_BLOCKS);
    printattr(GL_MAX_GEOMETRY_UNIFORM_BLOCKS);
    printattr(GL_MAX_FRAGMENT_UNIFORM_BLOCKS);
    printattr(GL_MAX_COMBINED_UNIFORM_BLOCKS);
    printattr(GL_MAX_UNIFORM_BUFFER_BINDINGS);
    printattr(GL_MAX_UNIFORM_BLOCK_SIZE);
    printattr(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS);
    printattr(GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS);
    printattr(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS);
    printattr(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS);
    printattr(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS);
    printattr(GL_MAX_GEOMETRY_OUTPUT_VERTICES);
    printattr(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS);
    printattr(GL_MAX_VERTEX_OUTPUT_COMPONENTS);
    printattr(GL_MAX_GEOMETRY_INPUT_COMPONENTS);
    printattr(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS);
    printattr(GL_MAX_FRAGMENT_INPUT_COMPONENTS);
    printattr(GL_MAX_SERVER_WAIT_TIMEOUT);
    printattr(GL_MAX_SAMPLE_MASK_WORDS);
    printattr(GL_MAX_COLOR_TEXTURE_SAMPLES);
    printattr(GL_MAX_DEPTH_TEXTURE_SAMPLES);
    printattr(GL_MAX_INTEGER_SAMPLES);
    printattr(GL_MAX_DUAL_SOURCE_DRAW_BUFFERS);
    printattr(GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET);
    printattr(GL_MAX_GEOMETRY_SHADER_INVOCATIONS);
    printattr(GL_MAX_FRAGMENT_INTERPOLATION_OFFSET);
    printattr(GL_MAX_VERTEX_STREAMS);
    printattr(GL_MAX_SUBROUTINES);
    printattr(GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS);
    printattr(GL_MAX_PATCH_VERTICES);
    printattr(GL_MAX_TESS_GEN_LEVEL);
    printattr(GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS);
    printattr(GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS);
    printattr(GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS);
    printattr(GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS);
    printattr(GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS);
    printattr(GL_MAX_TESS_PATCH_COMPONENTS);
    printattr(GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS);
    printattr(GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS);
    printattr(GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS);
    printattr(GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS);
    printattr(GL_MAX_TESS_CONTROL_INPUT_COMPONENTS);
    printattr(GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS);
    printattr(GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS);
    printattr(GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS);
    printattr(GL_MAX_TRANSFORM_FEEDBACK_BUFFERS);
    printattr(GL_MAX_VERTEX_UNIFORM_VECTORS);
    printattr(GL_MAX_VARYING_VECTORS);
    printattr(GL_MAX_FRAGMENT_UNIFORM_VECTORS);
    printattr(GL_MAX_VIEWPORTS);
    printattr(GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS);
    printattr(GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS);
    printattr(GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS);
    printattr(GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS);
    printattr(GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS);
    printattr(GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS);
    printattr(GL_MAX_VERTEX_ATOMIC_COUNTERS);
    printattr(GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS);
    printattr(GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS);
    printattr(GL_MAX_GEOMETRY_ATOMIC_COUNTERS);
    printattr(GL_MAX_FRAGMENT_ATOMIC_COUNTERS);
    printattr(GL_MAX_COMBINED_ATOMIC_COUNTERS);
    printattr(GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE);
    printattr(GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS);
    printattr(GL_MAX_IMAGE_UNITS);
    printattr(GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS);
    printattr(GL_MAX_IMAGE_SAMPLES);
    printattr(GL_MAX_VERTEX_IMAGE_UNIFORMS);
    printattr(GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS);
    printattr(GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS);
    printattr(GL_MAX_GEOMETRY_IMAGE_UNIFORMS);
    printattr(GL_MAX_FRAGMENT_IMAGE_UNIFORMS);
    printattr(GL_MAX_COMBINED_IMAGE_UNIFORMS);
    printattr(GL_MAX_ELEMENT_INDEX);
    printattr(GL_MAX_COMPUTE_UNIFORM_BLOCKS);
    printattr(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS);
    printattr(GL_MAX_COMPUTE_IMAGE_UNIFORMS);
    printattr(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE);
    printattr(GL_MAX_COMPUTE_UNIFORM_COMPONENTS);
    printattr(GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS);
    printattr(GL_MAX_COMPUTE_ATOMIC_COUNTERS);
    printattr(GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS);
    printattr(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS);
    printattr(GL_MAX_DEBUG_MESSAGE_LENGTH);
    printattr(GL_MAX_DEBUG_LOGGED_MESSAGES);
    printattr(GL_MAX_DEBUG_GROUP_STACK_DEPTH);
    printattr(GL_MAX_LABEL_LENGTH);
    printattr(GL_MAX_UNIFORM_LOCATIONS);
    printattr(GL_MAX_FRAMEBUFFER_WIDTH);
    printattr(GL_MAX_FRAMEBUFFER_HEIGHT);
    printattr(GL_MAX_FRAMEBUFFER_LAYERS);
    printattr(GL_MAX_FRAMEBUFFER_SAMPLES);
    printattr(GL_MAX_WIDTH);
    printattr(GL_MAX_HEIGHT);
    printattr(GL_MAX_DEPTH);
    printattr(GL_MAX_LAYERS);
    printattr(GL_MAX_COMBINED_DIMENSIONS);
    printattr(GL_MAX_NAME_LENGTH);
    printattr(GL_MAX_NUM_ACTIVE_VARIABLES);
    printattr(GL_MAX_NUM_COMPATIBLE_SUBROUTINES);
    printattr(GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS);
    printattr(GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS);
    printattr(GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS);
    printattr(GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS);
    printattr(GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS);
    printattr(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS);
    printattr(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS);
    printattr(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS);
    printattr(GL_MAX_SHADER_STORAGE_BLOCK_SIZE);
    printattr(GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES);
    printattr(GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET);
    printattr(GL_MAX_VERTEX_ATTRIB_BINDINGS);
    printattr(GL_MAX_VERTEX_ATTRIB_STRIDE);
    printattr(GL_MAX_CULL_DISTANCES);
    printattr(GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES);
    printattr(GL_MAX_TEXTURE_MAX_ANISOTROPY);
}
#undef printattr
void draw_text(cppp::sv sv,cppp::fvec2 pos,cppp::fvec2 npp,sgl::CachedFont& gc,sgl::Shaper& sh,const sgl::SDFRenderer& sb){
    for(auto it = sh.shape(sv,gc.font());it;++it){
        auto& gl = gc.query(it.glyph());
        if(gl.bitmap()){
            sb.draw(gl.bitmap(),pos+cppp::fvec2(it.bearing()+gl.bearing())*npp,npp,{1.0f});
        }
        pos += npp*cppp::fvec2(it.advance())/64.0f;
    }
}
void draw_text(cppp::sv sv,cppp::fvec2 pos,cppp::fvec2 npp,sgl::CachedFont& gc,sgl::Shaper& sh,const sgl::SimpleBlitter& b){
    for(auto it = sh.shape(sv,gc.font());it;++it){
        auto& gl = gc.query(it.glyph());
        if(gl.bitmap()){
            b.blit_rel(gl.bitmap(),pos+cppp::fvec2(it.bearing()+gl.bearing())*npp,npp);
        }
        pos += npp*cppp::fvec2(it.advance())/64.0f;
    }
}
int main(){
    using namespace cppp::literals;
    SDL_SetAppMetadata("SGL Test App",nullptr,"sgl.cpp");
    SDL_InitSubSystem(SDL_INIT_VIDEO);

    constexpr std::uint32_t SW = 1200;
    constexpr std::uint32_t SH = 600;
    sgl::Window win{u8"Hello, world!"s,SW,SH};
    sgl::CoordinateMap cm{SW,SH};
    { // scope for all GL objects. Their dtors must run before we destroy everything with SDL_Quit().
    // gldbg();
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glEnable(GL_CULL_FACE);
    sgl::init_gl_for_text();
    gldbg();
    sgl::FreeType ftlib;
    sgl::CachedFont cf{ftlib.load_font_from_fc(u8"Consolas"s),sgl::SdfMode::DIRECT};
    std::uint32_t i = 13;
    float sca = 1.9f;
    cf.font().init_width_pt(i<<6uz,191,191);
    sgl::SimpleBlitter b;
    sgl::SDFRenderer sb;
    sgl::SDFRenderer sr;
    bool sdf = true;
    SDL_GL_SetSwapInterval(-1);
    cppp::str buf;
    win.start_input();
    cppp::str fn{cf.font().get_name()};
    sgl::Shaper shp;
    while(true){
        for(const auto& e : sgl::events()){
            switch(e.type){
                case SDL_EVENT_QUIT: goto cleanup;
                case SDL_EVENT_WINDOW_RESIZED:
                    glViewport(0,0,e.window.data1,e.window.data2);
                    cm.update(static_cast<std::uint32_t>(e.window.data1),static_cast<std::uint32_t>(e.window.data2));
                    break;
                case SDL_EVENT_KEY_DOWN:
                    switch(e.key.key){
                        case SDLK_UP:
                            ++i;
                            cf.font().set_width_pt(i<<6uz,191,191);
                            cf.purge();
                            break;
                        case SDLK_DOWN:
                            --i;
                            cf.font().set_width_pt(i<<6uz,191,191);
                            cf.purge();
                            break;
                        case SDLK_LEFT:
                            sca -= 0.05f;
                            break;
                        case SDLK_RIGHT:
                            sca += 0.05f;
                            break;
                        case SDLK_TAB:
                            cf.reset_sdf((sdf = !sdf)?sgl::SdfMode::DIRECT:sgl::SdfMode::NO_SDF);
                            break;
                        case SDLK_BACKSPACE:
                            if(!buf.empty()) buf.pop_back();
                            break;
                        case SDLK_RETURN:
                            cf = {buf.starts_with(u8'/')?ftlib.load_font(buf):ftlib.load_font_from_fc(buf),sdf?sgl::SdfMode::DIRECT:sgl::SdfMode::NO_SDF};
                            buf.clear();
                            cf.font().init_width_pt(i<<6uz,191,191);
                            fn = cf.font().get_name();
                            break;
                        default:;
                    }
                    break;
                case SDL_EVENT_TEXT_INPUT:
                    for(const char c : std::string_view{e.text.text}){
                        buf.push_back(static_cast<char8_t>(c));
                    }
                    break;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);
        cppp::fvec2 tpos{20.0f,static_cast<float>(cm.win_size().y()>>1)};
        cppp::fvec2 bpos{20.0f,static_cast<float>(cm.win_size().y()*3>>2)};
        if(sdf){
            draw_text(cppp::format<u8"{} {}pt x {:.2f} (SDF)"_ts>(fn,i,sca),cm.cvt_abs(tpos),cm.pixel_size()*sca,cf,shp,sb);
            draw_text(buf,cm.cvt_abs(bpos),cm.pixel_size()*sca,cf,shp,sb);
        }else{
            draw_text(cppp::format<u8"{} {}pt x {:.2f} (No SDF)"_ts>(fn,i,sca),cm.cvt_abs(tpos),cm.pixel_size()*sca,cf,shp,b);
            draw_text(buf,cm.cvt_abs(bpos),cm.pixel_size()*sca,cf,shp,b);
        }
        win.flip();
    }
    }
    cleanup:
    SDL_Quit();
    return 0;
}
