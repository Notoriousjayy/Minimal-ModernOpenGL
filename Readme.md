# ModernGLProject

A minimal **modern OpenGL** project using **GLFW**, **GLAD**, and **GLM**, built with **CMake**. This project demonstrates how to:

- Initialize a Core Profile OpenGL 3.3 context using GLFW
- Load OpenGL function pointers with GLAD
- Use GLM for matrix/vector math
- Encapsulate shader compilation & linking in a `GLSLProgram` class
- Encapsulate VBO/VAO drawing in a `VertexArray` class

It draws a simple colored triangle with a basic vertex and fragment shader, plus an MVP matrix uniform.

---

## Contents

```
.
├── CMakeLists.txt            # Main CMake build script
├── include/
│   ├── glslprogram.h         # GLSL shader program class header
│   └── vertexarray.h         # Vertex array/VBO class header
├── install_dependencies.sh   # Checks & installs system packages if needed
├── shaders/
│   ├── simple.frag           # Fragment shader
│   └── simple.vert           # Vertex shader
└── src/
    ├── glslprogram.cpp       # GLSL shader program implementation
    ├── main.cpp              # Entry point: creates window, renders triangle
    └── vertexarray.cpp       # Vertex array/VBO implementation
```

---

## Prerequisites

1. A **Debian/Ubuntu**-like system with:

   - C++ compiler (e.g., `g++` from `build-essential`)
   - `cmake`
   - Mesa OpenGL development libraries (`libgl1-mesa-dev`)
   - X development libraries (`xorg-dev`)

2. **CMake** 3.14 or later.

3. A GPU/driver that supports **OpenGL 3.3** or above.

---

## Quick Start

### 1. Install Dependencies

If you’re running on Ubuntu/WSL or a similar Debian-based environment, run:

```bash
./install_dependencies.sh
```

This script checks for and installs:

- `build-essential`
- `cmake`
- `libgl1-mesa-dev`
- `xorg-dev`

If you prefer manual installation, you can do:

```bash
sudo apt-get update
sudo apt-get install build-essential cmake libgl1-mesa-dev xorg-dev
```

### 2. Build & Run

Use the `build_and_run.sh` script to configure, compile, and launch the demo:

```bash
./build_and_run.sh
```

Under the hood, this script:

1. Creates a `build/` directory (if not present).
2. Runs `cmake ..` to configure the project.
3. Runs `make` to compile the code.
4. Executes `./ModernGLProject` from `build/`.

> **Note**: If you prefer manual steps:
>
> ```bash
> mkdir build
> cd build
> cmake ..
> make
> ./ModernGLProject
> ```

---

## Project Overview

### **Shader Program Class** (`GLSLProgram`)

Encapsulates:

- Reading & compiling GLSL vertex/fragment shaders
- Linking them into a single OpenGL program
- Setting uniform variables (floats, integers, vectors, matrices)

**Key methods**:

- `bool create(const char* vertFile, const char* fragFile)`
- `void use() const`
- `static void stop()`
- `setUniform(...)` overloads for float, int, vec3, mat4

### **Vertex Array Class** (`VertexArray`)

Emulates immediate-style drawing with a high-level interface:

- `begin(GLenum mode)` / `end()` to collect vertices
- Automatically uploads vertex data (position/color) to a VAO+VBO
- `draw()` performs a draw call (e.g., `glDrawArrays`)

### **Shaders** (`simple.vert`, `simple.frag`)

- A simple vertex shader transforms each vertex by an MVP matrix, passing color along.
- A simple fragment shader outputs the interpolated color.

---

## Running on WSL/Ubuntu

1. **Install** X server on Windows (like **VcXsrv** or **Xming**) if you want to see the actual OpenGL window.
2. **Export** your `DISPLAY` variable inside WSL:
   ```bash
   export DISPLAY=:0
   ```
   or the IP address if using a custom X server config.
3. **Install** the dev packages with `install_dependencies.sh`.
4. **Run** `./build_and_run.sh`.
5. A window with a colored triangle should appear on your X server.

---

## Troubleshooting

- **Could NOT find OpenGL**:  
  Ensure you have installed `libgl1-mesa-dev` (and `xorg-dev`) on Debian/Ubuntu.
- **Header conflicts** (`#error OpenGL header already included`):  
  Ensure you `#define GLFW_INCLUDE_NONE` and include `<glad/glad.h>` before `<GLFW/glfw3.h>`.
- **GLM not found**:  
  Make sure you’re linking `glm::glm` in `target_link_libraries`.

---

## Next Steps

- **Add indices** for `VertexArray` to draw with `GL_ELEMENT_ARRAY_BUFFER`.
- **Expand** the shaders with lighting, textures, or instancing.
- **Integrate** an ECS or scene-graph approach for a larger engine.

---

## License

This project is provided as-is for educational/demo purposes. For real use, please check each dependency’s license (GLFW, GLAD, GLM, etc.).

---

**Enjoy building modern OpenGL projects!** If you have any issues or questions, feel free to modify or raise them in your own repository.
