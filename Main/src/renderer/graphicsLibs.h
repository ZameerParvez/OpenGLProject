#pragma once

#include "openglLoader.h"  // NOTE: must be included before glfw3

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace ManageSetup {

// NOTE: Assumes that only one window is wanted (I think this could be extended to make multiple windows though, by adding a function that uses glfwCreateWindow and stores the new windows on a list, so they can be referenced later)
class GlfwContextManage {
   private:
    GlfwContextManage(int width, int height, const char* windowName) {
        window = GlfwContextManage::GlfwWindowContextSetup(width, height, windowName);
    }
    GlfwContextManage(const GlfwContextManage& c) = delete;

    static GLFWwindow* GlfwWindowContextSetup(int width, int height, const char* windowName);

    static void GlfwWindowContextDestroy() {
        glfwTerminate();
    }

   public:

    GLFWwindow* window = nullptr;

    ~GlfwContextManage() {
        GlfwContextManage::GlfwWindowContextDestroy();
    }

    static const GlfwContextManage& Instance(int width = 100, int height = 100, const char* windowName = "title") {
        static GlfwContextManage context(width, height, windowName);
        return context;
    }
};

extern int LoadOpenGL();

// uses opengl 3 and glfw
extern void ImguiContextSetup(GLFWwindow* window);

extern void ImguiContextDestroy();

extern void ImguiContextStartFrame();

extern void ImguiContextEndFrame();
}  // namespace ManageSetup

extern GLenum GlCheckError(const char* file, int line);
#define GL_CHECK_ERROR() GlCheckError(__FILE__, __LINE__)