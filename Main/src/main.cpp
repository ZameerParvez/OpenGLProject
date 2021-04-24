#include <glad/glad.h> // NOTE: must be the first include
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

// helper functions to manage imgui context
namespace ManageImgui {
    const auto createContext = [](GLFWwindow* window) {
        const char* glsl_version = "#version 130";
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
    };

    const auto destroyContext = []() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    };

    const auto startFrame = []() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    };

    const auto endFrame = []() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    };    
}


void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

auto framebuffer_size_callback = [](GLFWwindow* window, int width, int height) -> void { glViewport(0, 0, width, height); };

int main() {
    // Init glfw to use opengl 4.6
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Init glad so opengl functions can be used
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    // set viewport size and add call back to update size when the window is expanded or shrunk
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // set up imgui
    ManageImgui::createContext(window);

    // the render loop
    // Infinite loop until we want to close the window
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // set colour to clear to, and clear (clearcolor changes the state of the context, so calling clear after would always colour it the same anyways)
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ManageImgui::startFrame();

        // do rendering here
        bool a = true;
        ImGui::ShowDemoWindow(&a);

        // ====

        ManageImgui::endFrame();
        // poll input events and calls callbacks, and swaps buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up imgui
    ManageImgui::destroyContext();

    glfwTerminate();
    return 0;
}
