#include "demos.h"

using namespace Renderer;

// NOTE: These demos might be better as objects so that they can de allocate resources at destruction time
namespace SimpleTriange {

struct Triangle {
    const char* vertexShaderSource =
        "#version 460 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    const char* fragmentShaderSource =
        "#version 460 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

    const char* fragmentShaderSource2 =
        "#version 460 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
        "}\0";

    GLuint ShaderInUse[2];
    GLuint VAOInUse[2];

    Triangle() {
        InitialiseTriangle();
    }

    ~Triangle() {
        glDeleteVertexArrays(1, &VAOInUse[0]);
        glDeleteVertexArrays(1, &VAOInUse[1]);
    }

    void InitialiseTriangle() {
        float vertices1[]{
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f};
        float vertices2[]{
            -1.0f, -0.7f, 0.0f,
            -0.6f, -0.6f, 0.0f,
            -0.2f, -0.7f, 0.0f};

        GLuint vertexShader = Shader::CompileShader(vertexShaderSource, ShaderType::VERTEX);
        GLuint fragmentShader = Shader::CompileShader(fragmentShaderSource, ShaderType::FRAGMENT);
        GLuint shaderProgram = Shader::LinkShaders(vertexShader, fragmentShader);

        GLuint fragmentShader2 = Shader::CompileShader(fragmentShaderSource2, ShaderType::FRAGMENT);
        GLuint shaderProgram2 = Shader::LinkShaders(vertexShader, fragmentShader2);

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

            glDeleteBuffers(1, &VBO);

            return VAO;
        };

        VAOInUse[0] = initVertexArray(vertices1, sizeof(vertices1));
        VAOInUse[1] = initVertexArray(vertices2, sizeof(vertices2));

        ShaderInUse[0] = shaderProgram;
        ShaderInUse[1] = shaderProgram2;
    }

    void Draw() {
        for (unsigned int i = 0; i < 2; i++) {
            glUseProgram(ShaderInUse[i]);
            glBindVertexArray(VAOInUse[i]);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        glBindVertexArray(0);
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
};

}  // namespace SimpleTriange

extern void Demos::SimpleTriangle() {
    static SimpleTriange::Triangle tri;

    tri.Draw();
    tri.ImguiMenu();
}
