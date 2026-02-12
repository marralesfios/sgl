#pragma once
#include<cppp/string.hpp>
#include<cppp/vector.hpp>
#include<harfbuzz/hb.h>
#include<filesystem>
#include<concepts>
#include<utility>
#include<ft2build.h>
#include FT_FREETYPE_H
#include"../gl.hpp"
namespace sgl{
    void init_gl_for_text();
    namespace detail{
        template<typename T,std::invocable<T> auto const& delf>
        class resource_handle{
            void _die() noexcept{
                /*
                Don't std::terminate() when delf fails!
                
                See discussion: https://freetype.nongnu.narkive.com/U1kU3Tr3/ft-ft-done-face-should-not-return-an-error
                Archive: https://web.archive.org/web/20260115032804/https://freetype.nongnu.narkive.com/U1kU3Tr3/ft-ft-done-face-should-not-return-an-error
                and ft2 source code:
                FT_Done_FreeType (base/ftinit.c)
                FT_Done_Face (base/ftobjs.c)
                
                The dtors only fail if you try to delete a null handle.
                */
                static_cast<void>(delf(handle));
            }
            protected:
            
                T handle;
                resource_handle() = default;
                resource_handle(T h) : handle(h){}
                resource_handle(const resource_handle&) = delete;
                resource_handle(resource_handle&& other) noexcept : handle(std::exchange(other.handle,nullptr)){}
                resource_handle& operator=(const resource_handle&) = delete;
                resource_handle& operator=(resource_handle&& other) noexcept{
                    if(this != &other){
                        _die();
                        handle = std::exchange(other.handle,nullptr);
                    }
                    return *this;
                }
                ~resource_handle(){
                    _die();
                }
        };
    }
    class Glyph{
        Texture<TextureType::TWO_D> _bitmap;
        std::uint32_t _advance;
        cppp::ivec2 _displacement;
        friend class Font;
        Glyph(FT_GlyphSlot);
        public:
            Texture<TextureType::TWO_D>& bitmap(){
                return _bitmap;
            }
            const Texture<TextureType::TWO_D>& bitmap() const{
                return _bitmap;
            }
            std::uint32_t advance() const{
                return _advance;
            }
            cppp::ivec2 bearing() const{
                return _displacement;
            }
    };
    enum class SdfMode{
        NO_SDF,DIRECT,FROM_BITMAP
    };
    class Font : detail::resource_handle<FT_Face,FT_Done_Face>{
        friend class FreeType;
        friend class Shaper;
        Font(FT_Face hand);
        static float fp6itf(std::int32_t v){
            return static_cast<float>(v)/64.0f;
        }
        static float fp6itf(std::uint32_t v){
            return static_cast<float>(v)/64.0f;
        }
        public:
            void init_width_px(std::uint32_t px);
            // pt in fp.6 (i.e. 1/64 units)
            void init_width_pt(std::uint32_t pt,std::uint32_t hdpi,std::uint32_t vdpi);

            void set_width_px(std::uint32_t px);
            // pt in fp.6 (i.e. 1/64 units)
            void set_width_pt(std::uint32_t pt,std::uint32_t hdpi,std::uint32_t vdpi);

            std::u8string get_name() const{
                return cppp::tou8(std::string_view{handle->family_name});
            }
            
            // fp.6 (i.e. 1/64 units)
            std::uint32_t line_height() const{
                return static_cast<std::uint32_t>(FT_MulFix(handle->height,handle->size->metrics.y_scale));
            }
            float line_height_px() const{
                return fp6itf(line_height());
            }
            // fp.6 (i.e. 1/64 units)
            std::int32_t ascender() const{
                return static_cast<std::int32_t>(FT_MulFix(handle->ascender,handle->size->metrics.y_scale));
            }
            float ascender_px() const{
                return fp6itf(ascender());
            }
            // fp.6 (i.e. 1/64 units), usually negative!
            std::int32_t descender() const{
                return static_cast<std::int32_t>(FT_MulFix(handle->descender,handle->size->metrics.y_scale));
            }
            float descender_px() const{
                return fp6itf(descender());
            }
            std::uint32_t char_to_glyph_id(char32_t cp) const;
            Glyph load_glyph(std::uint32_t gl,SdfMode sdf=SdfMode::NO_SDF);
    };
    class FreeType : detail::resource_handle<FT_Library,FT_Done_FreeType>{
        public:
            FreeType();
            Font load_font(const char* path);
            Font load_font(std::filesystem::path path);
            Font load_font_from_fc(const cppp::str& name);
    };
    class CachedFont{
        std::unordered_map<std::uint32_t,Glyph> gv;
        Font f;
        SdfMode sdf;
        public:
            CachedFont(Font&& f,SdfMode sm=SdfMode::NO_SDF) : f(std::move(f)), sdf(sm){}
            Font& font(){
                return f;
            }
            const Font& font() const{
                return f;
            }
            Glyph& query(std::uint32_t gl);
            std::size_t size() const{
                return gv.size();
            }
            void reset_sdf(SdfMode newmode){
                sdf = newmode;
                purge();
            }
            void purge(){
                gv.clear();
            }
    };
    class Shaper : detail::resource_handle<hb_buffer_t*,hb_buffer_destroy>{
        public:
            /*
            not a standard C++ iterator!
            Usage:
            for(auto it = shaper.shape(font);it;++it){
                ...
            }
            */
            class iterator{
                hb_buffer_t* buf;
                hb_glyph_info_t* info;
                hb_glyph_info_t* end;
                hb_glyph_position_t* pos;
                friend Shaper;
                iterator(hb_buffer_t* b,hb_glyph_info_t* i,hb_glyph_info_t* e,hb_glyph_position_t* p) : buf(b), info(i), end(e), pos(p){}
                public:
                    iterator(const iterator&) = delete;
                    iterator(iterator&&) = delete;
                    iterator& operator=(const iterator&) = delete;
                    iterator& operator=(iterator&&) = delete;
                    std::uint32_t glyph() const{
                        return info->codepoint;
                    }
                    cppp::ivec2 bearing() const{
                        return {pos->x_offset,pos->y_offset};
                    }
                    cppp::ivec2 advance() const{
                        return {pos->x_advance,pos->y_advance};
                    }
                    iterator& operator++(){
                        ++info;
                        ++pos;
                        return *this;
                    }
                    iterator operator++(int) = delete;
                    explicit operator bool() const{
                        return info!=end;
                    }
                    ~iterator(){
                        hb_buffer_reset(buf);
                    }
            };
            Shaper() : resource_handle(hb_buffer_create()){}
            iterator shape(cppp::sv str,const Font& f);
    };
}
