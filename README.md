## 🧬 Multithreaded Game of Life (OpenGL/C++)

This is a multithreaded implementation of Conway's Game of Life using OpenGL for rendering and C++ with std::thread for concurrency. The simulation grid evolves in real-time and is displayed using GPU-accelerated point rendering. Grid updates are performed in parallel to take full advantage of multi-core CPUs.

##### 🚀 Features

- 🔄 Conway's Game of Life logic with double-buffering
- 🧵 Parallel cell processing using std::thread and std::barrier
- 🖼️ OpenGL rendering with dynamic shader support
- 🪟 Custom Window class with GLFW for window management
- 🧱 Adjustable grid resolution and screen resizing support
- 🕹️ Live rendering at ~60 FPS with gl_PointSize-based pixel control
- 🔤 Text rendering layer texture atlas and projecting text onto the viewport, enabling efficient dynamic display of in-game info and UI elements
- 🧩 A Paths Management class for shader files and font bitmap.

##### 🧰 Dependencies

- [GLFW](https://www.glfw.org/) — window/context management
- [GLAD](https://glad.dav1d.de/) — OpenGL function loader
- OpenGL 3.3 or later
- C++ compliant compiler (GCC 11+, Clang 12+, MSVC 2022)

##### 🛠️ Build Instructions/✅ Requirements

- CMake 3.10+
- A C++ compiler (e.g. g++, clang++, or MSVC)
- Git (for pulling submodules, if applicable)

#### 🧾 Build Steps

##### Clone the project

    git clone https://github.com/yourname/OpenGLProject.git
    cd OpenGLProject

##### Create a build directory & run cmake

    # 1. Create build dir (if not exist)
    mkdir -p build

    # 2. Configure the project
    cmake -S . -B build

    # 3. Build
    cmake --build build

##### 🔄 Clean Build

    cmake --build build --target clean-all

    #or manualy
    rm -rf build

##### 🐛 Build Debug or Release

    #To build in Debug or Release mode:
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug   # or Release
    cmake --build build

#### 📁 Directory Structure

    OpenGLProject/
    │
    ├── include/ # Header files (Window, GameOfLife, Shader, etc.)
    ├── src/ # C++ implementation files
    ├── shaders/ # GLSL vertex & fragment shaders
    ├── external/ # GLAD & GLFW
    │ ├── glad/
    │ └── glfw/
    ├── build/ # CMake build folder (created at runtime)
    ├── CMakeLists.txt # Main build script
    └── README.md # This file

#### 📜 License

The GNU General Public License v3.0 GNU License.
