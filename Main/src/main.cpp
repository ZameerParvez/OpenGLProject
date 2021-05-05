#include "demos/demos.h"
#include "renderer/renderer.h"

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

auto framebuffer_size_callback = [](GLFWwindow* window, int width, int height) -> void { glViewport(0, 0, width, height); };


int main() {
    constexpr int WIDTH = 800;
    constexpr int HEIGHT = 600;

    GLFWwindow* window = ManageSetup::GlfwContextManage::Instance(WIDTH, HEIGHT, "LearnOpenGL").window;

    ManageSetup::LoadOpenGL();

    // set viewport size and add call back to update size when the window is expanded or shrunk
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // set up imgui
    ManageSetup::ImguiContextSetup(window);

    // the render loop
    // Infinite loop until we want to close the window
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // set colour to clear to, and clear (clearcolor changes the state of the context, so calling clear after would always colour it the same anyways)
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ManageSetup::ImguiContextStartFrame();

        // do rendering here
        Demos::RunDemos();

        // ====

        ManageSetup::ImguiContextEndFrame();
        
        // poll input events and calls callbacks, and swaps buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ManageSetup::ImguiContextDestroy();
    return 0;
}
