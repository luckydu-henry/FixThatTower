#include <iostream>
#include <format>
#include <fstream>
#include <string>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <LogColor.hpp>
#include <LogLogger.hpp>

#define U8(STR)         reinterpret_cast<const char*>(u8##STR)
#define THROW(STR)      throw std::runtime_error(STR)
#ifdef _DEBUG
#define GL_CALL(STATE)  do {                            \
STATE; auto v = glGetError();                           \
if (v != GL_NO_ERROR)                                   \
    IE::Util::lConsole.Error("GL_ERROR: {:d}|{:s}", v); \
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


    uint32_t vid = 0, fid = 0, pg = 0;

    GL_CALL(vid = glCreateShader(GL_VERTEX_SHADER));
    GL_CALL(fid = glCreateShader(GL_FRAGMENT_SHADER));

    GL_CALL(glShaderBinary(1, &vid, GL_SHADER_BINARY_FORMAT_SPIR_V, vbuf.data(), vbuf.size()));
    GL_CALL(glSpecializeShader(vid, "main", 0, nullptr, nullptr));

    GL_CALL(glShaderBinary(1, &fid, GL_SHADER_BINARY_FORMAT_SPIR_V, fbuf.data(), fbuf.size()));
    GL_CALL(glSpecializeShader(fid, "main", 0, nullptr, nullptr));

    GL_CALL(pg = glCreateProgram());
    GL_CALL(glAttachShader(pg, vid));
    GL_CALL(glAttachShader(pg, fid));

    GL_CALL(glLinkProgram(pg));

    GL_CALL(glDeleteShader(vid));
    GL_CALL(glDeleteShader(fid));

    float bd[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.0f,  0.5f
    };

    uint32_t vbo = 0, vao = 0;

    GL_CALL(glGenVertexArrays(1, &vao));
    GL_CALL(glBindVertexArray(vao));

    GL_CALL(glGenBuffers(1, &vbo));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(bd), bd, GL_STATIC_DRAW));

    GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
    GL_CALL(glEnableVertexAttribArray(0));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CALL(glBindVertexArray(0));

    while (!glfwWindowShouldClose(window.get())) {

        GL_CALL(glClearColor((float)std::sin(glfwGetTime()), 0.0f, 0.5f, 1.0f));
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        GL_CALL(glUseProgram(pg));
        GL_CALL(glBindVertexArray(vao));
        GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 3));

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    GL_CALL(glDeleteProgram(pg));
    GL_CALL(glDeleteBuffers(1, &vbo));
    GL_CALL(glDeleteVertexArrays(1, &vao));

    glfwTerminate();
}
