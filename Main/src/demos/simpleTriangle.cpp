#include "demos.h"

// NOTE: These demos might be better as objects so that they can de allocate resources at destruction time
namespace SimpleTriange {

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

constexpr char* fragmentShaderSource2 =
    "#version 460 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\0";

// TODO: Be able to change these in imgui (would need to keep track of what shaders and VAOs have been made)
GLuint ShaderInUse[2];
GLuint VAOInUse[2];

void InitialiseTriangle() {
    float vertices1[]{
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    float vertices2[]{
        -1.0f, -0.7f, 0.0f,
        -0.6f, -0.6f, 0.0f,
        -0.2f, -0.7f, 0.0f
    };

    GLuint vertexShader = CompileShader(vertexShaderSource, ShaderType::VERTEX);
    GLuint fragmentShader = CompileShader(fragmentShaderSource, ShaderType::FRAGMENT);
    GLuint shaderProgram = LinkShaders(vertexShader, fragmentShader);
    
    GLuint fragmentShader2 = CompileShader(fragmentShaderSource2, ShaderType::FRAGMENT);
    GLuint shaderProgram2 = LinkShaders(vertexShader, fragmentShader2);

    // use the program (already compiled and linked) and delete the no longer needed shader objects
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader2);

    auto initVertexArray = [](float* vertices, size_t sizeOfVertices) -> GLuint {
        GLuint VAO, VBO;

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeOfVertices, vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));
        glEnableVertexAttribArray(0);

        // unbind the VAO, so it doesn't manage refer to anything else
        glBindVertexArray(0);

        return VAO;
    };
    
    VAOInUse[0] = initVertexArray(vertices1, sizeof(vertices1));
    VAOInUse[1] = initVertexArray(vertices2, sizeof(vertices2));

    ShaderInUse[0] = shaderProgram;
    ShaderInUse[1] = shaderProgram2;
}

void ImguiMenu() {
    static bool drawWireFrame = false;

    // NOTE: It could be better to have things which change the state available in a top level imgui
    ImGui::Begin("Simple Triangles");

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

extern void Demos::SimpleTriangle(bool init) {
    if (init) {
        SimpleTriange::InitialiseTriangle();
    }

    for (unsigned int i = 0; i < 2; i++) {
        glUseProgram(SimpleTriange::ShaderInUse[i]);
        glBindVertexArray(SimpleTriange::VAOInUse[i]);
        glDrawArrays(GL_TRIANGLES, 0, 6);   
    }
    glBindVertexArray(0);
    
    SimpleTriange::ImguiMenu();
}
