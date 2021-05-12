#pragma once

#include <string>

#include "openglLoader.h"

namespace Renderer {

enum class ShaderType {
    VERTEX,
    FRAGMENT
};

// TODO: Improve useability by allowing statically defined shaders and shadermanagers in global scope (although it probably shouldn't be used like that) (this would likely require the glfw context and glad loader functions to have their state to not be set from main, and be callable from anywhere)
class Shader {
   private:
    mutable GLuint shaderId = 0;

    static std::string ReadShaderSource(const std::string& filePath);
    static GLuint CompileAndLink(const Shader& shader);

   public:
    // returns a reference to a compiled shader (does not manage the deletion of the shader)
    static GLuint CompileShader(const char* shaderSource, ShaderType type);
    // currently returns the reference to the compiled shader, and compiles vertex
    static GLuint LinkShaders(GLuint vertexShader, GLuint fragmentShader);

    // NOTE: These shaders are in the renderer's source directory and are coppied to the cmake build directory when built. CMAKE can't track updates to these and other shaders, so when they are update make sure to touch the cmake file for the main project before building
    inline static const std::string DEFAULT_VERTEX_SHADER = "src/renderer/default-resources/shaders/defaultVertex.vs";
    inline static const std::string DEFAULT_FRAGMENT_SHADER = "src/renderer/default-resources/shaders/defaultFragment.fs";

    std::string vsPath;
    std::string fsPath;

    Shader(
        const std::string& vertexShaderPath = DEFAULT_VERTEX_SHADER,
        const std::string& fragmentShaderPath = DEFAULT_FRAGMENT_SHADER) : vsPath{vertexShaderPath}, fsPath{fragmentShaderPath} {}

    // cannot copy shader object because it wouldn't link together a new shader program, so they would just refer to the same program object
    Shader(Shader& s) = delete;

    Shader(Shader&& shader) {
        this->shaderId = shader.shaderId;
        this->vsPath = std::move(shader.vsPath);
        this->fsPath = std::move(shader.fsPath);
        shader.shaderId = 0;
    }

    ~Shader() {
        if (shaderId == 0) return;
        glDeleteProgram(shaderId);
    }

    // build and rebuild the whole shader program, with the current values of the shader paths
    Shader& Build();
    const Shader& Build() const;

    void Use() const {
        glUseProgram(shaderId);
    }

    // NOTE: the shader program must be currently in use for the uniform to be updated
    void SetUniform(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(shaderId, name.c_str()), static_cast<int>(value));
    }

    void SetUniform(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(shaderId, name.c_str()), value);
    }

    void SetUniform(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(shaderId, name.c_str()), value);
    }

    static const Shader* GetDefault() {
        static const Shader defaultShader(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
        static bool exists = false;
        if (!exists) {
            defaultShader.Build();
            exists = true;
        }

        return &defaultShader;
    }
};

}  // namespace Renderer
