#include <span>
#include <string>

namespace IE {

    template <typename T>
    struct GL_type_trait {};
    template <>
    struct GL_type_trait<float> {
        static constexpr uint32_t enum_value = 0x1406;
    };
    template <>
    struct GL_type_trait<uint32_t> {
        static constexpr uint32_t enum_value = 0x1405;
    };

    template <typename T>
    constexpr uint32_t GL_enum_value = GL_type_trait<T>::enum_value;

    // Base class of all OpenGL objects.
    struct GLObject { uint32_t id = 0; };

    template <uint32_t BufferType, typename ElemT>
    struct BufferObject : public GLObject {
        enum { StaticDraw = 0x88e4 };
        using value_type = ElemT;

        BufferObject(std::span<ElemT> dt, uint32_t usage);
        void Bind(bool bState);
        ~BufferObject();
    };

    template <uint32_t ShaderType>
    struct ShaderObject : public GLObject {
        ShaderObject(const std::string& buf);
        ~ShaderObject();
    };

    struct VtxArray : public GLObject {
        VtxArray();

        template <typename T>
        void PushAttribPointer(uint32_t dim, bool bnorm, uint32_t stride, size_t offset);

        void PopAttribPointer ();
        void Bind             (bool bState);

        ~VtxArray();
    private:
        uint32_t mIdx;
    };

    template void VtxArray::PushAttribPointer<float>(uint32_t dim, bool bnorm, uint32_t stride, size_t offset);


    struct ShaderProgram : public GLObject {
        ShaderProgram(std::initializer_list<uint32_t> ids);

        void Bind(bool bState);
        ~ShaderProgram();
    };

    template <uint32_t Dimension>
    struct TextureObject : public GLObject {
        TextureObject();
    };

    // Using make things easier.
    using VtxBuffer = BufferObject<0x8892, float>;
    using IdxBuffer = BufferObject<0x8893, uint32_t>;
    using VtxShader = ShaderObject<0x8b31>;
    using PixShader = ShaderObject<0x8b30>;
}
