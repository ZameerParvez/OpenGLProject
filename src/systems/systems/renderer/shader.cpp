#include "shader.h"

#include <fstream>
#include <sstream>
#include <stdio.h>

using namespace Renderer;

const std::unordered_map<std::string, const Shader> Shader::GetDefaults() {
    std::unordered_map<std::string, const Shader> smap {};

    auto s1 = std::make_pair(Shader::DEFAULT_SHADER_PROGRAM_NAME, Shader());

    smap.insert(std::move(s1));

    for (const auto& tex : DEFAULTS) tex.second.Build();

    return smap;
}

const std::unordered_map<std::string, const Shader> Shader::DEFAULTS = Shader::GetDefaults();

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
        fmt::print(stdout, "{} {}", glErrorMessage, infoLog);
    }

    return shader;
}

GLuint Shader::LinkShaders(GLuint vertexShader, GLuint fragmentShader) {
    // must have vertex and fragment shaders
    if (!vertexShader || !fragmentShader) fmt::print(
        stdout,
        "A vertex and fragment shader are necessary for the shader program to be compiled and linked\n"
    );

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
        fmt::print(
            stdout,
            "ERROR::SHADER::PROGRAM::LINKING_FAILED\n{}\n",
            infoLog
        );
    }

    return shaderProgram;
}

GLuint Shader::CompileAndLink(const Shader& shader) {
    // output the program id and delete the seperate shader objects (keeping only the program)

    // read shaders from file
    std::string vertexShader = ReadShaderSource(shader.vsPath.string());
    std::string fragmentShader = ReadShaderSource(shader.fsPath.string());

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
        fmt::print(
            stdout,
            "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\npath: {}\n{}\n",
            filePath,
            e.what()
        );
    }

    return source.str();
}

Shader& Shader::Build() {
    if (shaderId != 0) glDeleteProgram(shaderId);
    fmt::print(stdout, "\n== Building Shader ==\n");
    shaderId = CompileAndLink(*this);
    fmt::print(stdout, "{}\n-- Done --\n", *this);
    return *this;
}

const Shader& Shader::Build() const {
    if (shaderId != 0) glDeleteProgram(shaderId);
    fmt::print(stdout, "\n== Building Shader ==\n");
    shaderId = CompileAndLink(*this);
    fmt::print(stdout, "{}\n-- Done --\n", *this);
    return *this;
}
