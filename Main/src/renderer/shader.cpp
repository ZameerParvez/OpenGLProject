#include "shader.h"

#include <fstream>
#include <sstream>

using namespace Renderer;

// NOTE: This requires the glfw context to be loaded and glad to be loaded, which causes problems with using these functions in global scope
GLuint Shader::CompileShader(const char* shaderSource, ShaderType type) {
    GLuint glShaderType;
    const char* glErrorMessage;

    switch (type) {
        case ShaderType::VERTEX:
            glShaderType = GL_VERTEX_SHADER;
            glErrorMessage = "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
            break;
        case ShaderType::FRAGMENT:
            glShaderType = GL_FRAGMENT_SHADER;
            glErrorMessage = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n";
            break;
        default:
            glShaderType = GL_VERTEX_SHADER;
            glErrorMessage = "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
    }

    // create shader object in gpu, and load shader source into it, then compile
    GLuint shader;
    shader = glCreateShader(glShaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    // check if shader compilation was successful
    GLint success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << glErrorMessage << infoLog << std::endl;
    }

    return shader;
}

GLuint Shader::LinkShaders(GLuint vertexShader, GLuint fragmentShader) {
    // must have vertex and fragment shaders
    if (!vertexShader || !fragmentShader) std::cout << ("A vertex and fragment shader are necessary for the shader program to be compiled and linked\n") << std::endl;

    // link shaders (so outputs and inputs match)
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check if shader linking was successful
    GLint success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    return shaderProgram;
}

GLuint Shader::CompileAndLink(const Shader& shader) {
    // output the program id and delete the seperate shader objects (keeping only the program)

    // read shaders from file
    std::string vertexShader = ReadShaderSource(shader.vsPath);
    std::string fragmentShader = ReadShaderSource(shader.fsPath);

    //compile and link them into a program
    GLuint vertexShaderID = CompileShader(vertexShader.c_str(), ShaderType::VERTEX);
    GLuint fragmentShaderID = CompileShader(fragmentShader.c_str(), ShaderType::FRAGMENT);

    GLuint programId = LinkShaders(vertexShaderID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programId;
}

std::string Shader::ReadShaderSource(const std::string& filePath) {
    std::ifstream shaderFile;
    std::ostringstream source;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shaderFile.open(filePath);
        source << shaderFile.rdbuf();
        shaderFile.close();
    } catch (const std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\npath:" << filePath << "\n"
                  << e.what() << std::endl;
    }

    return source.str();
}

Shader& Shader::Build() {
    if (shaderId != 0) glDeleteProgram(shaderId);
    std::cout << "\n== Building Shader =="
              << std::endl;
    shaderId = CompileAndLink(*this);
    std::cout << *this << "\n-- Done --" << std::endl;
    return *this;
}

const Shader& Shader::Build() const {
    if (shaderId != 0) glDeleteProgram(shaderId);
    std::cout << "\n== Building Shader =="
              << std::endl;
    shaderId = CompileAndLink(*this);
    std::cout << *this << "\n-- Done --" << std::endl;
    return *this;
}

std::ostream& Renderer::operator<<(std::ostream& os, const Shader& sh) {
    os
        << "Shader ID: " << sh.shaderId << "\n"
        << "    Vertex Path: " << sh.vsPath << "\n"
        << "    Fragment Path: " << sh.fsPath;

    return os;
}