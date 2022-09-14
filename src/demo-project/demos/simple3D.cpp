#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "demos.h"

using namespace Renderer;

namespace ThreeD {

struct CamRef {
    Camera* cam = nullptr;

    void RegisterCam(Camera& c) {
        cam = &c;
    }

    Camera* GetCam() {
        return cam;
    }
};

CamRef cref;

float lastX = 400, lastY = 300;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    if (cref.GetCam()) cref.GetCam()->ProcessMouseMovement(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (cref.GetCam()) cref.GetCam()->ProcessMouseScroll(static_cast<float>(yoffset));
}

struct ThreeD {
    GLuint VAO;
    ManagedResources<Shader> shaders;
    ManagedResources<Texture> textures;
    const Shader* ShaderInUse = nullptr;

    Camera camera;

    float deltaTime = 0.0f;  // Time between current frame and last frame
    float lastFrame = 0.0f;  // Time of last frame

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection;

    std::vector<glm::vec3> cubePositions = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    ThreeD() {
        cref.RegisterCam(camera);
        Initialise3DDemo();
    }

    ~ThreeD() {
        // delete the VAO
        glDeleteVertexArrays(1, &VAO);
    }

    void Initialise3DDemo() {
        glfwSetCursorPosCallback(glfwGetCurrentContext(), mouse_callback);
        glfwSetScrollCallback(glfwGetCurrentContext(), scroll_callback);

        glEnable(GL_DEPTH_TEST);  // this would be enabled all the time, there is currently no control for toggling this in the renderer

        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        projection = glm::perspective(glm::radians(camera.fov), 800.0f / 600.0f, 0.1f, 100.0f);

        float vertices[] = {
            // vertex position --tex coords
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

        textures.OwnRes("container", std::move(Texture(Demos::TEXTURE_PATH / "container.jpg", TexUnit::T0, ImageDataType::RGB).Build()));
        textures.OwnRes("smiley", std::move(Texture(Demos::TEXTURE_PATH / "awesomeface.png", TexUnit::T15, ImageDataType::RGBA).Build()));
        shaders.OwnRes("textureDemo", std::move(Shader(Demos::SHADER_PATH / "textureDemo.vs", Demos::SHADER_PATH / "textureDemo.fs").Build()));

        shaders.OwnRes("3DDemo", std::move(Shader(Demos::SHADER_PATH / "3DDemo.vs", Demos::SHADER_PATH / "textureDemo.fs").Build()));
        shaders.GetRes("3DDemo")->Use();

        ShaderInUse = shaders.GetRes("3DDemo");

        GLuint VBO;

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // load vertex position data into vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // load texture coordinates into vertex attributes
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // unbind the VAO, so it doesn't manage refer to anything else
        glBindVertexArray(0);

        // delete the VBO and EBO (must be done after unbinding the vertex array object)
        glDeleteBuffers(1, &VBO);
    }

    void processInput(GLFWwindow* window) {
        // do similar thing for the other directions
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(CameraControl::FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(CameraControl::BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(CameraControl::LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(CameraControl::RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) camera.ProcessKeyboard(CameraControl::RESET, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.ProcessKeyboard(CameraControl::UP, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera.ProcessKeyboard(CameraControl::DOWN, deltaTime);
    }

    void Draw() {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(glfwGetCurrentContext());
        projection = glm::perspective(glm::radians(camera.fov), 800.0f / 600.0f, 0.1f, 100.0f);

        glBindVertexArray(VAO);

        ShaderInUse->Use();

        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i] - glm::vec3{0, 0, 20});
            float angle = 2.0f * (i + 1);
            model = glm::rotate(model, (float)glfwGetTime() * angle * 0.2f, glm::vec3(1.0f, 0.3f, 0.5f));
            Uniform::Set(*ShaderInUse, "model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 view{camera.GetViewMatrix()};

        // Uniform::Set(*ShaderInUse, "model", model);
        Uniform::Set(*ShaderInUse, "view", view);
        Uniform::Set(*ShaderInUse, "projection", projection);

        Uniform::Set(*ShaderInUse, "texture1", *textures.GetRes("container"));
        Uniform::Set(*ShaderInUse, "texture2", *textures.GetRes("smiley"));

        glBindVertexArray(0);
    }

    // TODO: Improve use of ImGui to make a more understandable ui
    void ImguiMenu() {
        // TODO: Move this functionality into the renderer
        static bool drawWireFrame = false;

        // NOTE: It could be better to have things which change the state available in a top level imgui
        ImGui::Begin("Simple ThreeD");

        if (ImGui::Button("Toggle Wireframe Mode")) {
            drawWireFrame = !drawWireFrame;
            if (drawWireFrame) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }

        ImGui::BeginChild(1);
        ImGui::Text("Shaders");
        if (ImGui::Button("rebuild current shader")) ShaderInUse->Build().Use();
        if (ImGui::Button("default")) ShaderInUse = shaders.GetRes(Shader::DEFAULT_SHADER_PROGRAM_NAME);
        for (const auto& prog : shaders.existingResources) {
            if (ImGui::Button(prog.first.c_str())) ShaderInUse = shaders.GetRes(prog.first);
        }
        ImGui::EndChildFrame();

        ImGui::End();
    }
};

}  // namespace ThreeD

extern void Demos::Simple3D() {
    static ThreeD::ThreeD demo;

    demo.Draw();
    demo.ImguiMenu();
}
