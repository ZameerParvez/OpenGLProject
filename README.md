# OpenGL Project
## What is this?
- This is my code base for following the [Learn OpenGL Tutorials](https://learnopengl.com)
- I will also use it for other graphics demos

## Structure
- lib contains the libraries used by this project
  - GLAD
  - GLFW
  - DearImGui
- Main contains the source for this project

## How To Use
- open the terminal in the top level of this project, and build the project outside of the source tree

```bash
mkdir build
cmake -S . -B build
cd build
make
```

- the executable should be "build/Main/Main"
