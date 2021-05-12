#include "demos.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "iostream"

using namespace Renderer;

namespace TexDemo {

enum class TexUnit {
    AUTO,
    T0 = GL_TEXTURE0,
    T1 = GL_TEXTURE1,
    T2 = GL_TEXTURE2,
    T3 = GL_TEXTURE3,
    T4 = GL_TEXTURE4,
    T5 = GL_TEXTURE5,
    T6 = GL_TEXTURE6,
    T7 = GL_TEXTURE7,
    T8 = GL_TEXTURE8,
    T9 = GL_TEXTURE9,
    T10 = GL_TEXTURE10,
    T11 = GL_TEXTURE11,
    T12 = GL_TEXTURE12,
    T13 = GL_TEXTURE13,
    T14 = GL_TEXTURE14,
    T15 = GL_TEXTURE15
};

enum class ImageDataType {
    RGB = GL_RGB,
    RGBA = GL_RGBA
};

struct TexDemo {
    GLuint VAO;
    GLuint tex[2];
    ManagedResources<Shader> shaders;
    const Shader* ShaderInUse = nullptr;

    TexDemo() {
        InitialiseTexDemo();
    }

    ~TexDemo() {
        // delete the VAO
        glDeleteVertexArrays(1, &VAO);
        glDeleteTextures(1, &tex[0]);
        glDeleteTextures(1, &tex[1]);
    }

    // TODO: refactor and encapsulate texture creation

    void LoadImage(unsigned int& texID, const char* filePath, TexUnit unit = TexUnit::AUTO, ImageDataType t = ImageDataType::RGB) {
        if (unit != TexUnit::AUTO) {
            glActiveTexture((GLenum) unit);
        }

        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);

        // set wrapping/filtering options on the currently bound texture 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);  // tell stb_image.h to flip loaded texture's on the y-axis.
        unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)t, width, height, 0, (GLenum)t, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture: " << filePath << std::endl;
        }

        stbi_image_free(data);
    }

    void InitialiseTexDemo() {
        LoadImage(tex[0], "src/resources/textures/container.jpg", TexUnit::T0, ImageDataType::RGB);
        LoadImage(tex[1], "src/resources/textures/awesomeface.png", TexUnit::T2, ImageDataType::RGBA);

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

        shaders.GetRes("textureDemo")->SetUniform("texture1", 0);
        shaders.GetRes("textureDemo")->SetUniform("texture2", 2);
        
        glActiveTexture((GLenum) TexUnit::T0);
        glBindTexture(GL_TEXTURE_2D, tex[0]);

        glActiveTexture((GLenum) TexUnit::T1);
        glBindTexture(GL_TEXTURE_2D, tex[1]);

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
