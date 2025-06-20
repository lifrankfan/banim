# banim

Add VCPKG_ROOT to .bashrc and source:

export VCPKG_ROOT=$HOME/vcpkg

Create build directory:
mkdir build && cd build

Run CMakeLists.txt

cmake .. \
  -G "Unix Makefiles" \
  -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake

Run Make
Make

Run project
./project_name

+-----------------------+       +------------------------+
|    Your Shapes        |       |      Scene             |
| (Rectangle, Circle)   |       | - List of shapes       |
+-----------------------+       | - render()             |
          |                     +------------------------+
          |                             |
          v                             v
+-----------------------+       +------------------------+
|   CairoSurface (CPU)  |<------+  shape->draw(cr)       |
|  - 2D drawing buffer  |                             |
+-----------------------+                             |
          | Cairo renders                             |
          v                                           |
+-----------------------+                             |
|  Texture2D (GPU)      |<-----------------------------+
|  - Gets uploaded with Cairo pixels                  |
+-----------------------+
          |
          v
+-----------------------+
|     Shader (GPU)      |
| - Vertex: puts quad on screen |
| - Fragment: samples texture   |
+-----------------------+
          |
          v
+-----------------------+
|  OpenGL draws quad    |
| with Cairo image      |
+-----------------------+
          |
          v
+-----------------------+
|   GLFW window         |
| (GLContext)           |
+-----------------------+
