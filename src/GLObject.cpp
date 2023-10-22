#include <source_location>

#include <GLObject.hpp>
#include <glad/glad.h>

#include <LogLogger.hpp>
#include <LogColor.hpp>

#define THROW(STR)      {                                                                             \
auto l = std::source_location::current();                                                             \
throw std::runtime_error(std::format(STR##"|{}({},{})",l.file_name(), l.line(), l.column()).c_str());}
#ifdef _DEBUG
#define GL_CALL(STATE)  do {                                                                                 \
STATE; auto v = glGetError();                                                                                \
auto l = std::source_location::current();                                                                    \
if (v != GL_NO_ERROR) {                                                                                      \
    IE::Util::lConsole.Error("GL_ERROR: {:d}|{}({},{})", (int)v, l.file_name(), l.line(), l.column());       \
}                                                                                                            \
} while(false)
#else  
#define GL_CALL(STATE) do {STATE;} while(false)
#endif


namespace IE {

    template class BufferObject<GL_ARRAY_BUFFER, float>;
    template class BufferObject<GL_ELEMENT_ARRAY_BUFFER, uint32_t>;
    template class ShaderObject<GL_VERTEX_SHADER>;
    template class ShaderObject<GL_FRAGMENT_SHADER>;

    template <uint32_t BT, typename T>
    BufferObject<BT, T>::BufferObject(std::span<T> t, uint32_t usage) {
        GL_CALL(glGenBuffers(1, &id));
        GL_CALL(glBindBuffer(BT, id));
        GL_CALL(glBufferData(BT, t.size_bytes(), t.data(), usage));
    }
    template <uint32_t BT, typename T>
    void BufferObject<BT, T>::Bind(bool bState) {
        uint32_t bid = bState == true ? id : 0;
        GL_CALL(glBindBuffer(BT, bid));
    }
    template <uint32_t BT, typename T>
    BufferObject<BT, T>::~BufferObject() {
        GL_CALL(glDeleteBuffers(1, &id));
    }

    template <uint32_t ST>
    ShaderObject<ST>::ShaderObject(const std::string& buf) : GLObject() {
        GL_CALL(id = glCreateShader(ST));
        GL_CALL(glShaderBinary(1, &id, GL_SHADER_BINARY_FORMAT_SPIR_V, buf.data(), buf.size()));
        GL_CALL(glSpecializeShader(id, "main", 0, nullptr, nullptr));
    }
    template <uint32_t ST>
    ShaderObject<ST>::~ShaderObject() {
        GL_CALL(glDeleteShader(id));
    }

    VtxArray::VtxArray() : mIdx(0) {
        GL_CALL(glGenVertexArrays(1, &id));
        GL_CALL(glBindVertexArray(id));
    }

    template <typename T>
    void VtxArray::PushAttribPointer(uint32_t dim, bool bnorm, uint32_t stride, size_t offset) {
        GL_CALL(glVertexAttribPointer(mIdx, dim, GL_enum_value<T>, bnorm, stride * sizeof(T), (void*)(offset * sizeof(T))));
        GL_CALL(glEnableVertexAttribArray(mIdx));
        ++mIdx;
    }
    void VtxArray::PopAttribPointer() {
        if (mIdx > 0) {
            --mIdx;
            GL_CALL(glDisableVertexAttribArray(mIdx));
        }
        THROW("index out of range!");
    }
    void VtxArray::Bind(bool bState) {
        uint32_t k = bState == true ? id : 0;
        GL_CALL(glBindVertexArray(k));
    }
    VtxArray::~VtxArray() {
        GL_CALL(glDeleteVertexArrays(1, &id));
    }

    ShaderProgram::ShaderProgram(std::initializer_list<uint32_t> ids) {
        GL_CALL(id = glCreateProgram());
        for (uint32_t k : ids) GL_CALL(glAttachShader(id, k));
        GL_CALL(glLinkProgram(id));
    }
    void ShaderProgram::Bind(bool bState) {
        uint32_t i = bState == true ? id : 0;
        GL_CALL(glUseProgram(i));
    }
    ShaderProgram::~ShaderProgram() {
        GL_CALL(glDeleteProgram(id));
    }
}