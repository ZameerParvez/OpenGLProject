#include "demos.h"

namespace SimpleRectangle {

constexpr char* vertexShaderSource =
    "#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

constexpr char* fragmentShaderSource =
    "#version 460 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

// TODO: Be able to change these in imgui (would need to keep track of what shaders and VAOs have been made)
GLuint ShaderInUse;
GLuint VAOInUse;

void InitialiseRectangle() {
    float vertices[] = {
        0.5f, 0.5f, 0.0f,    // top right
        0.5f, -0.5f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f, 0.5f, 0.0f    // top left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };

    GLuint vertexShader = CompileShader(vertexShaderSource, ShaderType::VERTEX);
    GLuint fragmentShader = CompileShader(fragmentShaderSource, ShaderType::FRAGMENT);
    GLuint shaderProgram = LinkShaders(vertexShader, fragmentShader);

    // use the program (already compiled and linked) and delete the no longer needed shader objects
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind the VAO, so it doesn't manage refer to anything else
    glBindVertexArray(0);

    VAOInUse = VAO;
    ShaderInUse = shaderProgram;
}

void ImguiMenu() {
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

    ImGui::End();
}
}

// may not work as expected
extern void Demos::SimpleRectangle(bool init) {
    if (init) {
        SimpleRectangle::InitialiseRectangle();
    }

    glUseProgram(SimpleRectangle::ShaderInUse);
    glBindVertexArray(SimpleRectangle::VAOInUse);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    SimpleRectangle::ImguiMenu();
}
