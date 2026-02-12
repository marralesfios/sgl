#include<sgl/ext/freetype.hpp>
#include<harfbuzz/hb-ft.h>
#include<fontconfig/fontconfig.h>
#include<stdexcept>
#include<format>
#include<memory>
namespace sgl{
    using namespace std::literals;
    void init_gl_for_text(){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    }
    struct destroy_fcpat{
        void operator()(FcPattern* pat) const noexcept{
            FcPatternDestroy(pat);
        }
    };
    Glyph::Glyph(FT_GlyphSlot slt) : _bitmap(nullptr), _advance(static_cast<std::uint32_t>(slt->metrics.horiAdvance)), _displacement(slt->bitmap_left,slt->bitmap_top){
        std::uint32_t w = slt->bitmap.width;
        std::uint32_t h = slt->bitmap.rows;
        if(w&&h){
            _bitmap.create();
            _bitmap.allocate_static(sgl::ImageFormat::R8,w,h);
            _bitmap.upload(slt->bitmap.buffer,PixelFormat::R,0,0,w,h);
        }
    }
    std::uint32_t Font::char_to_glyph_id(char32_t cp) const{
        return FT_Get_Char_Index(handle,static_cast<std::uint32_t>(cp));
    }
    Glyph Font::load_glyph(std::uint32_t gl,SdfMode sdf){
        if(int errc=FT_Load_Glyph(handle,gl,FT_LOAD_PEDANTIC|FT_LOAD_NO_AUTOHINT)){
            throw std::runtime_error(std::format("FT_Load_Char failed with {}"sv,errc));
        }
        if(sdf!=SdfMode::DIRECT){
            if(int errc = FT_Render_Glyph(handle->glyph,FT_RENDER_MODE_NORMAL)){
                throw std::runtime_error(std::format("FT_Render_Glyph to bitmap failed with {}"sv,errc));
            }
        }
        if(sdf!=SdfMode::NO_SDF){
            if(int errc = FT_Render_Glyph(handle->glyph,FT_RENDER_MODE_SDF)){
                throw std::runtime_error(std::format("FT_Render_Glyph to SDF failed with {}"sv,errc));
            }
        }
        return {handle->glyph};
    }
    FreeType::FreeType(){
        if(int errc=FT_Init_FreeType(&handle)){
            throw std::runtime_error(std::format("FT_Init_FreeType failed with {}"sv,errc));
        }
    }
    static void _kill_font(void* p){
        hb_font_destroy(static_cast<hb_font_t*>(static_cast<FT_Face>(p)->generic.data));
    }
    Font::Font(FT_Face hand) : resource_handle(hand){}
    void Font::init_width_px(std::uint32_t px){
        FT_Set_Pixel_Sizes(handle,px,0);
        handle->generic.data = hb_ft_font_create(handle,nullptr);
        handle->generic.finalizer = &_kill_font;
    }
    void Font::init_width_pt(std::uint32_t pt,std::uint32_t hdpi,std::uint32_t vdpi){
        FT_Set_Char_Size(handle,pt,0,hdpi,vdpi);
        handle->generic.data = hb_ft_font_create(handle,nullptr);
        handle->generic.finalizer = &_kill_font;
    }
    void Font::set_width_px(std::uint32_t px){
        FT_Set_Pixel_Sizes(handle,px,0);
        hb_ft_font_changed(static_cast<hb_font_t*>(handle->generic.data));
    }
    void Font::set_width_pt(std::uint32_t pt,std::uint32_t hdpi,std::uint32_t vdpi){
        FT_Set_Char_Size(handle,pt,0,hdpi,vdpi);
        hb_ft_font_changed(static_cast<hb_font_t*>(handle->generic.data));
    }
    Font FreeType::load_font(std::filesystem::path path){
        return load_font(path.c_str());
    }
    Font FreeType::load_font(const char* path){
        FT_Face face;
        if(int errc=FT_New_Face(handle,path,0,&face)){
            throw std::runtime_error(std::format("FT_New_Face failed with {}"sv,errc));
        }
        return face;
    }
    Font FreeType::load_font_from_fc(const cppp::str& name){
        FcResult result;
        std::unique_ptr<FcPattern,destroy_fcpat> pat{FcNameParse(reinterpret_cast<const FcChar8*>(name.c_str()))};
        FcPatternAddInteger(pat.get(),FC_WEIGHT,FC_WEIGHT_REGULAR);
        FcPatternAddInteger(pat.get(),FC_SLANT,FC_SLANT_ROMAN);
        if(std::unique_ptr<FcPattern,destroy_fcpat> font{FcFontMatch(nullptr,pat.get(),&result)}){
            if(result != FcResultMatch) throw std::runtime_error("Can't find font.");
            FcChar8* file;
            if(FcPatternGetString(font.get(),FC_FILE,0,&file) != FcResultMatch){
                throw std::runtime_error("Can't find font file.");
            }
            return load_font(reinterpret_cast<const char*>(file));
        }else{
            throw std::runtime_error("Can't parse font name.");
        }
    }
    Glyph& CachedFont::query(std::uint32_t gl) const{
        if(auto it=gv.find(gl);it!=gv.end()){
            return it->second;
        }
        Glyph& ret = gv.try_emplace(gl,f.load_glyph(gl,sdf)).first->second;
        if(ret.bitmap()){
            ret.bitmap().set_downscale_interpolation(sdf!=SdfMode::NO_SDF);
            ret.bitmap().set_upscale_interpolation(sdf!=SdfMode::NO_SDF);
            ret.bitmap().wrapx(TextureWrapMode::CLAMP_TO_EDGE);
            ret.bitmap().wrapy(TextureWrapMode::CLAMP_TO_EDGE);
            if(sdf==SdfMode::NO_SDF){
                ret.bitmap().swizzle(sgl::TextureSwizzleTarget::R,sgl::TextureSwizzleSource::ONE);
                ret.bitmap().swizzle(sgl::TextureSwizzleTarget::G,sgl::TextureSwizzleSource::ONE);
                ret.bitmap().swizzle(sgl::TextureSwizzleTarget::B,sgl::TextureSwizzleSource::ONE);
                ret.bitmap().swizzle(sgl::TextureSwizzleTarget::A,sgl::TextureSwizzleSource::R);
            }
        }
        return ret;
    }
    Shaper::iterator Shaper::shape(cppp::sv str,const Font& f){
        hb_buffer_add_utf8(handle,reinterpret_cast<const char*>(str.data()),static_cast<int>(str.size()),0,-1);
        hb_buffer_guess_segment_properties(handle);
        hb_shape(reinterpret_cast<hb_font_t*>(f.handle->generic.data),handle,nullptr,0);
        std::uint32_t n;
        hb_glyph_info_t* i = hb_buffer_get_glyph_infos(handle,&n);
        hb_glyph_position_t* p = hb_buffer_get_glyph_positions(handle,&n);
        return {handle,i,i+n,p};
    }
}
