cmake_minimum_required(VERSION 3.15)
project(IvoryTower VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory("vendor/glfw")
# Add VULKAN_SDK include and lib directories.

include_directories("vendor/glad/include")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add source files
file(GLOB_RECURSE TOWER_SOURCES FILES "./src/*.c" "./src/*.cpp" "./include/*.hpp")
file(GLOB_RECURSE GLAD_SOURCES  FILES "./vendor/glad/src/*.c" "./vendor/glad/include/*.h")

# Copy resources to the build directory.
add_custom_target(copy_resources ALL
                  COMMAND ${CMAKE_COMMAND} -E copy_directory
                  ${PROJECT_SOURCE_DIR}/res
                  ${PROJECT_BINARY_DIR}/res
                  COMMENT "Copying resources into binary folder")

# Ivory tower game settings.
add_executable       (TowerGame ${TOWER_SOURCES} ${GLAD_SOURCES})
set_target_properties(TowerGame PROPERTIES 
                                CXX_STANDARD 20)
# Add glfw to game project.
target_link_libraries      (TowerGame PRIVATE glfw)

add_dependencies(TowerGame copy_resources)

# Set startup project as TowerGame for MSVC but not ALL_BUILD
set_property         (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT TowerGame)