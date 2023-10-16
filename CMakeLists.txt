cmake_minimum_required(VERSION 3.15)
project(IvoryTower VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory("vendor/glfw")
# Add VULKAN_SDK include and lib directories.
include_directories($ENV{VULKAN_SDK}/Include)
link_directories   ($ENV{VULKAN_SDK}/Lib)

# Add source files
file(GLOB_RECURSE TOWER_SOURCES FILES "./src/*.c" "./src/*.cpp")
# Ivory tower game settings.
add_executable       (TowerGame ${TOWER_SOURCES})
set_target_properties(TowerGame PROPERTIES 
                                CXX_STANDARD 20 
                                COMPILE_WARNING_AS_ERROR ON)
# Add glfw to game project.
target_link_libraries      (TowerGame PRIVATE glfw vulkan-1)

# Set startup project as TowerGame for MSVC but not ALL_BUILD
set_property         (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT TowerGame)