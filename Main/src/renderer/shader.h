#pragma once

#include <string>
#include <unordered_map>

#include "openglLoader.h"

namespace Renderer {

enum class ShaderType {
    VERTEX,
    FRAGMENT
};

// TODO: Improve useability by allowing statically defined shaders and shadermanagers in global scope (although it probably shouldn't be used like that) (this would likely require the glfw context and glad loader functions to have their state to not be set from main, and be callable from anywhere)
class Shader {
   private:

    unsigned int shaderId;

    static std::string ReadShaderSource(const std::string& filePath);

   public:

    // returns a reference to a compiled shader (does not manage the deletion of the shader)
    static GLuint CompileShader(const char* shaderSource, ShaderType type);
    // currently returns the reference to the compiled shader, and compiles vertex
    static GLuint LinkShaders(GLuint vertexShader, GLuint fragmentShader);

    // NOTE: These shaders are in the renderer's source directory and are coppied to the cmake build directory when built. CMAKE can't track updates to these and other shaders, so when they are update make sure to touch the cmake file for the main project before building
    inline static const std::string DEFAULT_VERTEX_SHADER = "src/renderer/defaultVertex.vs";
    inline static const std::string DEFAULT_FRAGMENT_SHADER = "src/renderer/defaultFragment.fs";

    friend class ShaderManaged;

    Shader(
        const std::string& vertexShaderPath = DEFAULT_VERTEX_SHADER,
        const std::string& fragmentShaderPath = DEFAULT_FRAGMENT_SHADER);

    // cannot copy shader object because it wouldn't link together a new shader program, so they would just refer to the same program object
    Shader(Shader& s) = delete;

    Shader(Shader&& shader) {
        this->shaderId = shader.shaderId;
        shader.shaderId = 0;
    }

    ~Shader() {
        if (shaderId == 0) return;
        glDeleteProgram(shaderId);
    }

    void use() const {
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

    static const Shader* GetDefaultShader() {
        static const Shader defaultShader(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
        return &defaultShader;
    }
};

// TODO: make existing shaders private and make it useable with iterator (expose iterator for pairs in the map) (only affects the use case where you want to iterate over the currently loaded shader programs)
class ShaderManaged {
   public:
    inline static std::string DEFAULT_SHADER = "default";
    std::unordered_map<std::string, Shader> existingShaders;

    ShaderManaged() {}

    ShaderManaged(const ShaderManaged& s) = delete;

    // moves the input shader into this structure (shader manager takes ownership of the object)
    bool AddShader(const std::string& name, Shader&& shader) {
        if (existingShaders.find(name) != existingShaders.end() || name == DEFAULT_SHADER) return false;
        existingShaders.insert({std::move(name), std::move(shader)});
        return true;
    }

    bool DeleteShader(const std::string& name) {
        // cannot remove default shader
        if (existingShaders.find(name) == existingShaders.end() || name == DEFAULT_SHADER) return false;
        existingShaders.erase(name);
        return true;
    }
    
    const Shader* GetProgram(const std::string& name) {
        if (existingShaders.find(name) == existingShaders.end() || name == DEFAULT_SHADER) return Shader::GetDefaultShader();
        return &existingShaders[name];
    }
};

}
