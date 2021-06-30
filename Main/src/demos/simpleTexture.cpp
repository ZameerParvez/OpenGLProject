#include "demos.h"

#include <iostream>

using namespace Renderer;

namespace TexDemo {

struct TexDemo {
    GLuint VAO;
    Texture tex[2];
    ManagedResources<Shader> shaders;
    const Shader* ShaderInUse = nullptr;

    TexDemo() {
        InitialiseTexDemo();
    }

    ~TexDemo() {
        // delete the VAO
        glDeleteVertexArrays(1, &VAO);
    }

    void InitialiseTexDemo() {
        tex[0] = std::move(Texture("src/resources/textures/container.jpg", TexUnit::T0, ImageDataType::RGB).Build());
        tex[1] = std::move(Texture("src/resources/textures/awesomeface.png", TexUnit::T15, ImageDataType::RGBA).Build());

        float vertices[] = {
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
        };
        

        unsigned int indices[] = {
            // note that we start from 0!
            0, 1, 3,  // first triangle
            1, 2, 3   // second triangle
        };


        shaders.OwnRes("textureDemo", std::move(Shader("src/resources/shaders/textureDemo.vs", "src/resources/shaders/textureDemo.fs").Build()));
        shaders.GetRes("textureDemo")->Use();
        ShaderInUse = shaders.GetRes("textureDemo");

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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // load vertex colour data into vertex attributes
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // load texture coordinates into vertex attributes
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2); 

        // unbind the VAO, so it doesn't manage refer to anything else
        glBindVertexArray(0);

        // delete the VBO and EBO (must be done after unbinding the vertex array object)
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void Draw() {
        ShaderInUse->Use();

        Uniform::Set(*ShaderInUse, "texture1", tex[0]);
        Uniform::Set(*ShaderInUse, "texture2", tex[1]);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    // TODO: Improve use of ImGui to make a more understandable ui
    void ImguiMenu() {
        // TODO: Move this functionality into the renderer
        static bool drawWireFrame = false;

        // NOTE: It could be better to have things which change the state available in a top level imgui
        ImGui::Begin("Simple TexDemo");

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
        if (ImGui::Button("default")) ShaderInUse = shaders.GetRes(shaders.DEFAULT_RES_NAME);
        for (const auto& prog : shaders.existingResources) {
            if (ImGui::Button(prog.first.c_str())) ShaderInUse = shaders.GetRes(prog.first);
        }
        ImGui::EndChildFrame();

        ImGui::End();
    }
};

}

extern void Demos::SimpleTexture() {
    static TexDemo::TexDemo texDemo;

    texDemo.Draw();
    texDemo.ImguiMenu();
}
