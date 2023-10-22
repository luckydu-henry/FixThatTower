#include <iostream>
#include <format>
#include <fstream>
#include <string>
#include <memory>
#include <source_location>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GLObject.hpp>
#include <LogColor.hpp>
#include <LogLogger.hpp>

#define U8(STR)         reinterpret_cast<const char*>(u8##STR)
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

constexpr uint32_t WIDTH = 1920, HEIGHT = 1080;

int main(int argc, char* argv[]) {
    if (!glfwInit()) {
        THROW("Can't initialize GLFW!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    std::shared_ptr<GLFWwindow> window{ glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr), glfwDestroyWindow };

    glfwSetFramebufferSizeCallback(window.get(), [](GLFWwindow* wd, int w, int h) {
        GL_CALL(glViewport(0, 0, w, h));
    });

    glfwMakeContextCurrent(window.get());

    // Load OpenGL function pointers.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        THROW("Can't load OpenGL function pointers!");
    }
    IE::Util::lConsole.Info("OpenGL version: {:s}", (const char*)glGetString(GL_VERSION));
    
    std::ifstream vSource("../res/triangle.vert.spv", std::ios::binary), // Must be binary form !!!
                  fSource("../res/triangle.frag.spv", std::ios::binary); // Must be binary form !!!

    std::string vbuf{ std::istreambuf_iterator<char>(vSource), std::istreambuf_iterator<char>() }, 
                fbuf{ std::istreambuf_iterator<char>(fSource), std::istreambuf_iterator<char>() };
    {

    IE::VtxShader vsd{ vbuf };
    IE::PixShader psd{ fbuf };

    IE::ShaderProgram spg{ vsd.id, psd.id };

    float bd[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    uint32_t ed[] = {
        0,1,2,
        0,2,3
    };


    IE::VtxArray  vao;
    IE::VtxBuffer vbo{ bd,IE::VtxBuffer::StaticDraw };
    IE::IdxBuffer ibo{ ed,IE::IdxBuffer::StaticDraw };

    vao.PushAttribPointer<float>(2, false, 2, 0);

    vbo.Bind(false);
    vao.Bind(false);

    while (!glfwWindowShouldClose(window.get())) {
        float cbd[] = { std::sinf((float)glfwGetTime()), 0.0f, 0.0f, 1.0f };

        glClearBufferfv(GL_COLOR, 0, cbd);

        spg.Bind(true);
        vao.Bind(true);
        GL_CALL(glDrawElements(GL_TRIANGLES, std::size(ed), IE::GL_enum_value<decltype(ibo)::value_type>, nullptr));

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    }
    glfwTerminate();
}
