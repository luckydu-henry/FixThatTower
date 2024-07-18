cmake_minimum_required(VERSION 3.16)
project(renderer_module VERSION 1.0
                        DESCRIPTION "Small but quick renderer with modern C++20"
                        LANGUAGES C CXX)

find_package(Vulkan REQUIRED)
add_subdirectory(vendor/glfw)

file(GLOB RDMD_HEADER  "include/*.hpp")
file(GLOB RDMD_SOURCE  "src/*.cpp")

add_library               (librdmd STATIC ${RDMD_HEADER} ${RDMD_SOURCE} "vendor/stb/stb.cpp")
target_compile_features   (librdmd STATIC cxx_std_20)
# To add force just simply add its include folder.
target_include_directories(librdmd STATIC "include/" "vendor/force/include/" "vendor/stb/stb.hpp")
target_link_libraries     (glfw ${GLFW_LIBRARIES} Vulkan::Vulkan)

file(GLOB EXAMPLE_SOURCES "example/*.cpp")
foreach(src ${EXAMPLE_SOURCES})
    get_filename_component(src_name ${src} NAME_WE)
    add_executable            (${src_name} ${src})
    target_compile_features   (${src_name} PUBLIC cxx_std_20)
    target_include_directories(${src_name} PUBLIC "include/" "vendor/force/include/")
    target_link_libraries     (${src_name} PUBLIC )
    set_target_properties     (${src_name} PROPERTIES FOLDER "example")
endforeach(src ${EXAMPLE_SOURCES})