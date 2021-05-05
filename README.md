# OpenGL Project
## What is this?
- This is my code base for following the [Learn OpenGL Tutorials](https://learnopengl.com)
- I will also use it for other graphics demos

## Structure
- lib contains the libraries used by this project
  - GLAD
  - GLFW
  - DearImGui
- Main contains the source for this project and the cmake file that builds the executable
  - src contains the source files
    - renderer contains useful abstractions for using opengl
    - demos contains some demos using opengl and ImGui

## How To Use
- open the terminal in the top level of this project, and build the project outside of the source tree

```bash
mkdir build
cmake -S . -B build
cd build
make
```

- the executable should be "build/Main/Main", if you are using linux
  - on another system having cmake build the project could emit build configuration for other build tools, such as a visual studio solution, which could be used to build and run the project instead

## Notes
- When adding new demos, the demos header must be updated to refer to the new demo function. The cmake file must also be updated to include any new source files
  - If new shaders are added, or old ones are edited, then the cmake file for Main would need to be reconfigured so that the shaders are copied to the build directories (unless the code references the shaders by their full paths, instead of relative paths)
