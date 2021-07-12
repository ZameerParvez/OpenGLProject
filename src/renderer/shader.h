#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "openglLoader.h"

namespace Renderer {

enum class ShaderType {
    VERTEX,
    FRAGMENT
};

class Shader {
   private:
    mutable GLuint shaderId = 0;

    friend struct Uniform;
    friend std::ostream& operator<<(std::ostream& os, const Shader& sh);

    static const std::unordered_map<std::string, const Shader> GetDefaults();
    static const std::unordered_map<std::string, const Shader> DEFAULTS;

    static GLuint CompileAndLink(const Shader& shader);

   public:
    // returns the the source code of the shader at the given path as a string
    static std::string ReadShaderSource(const std::string& filePath);
    // returns a reference to a compiled shader (does not manage the deletion of the shader)
    static GLuint CompileShader(const char* shaderSource, ShaderType type);
    // currently returns the reference to the compiled shader, and compiles vertex
    static GLuint LinkShaders(GLuint vertexShader, GLuint fragmentShader);

    // NOTE: These shaders are in the renderer's source directory and are coppied to the cmake build directory when built. CMAKE can't track updates to these and other shaders, so when they are update make sure to touch the cmake file for the main project before building
    inline static const std::string DEFAULT_VERTEX_SHADER = "renderer/default-resources/shaders/defaultVertex.vs";
    inline static const std::string DEFAULT_FRAGMENT_SHADER = "renderer/default-resources/shaders/defaultFragment.fs";
    inline static const std::string DEFAULT_SHADER_PROGRAM_NAME = "default-shader";

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

    static const Shader* GetDefault() {
        return &DEFAULTS.find(DEFAULT_SHADER_PROGRAM_NAME)->second;
    }

    static const std::unordered_map<std::string, const Shader>& GetDefaultMap() {
        return Shader::DEFAULTS;
    }
};

std::ostream& operator<<(std::ostream& os, const Shader& sh);

}  // namespace Renderer
