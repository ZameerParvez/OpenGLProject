#include "shader.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>


using namespace Renderer;

struct ShaderCompilationError : public std::runtime_error {
    ShaderCompilationError(const std::string& msg) : std::runtime_error(msg) {}
};

struct ShaderProgramLinkingError : public std::runtime_error {
    ShaderProgramLinkingError(const std::string& msg) : std::runtime_error(msg) {}
};

struct FileReadError : public std::runtime_error {
    FileReadError(const std::string& msg) : std::runtime_error(msg) {}
};

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
        std::ostringstream err;
        err << glErrorMessage << infoLog << std::endl;
        throw ShaderCompilationError(err.str());
    }

    return shader;
}

GLuint Shader::LinkShaders(GLuint vertexShader, GLuint fragmentShader) {
    // must have vertex and fragment shaders
    if (!vertexShader || !fragmentShader) throw ShaderProgramLinkingError("A vertex and fragment shader are necessary for the shader program to be compiled and linked\n");

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
        std::ostringstream err;
        err << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
            << infoLog << std::endl;
        throw ShaderProgramLinkingError(err.str());
    }

    return shaderProgram;
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
        std::ostringstream err;
        err << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\npath:" << filePath << "\n"
            << e.what() << std::endl;
        throw FileReadError(err.str());  // runtime error if shader isn't read succesfully
    }

    return source.str();
}

Shader::Shader(const std::string& vertexShaderPath,
               const std::string& fragmentShaderPath) {
    // read shaders from file
    std::string vertexShader = ReadShaderSource(vertexShaderPath);
    std::string fragmentShader = ReadShaderSource(fragmentShaderPath);

    //compile and link them into a program
    unsigned int vertexShaderID = CompileShader(vertexShader.c_str(), ShaderType::VERTEX);
    unsigned int fragmentShaderID = CompileShader(fragmentShader.c_str(), ShaderType::FRAGMENT);

    // output save the program id and delete the seperate shader objects (keeping only the program)
    this->shaderId = LinkShaders(vertexShaderID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

