#include "graphicsLibs.h"

#include <iostream>
#include <exception>

namespace ManageSetup {
static bool glfwContextExists = false;

GLFWwindow* GlfwContextManage::GlfwWindowContextSetup(int width, int height, const char* windowName) {
    // Init glfw to use opengl 4.6
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, windowName, NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        // TODO: throw error
        // return -1;
    }

    glfwContextExists = true;

    glfwMakeContextCurrent(window);
    return window;
}

extern int LoadOpenGL() {
    if (!glfwContextExists) {
        throw std::runtime_error("glfw context must exist before loading opengl");
    }

    // Init glad so opengl functions can be used
    static bool isLoaded = false;
    if (isLoaded) return 1;
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialise GLAD" << std::endl;
        // TODO: throw error
        return -1;
    }

    isLoaded = true;
    return 1;
}

// uses opengl 3 and glfw
extern void ImguiContextSetup(GLFWwindow* window) {
    const char* glsl_version = "#version 460";
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

extern void ImguiContextDestroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

extern void ImguiContextStartFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

extern void ImguiContextEndFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

};  // namespace ManageSetup


// NOTE: could use glDebugOutput after creating a debug context in glfw, because I am using opengl 4.6 and it could give more detailed error messages
GLenum GlCheckError(const char* file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }

        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}