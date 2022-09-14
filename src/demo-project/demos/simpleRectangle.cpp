#include "demos.h"

using namespace Renderer;

namespace SimpleRectange {

struct Rectangle {
    GLuint VAO;
    ManagedResources<Shader> shaders;
    const Shader* ShaderInUse = nullptr;

    Rectangle() {
        InitialiseRectangle();
    }

    ~Rectangle() {
        // delete the VAO
        glDeleteVertexArrays(1, &VAO);
    }

    void InitialiseRectangle() {
        float vertices[] = {
            // vertices         // colours
            0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top left
        };
        unsigned int indices[] = {
            // note that we start from 0!
            0, 1, 3,  // first triangle
            1, 2, 3   // second triangle
        };

        shaders.OwnRes("test", std::move(Shader(Demos::SHADER_PATH / "testVertex.vs", Shader::DEFAULT_FRAGMENT_SHADER).Build()));
        shaders.GetRes("test")->Use();
        ShaderInUse = shaders.GetRes("test");

        GLuint VBO, EBO;

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // load vertex position data into vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // load vertex colour data into vertex attributes
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // unbind the VAO, so it doesn't manage refer to anything else
        glBindVertexArray(0);

        // delete the VBO and EBO (must be done after unbinding the vertex array object)
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void Draw() {
        ShaderInUse->Use();
        Uniform::Set(*ShaderInUse, "time", static_cast<float>(glfwGetTime()));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    // TODO: Improve use of ImGui to make a more understandable ui
    void ImguiMenu() {
        // TODO: Move this functionality into the renderer
        static bool drawWireFrame = false;

        // NOTE: It could be better to have things which change the state available in a top level imgui
        ImGui::Begin("Simple Rectangle");

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
        if (ImGui::Button("default")) ShaderInUse = shaders.GetRes(Shader::DEFAULT_SHADER_PROGRAM_NAME);
        for (const auto& prog : shaders.existingResources) {
            if (ImGui::Button(prog.first.c_str())) ShaderInUse = shaders.GetRes(prog.first);
        }
        ImGui::EndChildFrame();

        ImGui::End();
    }
};

}

extern void Demos::SimpleRectangle() {
    static SimpleRectange::Rectangle rect;

    rect.Draw();
    rect.ImguiMenu();
}
