#pragma once
#include"commons.hpp"
#include<cppp/string.hpp>
#include<cppp/strmap.hpp>
#include<cppp/vector.hpp>
#include<type_traits>
#include<concepts>
#include<stdfloat>
#include<cstdint>
#include<utility>
#include<limits>
#include<memory>
#include<span>
namespace sgl{
    using gaddr_t = std::uint64_t;
    using gsize_t = std::uint64_t;
    using gsaddr_t = std::make_signed_t<gaddr_t>;
    using gssize_t = std::make_signed_t<gsize_t>;
    // smaller of the CPU/GPU sizes, since transferring is bound by both
    using tsize_t = std::conditional_t<sizeof(std::size_t)<sizeof(gsize_t),std::size_t,gsize_t>;
    using tssize_t = std::make_signed_t<tsize_t>;
    namespace detail{
        template<typename T,typename U>
        concept layouts_seem_identical = (sizeof(T)==sizeof(U))&&(alignof(T)==alignof(U))&&(std::is_signed_v<T> == std::is_signed_v<U>);
        static_assert(layouts_seem_identical<GLuint,std::uint32_t>
                    &&layouts_seem_identical<GLbitfield,std::uint32_t>
                    &&layouts_seem_identical<GLenum,std::uint32_t>
                    &&layouts_seem_identical<GLsizeiptr,gssize_t>
                    &&layouts_seem_identical<GLintptr,gsaddr_t>
                    &&layouts_seem_identical<GLint,std::int32_t>
                    &&std::same_as<GLfloat,float>
                    &&std::numeric_limits<GLfloat>::is_iec559,"Bad bad bad bad bad");
        template<void(*&delet)(GLsizei,const std::uint32_t*)>
        class GLObject{
            protected:
                std::uint32_t handle;
                GLObject() = default;
                GLObject(std::uint32_t h) : handle(h){}
            public:
                GLObject(const GLObject&) = delete;
                GLObject(GLObject&& other) noexcept : handle(std::exchange(other.handle,0)){}
                GLObject& operator=(const GLObject&) = delete;
                GLObject& operator=(GLObject&& other) noexcept{
                    if(this!=&other){
                        delet(1,&handle);
                        handle = std::exchange(other.handle,0);
                    }
                    return *this;
                }
                ~GLObject(){
                    delet(1,&handle);
                }
        };
    }
    enum class ImageFormat : std::uint32_t{
        R8=GL_R8,
        R32UI=GL_R32UI,
        RGB8=GL_RGB8,
        RGBA8=GL_RGBA8,
    };
    enum class PixelFormat : std::uint32_t{
        R=GL_RED,
        RGB=GL_RGB,
        RGBA=GL_RGBA,
        BGR=GL_BGR,
        BGRA=GL_BGRA
    };
    template<typename T>
    concept GLDataType = std::same_as<T,std::uint8_t>
                      || std::same_as<T,std::uint16_t>
                      || std::same_as<T,std::uint32_t>
                      || std::same_as<T,std::float16_t>
                      || std::same_as<T,float>
                      || std::same_as<T,double>;
    enum class GLDataTypeEnum : std::uint32_t{
        UINT8=GL_UNSIGNED_BYTE,
        UINT16=GL_UNSIGNED_SHORT,
        UINT32=GL_UNSIGNED_INT,
        FLOAT16=GL_HALF_FLOAT,
        FLOAT32=GL_FLOAT,
        FLOAT64=GL_DOUBLE
    };
    template<GLDataType T>
    constexpr GLDataTypeEnum enum_from_data_type = std::same_as<T,std::uint8_t>?GLDataTypeEnum::UINT8
                                                  :std::same_as<T,std::uint16_t>?GLDataTypeEnum::UINT16
                                                  :std::same_as<T,std::uint32_t>?GLDataTypeEnum::UINT32
                                                  :std::same_as<T,std::float16_t>?GLDataTypeEnum::FLOAT16
                                                  :std::same_as<T,float>?GLDataTypeEnum::FLOAT32
                                                  :std::same_as<T,double>?GLDataTypeEnum::FLOAT64
                                                  :GLDataTypeEnum{0};
    class GLBuffer : public detail::GLObject<glDeleteBuffers>{
        friend class VAO;
        public:
            enum class StaticConfig : std::uint32_t{
                MMAP_R=GL_MAP_READ_BIT,MMAP_W=GL_MAP_WRITE_BIT,MMAP_USE=GL_DYNAMIC_STORAGE_BIT,MMAP_SYNC,UPLOAD,CPUBUF
            };
            GLBuffer(){
                glCreateBuffers(1,&handle);
            }
            void allocate_static(gsize_t n,StaticConfig perms,const void* data=nullptr){
                glNamedBufferStorage(handle,GLsizeiptr(n),data,std::to_underlying(perms));
            }
            enum class WriteFrequency : std::uint32_t{
                STREAM=0x88E0,STATIC=0x88E4,DYNAMIC=0x88E8
            };
            enum class AccessMethod : std::uint32_t{
                UPLOAD=0b00,DOWNLOAD=0b01,NONE=0b10
            };
            void allocate_dynamic(gsize_t n,WriteFrequency f,AccessMethod m,const void* data=nullptr){
                glNamedBufferData(handle,GLsizeiptr(n),data,std::to_underlying(f)|std::to_underlying(m));
            }
            void upload(const void* data,tsize_t n,gaddr_t offset=0){
                glNamedBufferSubData(handle,GLintptr(offset),GLsizeiptr(n),data);
            }
            template<GLDataType Dt>
            void pixel_fill(ImageFormat internal_format,PixelFormat upload_format,const Dt* pixel){
                glClearNamedBufferData(handle,std::to_underlying(internal_format),std::to_underlying(upload_format),std::to_underlying(enum_from_data_type<Dt>),pixel);
            }
            template<GLDataType Dt>
            void pixel_fill(ImageFormat internal_format,PixelFormat upload_format,gaddr_t offset,gsize_t n,const Dt* pixel){
                glClearNamedBufferSubData(handle,std::to_underlying(internal_format),std::to_underlying(upload_format),GLintptr(offset),GLsizeiptr(n),std::to_underlying(enum_from_data_type<Dt>),pixel);
            }
            void copy_to(gaddr_t offset,gsize_t n,GLBuffer& other,gaddr_t dst_offset) const{
                glCopyNamedBufferSubData(handle,other.handle,GLintptr(offset),GLintptr(dst_offset),GLsizeiptr(n));
            }
            void uninitialize(){
                glInvalidateBufferData(handle);
            }
            void uninitialize(gaddr_t offset,gsize_t n){
                glInvalidateBufferSubData(handle,GLintptr(offset),GLsizeiptr(n));
            }
    };
    class VAO : public detail::GLObject<glDeleteVertexArrays>{
        public:
            VAO(){
                glCreateVertexArrays(1,&handle);
            }
            void use() const{
                glBindVertexArray(handle);
            }
            void add_buffer(const GLBuffer& vbo,std::uint32_t i,gaddr_t offset,std::uint32_t stride){
                glVertexArrayVertexBuffer(handle,i,vbo.handle,GLintptr(offset),GLsizei(stride));
            }
            void set_attr_f(std::uint32_t i,std::uint32_t buf,std::uint32_t size,GLDataTypeEnum type,bool normalize,std::uint32_t begin){
                glEnableVertexArrayAttrib(handle,i);
                glVertexArrayAttribBinding(handle,i,buf);
                glVertexArrayAttribFormat(handle,i,GLint(size),std::to_underlying(type),normalize,begin);
            }
            void set_attr_d(std::uint32_t i,std::uint32_t buf,std::uint32_t size,GLDataTypeEnum type,std::uint32_t begin){
                glEnableVertexArrayAttrib(handle,i);
                glVertexArrayAttribBinding(handle,i,buf);
                glVertexArrayAttribLFormat(handle,i,GLint(size),std::to_underlying(type),begin);
            }
            void set_attr_i(std::uint32_t i,std::uint32_t buf,std::uint32_t size,GLDataTypeEnum type,std::uint32_t begin){
                glEnableVertexArrayAttrib(handle,i);
                glVertexArrayAttribBinding(handle,i,buf);
                glVertexArrayAttribIFormat(handle,i,GLint(size),std::to_underlying(type),begin);
            }
            template<GLDataType Dt> requires(std::same_as<Dt,std::float16_t>||std::same_as<Dt,float>)
            void set_attr(std::uint32_t i,std::uint32_t buf,std::uint32_t size,std::uint32_t begin,bool normalize){
                set_attr_f(i,buf,size,enum_from_data_type<Dt>,normalize,begin);
            }
            template<GLDataType Dt> requires(!std::same_as<Dt,std::float16_t>&&!std::same_as<Dt,float>)
            void set_attr(std::uint32_t i,std::uint32_t buf,std::uint32_t size,std::uint32_t begin){
                if constexpr(std::same_as<Dt,double>){
                    set_attr_d(i,buf,size,enum_from_data_type<Dt>,begin);
                }else{
                    set_attr_i(i,buf,size,enum_from_data_type<Dt>,begin);
                }
            }
    };
    class Shader{
        std::uint32_t handle;
        friend class Program;
        public:
            enum class Type : std::uint32_t{
                VERTEX=GL_VERTEX_SHADER,FRAGMENT=GL_FRAGMENT_SHADER
            };
            Shader(Type t) : handle(glCreateShader(std::to_underlying(t))){}
            Shader(const Shader&) = delete;
            Shader(Shader&& other) : handle(std::exchange(other.handle,0)){}
            Shader& operator=(const Shader&) = delete;
            Shader& operator=(Shader&& other){
                if(this!=&other){
                    glDeleteShader(std::exchange(handle,std::exchange(other.handle,0)));
                }
                return *this;
            }
            void load(cppp::sv s);
            ~Shader(){
                glDeleteShader(handle);                
            }
    };
    enum class TextureType : std::uint32_t{
        ONE_D=GL_TEXTURE_1D,
        TWO_D=GL_TEXTURE_2D,
        THREE_D=GL_TEXTURE_3D,
    };
    enum class TextureSwizzleSource : std::uint32_t{
        R=GL_RED,
        G=GL_GREEN,
        B=GL_BLUE,
        A=GL_ALPHA,
        ZERO=GL_ZERO,
        ONE=GL_ONE
    };
    enum class TextureSwizzleTarget : std::uint32_t{
        R=GL_TEXTURE_SWIZZLE_R,
        G=GL_TEXTURE_SWIZZLE_G,
        B=GL_TEXTURE_SWIZZLE_B,
        A=GL_TEXTURE_SWIZZLE_A
    };
    enum class TextureWrapMode : std::uint32_t{
        REPEAT=GL_REPEAT,
        MIRRORED_REPEAT=GL_MIRRORED_REPEAT,
        CLAMP_TO_EDGE=GL_CLAMP_TO_EDGE,
        CLAMP_TO_BORDER=GL_CLAMP_TO_BORDER
    };
    template<TextureType type>
    class Texture : public detail::GLObject<glDeleteTextures>{
        friend class BindlessTextureHandle;
        using size_mem_t = std::conditional_t<type==TextureType::ONE_D,std::uint32_t,
                           std::conditional_t<type==TextureType::TWO_D,cppp::uvec2,
                           std::conditional_t<type==TextureType::THREE_D,cppp::uvec3,
                           void>>>;
        size_mem_t _size;
        template<GLDataType Dt>
        void _pixel_fill_sub(PixelFormat format,std::uint32_t x,std::uint32_t y,std::uint32_t z,std::uint32_t w,std::uint32_t h,std::uint32_t d,const Dt* pixel,std::uint32_t mip){
            glClearTexSubImage(handle,GLint(mip),GLint(x),GLint(y),GLint(z),GLsizei(w),GLsizei(h),GLsizei(d),std::to_underlying(format),std::to_underlying(enum_from_data_type<Dt>),pixel);
        }
        void _uninitialize_sub(std::uint32_t x,std::uint32_t y,std::uint32_t z,std::uint32_t w,std::uint32_t h,std::uint32_t d,std::uint32_t mip){
            glInvalidateTexSubImage(handle,mip,x,y,z,w,h,d);
        }
        public:
            Texture(std::nullptr_t) : GLObject(0){}
            Texture(){
                create();
            }
            // You only need this if you explicitly constructed the texture to be null!
            void create(){
                glCreateTextures(std::to_underlying(type),1,&handle);
            }
            explicit operator bool() const{
                return handle;
            }
            size_mem_t size() const{
                return _size;
            }
            void wrapx(TextureWrapMode wm){
                glTextureParameteri(handle,GL_TEXTURE_WRAP_S,GLint(std::to_underlying(wm)));
            }
            void wrapy(TextureWrapMode wm) requires(type!=TextureType::ONE_D){
                glTextureParameteri(handle,GL_TEXTURE_WRAP_T,GLint(std::to_underlying(wm)));
            }
            void set_upscale_interpolation(bool do_interpolate){
                glTextureParameteri(handle,GL_TEXTURE_MAG_FILTER,do_interpolate?GL_LINEAR:GL_NEAREST);
            }
            void set_downscale_interpolation(bool do_interpolate){
                glTextureParameteri(handle,GL_TEXTURE_MIN_FILTER,do_interpolate?GL_LINEAR:GL_NEAREST);
            }
            void allocate_static(ImageFormat fmt,std::uint32_t width) requires(type==TextureType::ONE_D){
                _size = width;
                glTextureStorage1D(handle,1,std::to_underlying(fmt),GLsizei(width));
            }
            void allocate_static(ImageFormat fmt,std::uint32_t width,std::uint32_t height) requires(type==TextureType::TWO_D){
                _size = cppp::uvec2(width,height);
                glTextureStorage2D(handle,1,std::to_underlying(fmt),GLsizei(width),GLsizei(height));
            }
            void allocate_static(ImageFormat fmt,std::uint32_t width,std::uint32_t height,std::uint32_t depth) requires(type==TextureType::THREE_D){
                _size = cppp::uvec3(width,height,depth);
                glTextureStorage3D(handle,1,std::to_underlying(fmt),GLsizei(width),GLsizei(height),GLsizei(depth));
            }
            void swizzle(TextureSwizzleTarget to,TextureSwizzleSource from){
                glTextureParameteri(handle,std::to_underlying(to),GLint(std::to_underlying(from)));
            }
            void bind() const{
                glBindTexture(std::to_underlying(type),handle);
            }
            template<GLDataType Dt>
            void upload(const Dt* data,PixelFormat pf,std::uint32_t x,std::uint32_t w,std::uint32_t mip=0) requires(type==TextureType::ONE_D){
                glTextureSubImage1D(handle,GLint(mip),GLint(x),GLint(w),std::to_underlying(pf),std::to_underlying(enum_from_data_type<Dt>),data);
            }
            template<GLDataType Dt>
            void upload(const Dt* data,PixelFormat pf,std::uint32_t x,std::uint32_t y,std::uint32_t w,std::uint32_t h,std::uint32_t mip=0) requires(type==TextureType::TWO_D){
                glTextureSubImage2D(handle,GLint(mip),GLint(x),GLint(y),GLsizei(w),GLsizei(h),std::to_underlying(pf),std::to_underlying(enum_from_data_type<Dt>),data);
            }
            template<GLDataType Dt>
            void upload(const Dt* data,PixelFormat pf,std::uint32_t x,std::uint32_t y,std::uint32_t z,std::uint32_t w,std::uint32_t h,std::uint32_t d,std::uint32_t mip=0) requires(type==TextureType::THREE_D){
                glTextureSubImage3D(handle,GLint(mip),GLint(x),GLint(y),GLint(z),GLsizei(w),GLsizei(h),GLsizei(d),std::to_underlying(pf),std::to_underlying(enum_from_data_type<Dt>),data);
            }
            template<GLDataType Dt>
            void download(std::span<Dt> data,PixelFormat pf,std::uint32_t mip=0){
                glGetTextureImage(handle,mip,std::to_underlying(pf),std::to_underlying(enum_from_data_type<Dt>),GLsizei(data.size()),data.data());
            }
            template<GLDataType Dt>
            void pixel_fill(PixelFormat format,const Dt* pixel,std::uint32_t mip=0){
                glClearTexImage(handle,GLint(mip),std::to_underlying(format),std::to_underlying(enum_from_data_type<Dt>),pixel);
            }
            template<GLDataType Dt> requires(type==TextureType::ONE_D)
            void pixel_fill(PixelFormat format,std::uint32_t x,std::uint32_t w,const Dt* pixel,std::uint32_t mip=0){
                _pixel_fill_sub(format,x,0,0,w,1,1,pixel,mip);
            }
            template<GLDataType Dt> requires(type==TextureType::TWO_D)
            void pixel_fill(PixelFormat format,std::uint32_t x,std::uint32_t y,std::uint32_t w,std::uint32_t h,const Dt* pixel,std::uint32_t mip=0){
                _pixel_fill_sub(format,x,y,0,w,h,1,pixel,mip);
            }
            template<GLDataType Dt> requires(type==TextureType::THREE_D)
            void pixel_fill(PixelFormat format,std::uint32_t x,std::uint32_t y,std::uint32_t z,std::uint32_t w,std::uint32_t h,std::uint32_t d,const Dt* pixel,std::uint32_t mip=0){
                _pixel_fill_sub(format,x,y,z,w,h,d,pixel,mip);
            }
            void uninitialize(std::uint32_t mip=0){
                glInvalidateTexImage(handle,mip);
            }
            void uninitialize(std::uint32_t x,std::uint32_t w,std::uint32_t mip=0) requires(type==TextureType::ONE_D){
                _uninitialize_sub(x,0,0,w,1,1,mip);
            }
            void uninitialize(std::uint32_t x,std::uint32_t y,std::uint32_t w,std::uint32_t h,std::uint32_t mip=0) requires(type==TextureType::TWO_D){
                _uninitialize_sub(x,y,0,w,h,1,mip);
            }
            void uninitialize(std::uint32_t x,std::uint32_t y,std::uint32_t z,std::uint32_t w,std::uint32_t h,std::uint32_t d,std::uint32_t mip=0) requires(type==TextureType::THREE_D){
                _uninitialize_sub(x,y,z,w,h,d,mip);
            }
    };
    class BindlessTextureHandle{
        friend class Program;
        std::uint64_t handle;
        public:
            BindlessTextureHandle(){
                if(!GLAD_GL_ARB_bindless_texture) throw std::runtime_error("GL_ARB_bindless_texture is not supported.");
            }
            template<TextureType type>
            explicit BindlessTextureHandle(const Texture<type>& tfrom) : handle(glGetTextureHandleARB(tfrom.handle)){}
            void enable(){
                glMakeTextureHandleResidentARB(handle);
            }
            void disable(){
                glMakeTextureHandleNonResidentARB(handle);
            }
    };
    class Program{
        std::uint32_t handle;
        public:
            Program() : handle(glCreateProgram()){}
            Program(cppp::sv vss,cppp::sv fss) : Program(){
                Shader vs{Shader::Type::VERTEX};
                vs.load(vss);
                add(vs);
                Shader fs{Shader::Type::FRAGMENT};
                fs.load(fss);
                add(fs);
                link();
            }
            Program(const Program&) = delete;
            Program(Program&& other) noexcept : handle(std::exchange(other.handle,0)){}
            Program& operator=(const Program&) = delete;
            Program& operator=(Program&& other) noexcept{
                if(this!=&other){
                    glDeleteProgram(std::exchange(handle,std::exchange(other.handle,0)));
                }
                return *this;
            }
            void add(const Shader& sh){
                glAttachShader(handle,sh.handle);
            }
            void link();
            void use() const{
                glUseProgram(handle);
            }
            std::uint32_t uniform_id(const cppp::str& name) const{
                return static_cast<std::uint32_t>(glGetUniformLocation(handle,reinterpret_cast<const char*>(name.c_str())));
            }
            void set_uniform(std::uint32_t uniform,float flt){
                glProgramUniform1f(handle,GLint(uniform),flt);
            }
            void set_uniform(std::uint32_t uniform,cppp::fvec2 fv){
                glProgramUniform2fv(handle,GLint(uniform),1,fv.data());
            }
            void set_uniform(std::uint32_t uniform,cppp::fvec3 fv){
                glProgramUniform3fv(handle,GLint(uniform),1,fv.data());
            }
            void set_uniform(std::uint32_t uniform,cppp::fvec4 fv){
                glProgramUniform4fv(handle,GLint(uniform),1,fv.data());
            }
            void set_uniform(std::uint32_t uniform,std::uint32_t ui){
                glProgramUniform1ui(handle,GLint(uniform),ui);
            }
            void set_uniform(std::uint32_t uniform,const BindlessTextureHandle& tex){
                glProgramUniformHandleui64ARB(handle,GLint(uniform),tex.handle);
            }
            ~Program(){
                glDeleteProgram(handle);                
            }
    };
    #define SGL_BITFIELD_ENUM(n) inline n operator|(n lhs,n rhs){ return n{std::to_underlying(lhs)|std::to_underlying(rhs)}; }
    SGL_BITFIELD_ENUM(GLBuffer::StaticConfig);
    #undef SGL_BITFIELD_ENUM
    
    class CoordinateMap{
        cppp::uvec2 _win_size;
        cppp::fvec2 conversion_factor;
        // space isn't a concern because we won't have tons of instances, so we aim to speed up queries by storing the y abs too
        float pixel_size_y;
        constexpr static cppp::fvec2 NDC_ORIGIN{-1.0f,1.0f};
        public:
            CoordinateMap(std::uint32_t w,std::uint32_t h)
            : _win_size(w,h), conversion_factor(2.0f/static_cast<float>(w),-2.0f/static_cast<float>(h)),
              pixel_size_y(2.0f/static_cast<float>(h)){}
            void update(std::uint32_t w,std::uint32_t h){
                conversion_factor.x() = 2.0f/static_cast<float>(_win_size.x() = w);
                conversion_factor.y() = -(pixel_size_y = 2.0f/static_cast<float>(_win_size.y() = h));
            }
            cppp::uvec2 win_size() const{
                return _win_size;
            }
            cppp::fvec2 cvt_rel(cppp::fvec2 pos) const{
                return pos*conversion_factor;
            }
            cppp::fvec2 cvt_abs(cppp::fvec2 pos) const{
                return NDC_ORIGIN + cvt_rel(pos);
            }
            cppp::fvec2 pixel_size() const{
                return {conversion_factor.x(),pixel_size_y};
            }
            cppp::ivec2 from_ndc_rel(cppp::fvec2 ndc) const{
                return cppp::ivec2(ndc/conversion_factor);
            }
            cppp::ivec2 from_ndc_abs(cppp::fvec2 ndc) const{
                return from_ndc_rel(ndc-NDC_ORIGIN);
            }
    };
}
