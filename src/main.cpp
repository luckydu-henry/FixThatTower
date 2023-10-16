#include <iostream>
#include <format>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#define _U8(STR) reinterpret_cast<const char*>(u8##STR)

int main(int argc, char* argv[]) {


    if (!glfwInit()) {
        std::cout << "Failed to init glfw!\n";
    }

    glfwSetErrorCallback([](int error, const char* desc) {
        std::cerr << std::format("GLFW error: ({:d}) {:s}\n", error, desc);
        glfwTerminate();
    });

    // For Vulkan usage.
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, _U8("ÄãºÃÊÀ½ç"), nullptr, nullptr);

    uint32_t count = 0;

    vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);

    std::cout << count << std::endl;

    while (!glfwWindowShouldClose(window)) {
        // Update window events.
        glfwPollEvents();
    }

    glfwTerminate();
}
