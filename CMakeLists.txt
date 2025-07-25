cmake_minimum_required(VERSION 3.21)
project(banim LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Debug build") # uncomment for debug build

# ——————————————————————————————————————————————————————————————
# C++ Standard
# ——————————————————————————————————————————————————————————————
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ——————————————————————————————————————————————————————————————
# Optional: vcpkg manifest mode
# ——————————————————————————————————————————————————————————————
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE
      "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# ——————————————————————————————————————————————————————————————
# Locate dependencies via pkg-config
# ——————————————————————————————————————————————————————————————
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW   REQUIRED glfw3)
pkg_check_modules(GLEW   REQUIRED glew)
pkg_check_modules(CAIRO  REQUIRED cairo)
find_package(OpenGL REQUIRED)

# ——————————————————————————————————————————————————————————————
# banim library (single TU: init.cpp)
# ——————————————————————————————————————————————————————————————
add_library(banim STATIC
  src/init.cpp
  src/shapes.cpp
  src/scene.cpp
  src/animations.cpp
  src/text.cpp
  src/easing.cpp
  src/style.cpp
  src/arch_components.cpp
)

# Public includes
target_include_directories(banim
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    ${GLFW_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${CAIRO_INCLUDE_DIRS}
)

# Extra compile flags from pkg-config (e.g. -pthread)
target_compile_options(banim
  PUBLIC
    ${GLFW_CFLAGS_OTHER}
    ${GLEW_CFLAGS_OTHER}
    ${CAIRO_CFLAGS_OTHER}
)

# Library search paths
target_link_directories(banim
  PUBLIC
    ${GLFW_LIBRARY_DIRS}
    ${GLEW_LIBRARY_DIRS}
    ${CAIRO_LIBRARY_DIRS}
)

# Link against GLFW, GLEW, Cairo, and OpenGL
target_link_libraries(banim
  PUBLIC
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${CAIRO_LIBRARIES}
    OpenGL::GL
)

# ——————————————————————————————————————————————————————————————
# Example executables
# ——————————————————————————————————————————————————————————————
add_executable(example examples/example.cpp)
target_link_libraries(example PRIVATE banim)

add_executable(architecture_demo examples/architecture_demo.cpp)
target_link_libraries(architecture_demo PRIVATE banim)

# debugging options
target_compile_options(banim PRIVATE -g -O0)
target_compile_options(example PRIVATE -g -O0)
target_compile_options(architecture_demo PRIVATE -g -O0)