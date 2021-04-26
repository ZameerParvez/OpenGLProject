#pragma once
#include "../graphicsLibs.h"

// Contains defenitions of the functions that will be created in seperate cpp files
namespace Demos {

using demoFunc = void (*)(bool);

// === Demos ===
extern void SimpleTriangle(bool init = false);
extern void SimpleRectangle(bool init = false);
// =============

// NOTE: if the same thing is selected then the same shape will be initialised again
void ImguiDemoSelector(Demos::demoFunc& currDemo, bool& initialSetupDone);
void RunDemos();

}  // namespace Demos


enum class ShaderType {
    VERTEX,
    FRAGMENT
};

// TODO: move these functions to a better place when there are more functions to wrap opengl things, also store shaders so they can also be changed at runtime
// returns a reference to a compiled shader (does not manage the deletion of the shader)
GLuint CompileShader(const char* shaderSource, ShaderType type);
// currently returns the reference to the compiled shader, and compiles vertex
GLuint LinkShaders(GLuint vertexShader, GLuint fragmentShader);

// NOTE: could use glDebugOutput after creating a debug context in glfw, because I am using opengl 4.6 and it could give more detailed error messages
GLenum GlCheckError(const char* filen, int line);
#define GL_CHECK_ERROR() GlCheckError(__FILE__, __LINE__) 
