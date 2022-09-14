#pragma once

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/std.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <filesystem>

#include "systems/config.h"
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

    inline static const std::filesystem::path DEFAULT_VERTEX_SHADER = System::DEFAULT_SHADER_PATH / "defaultVertex.vs";
    inline static const std::filesystem::path DEFAULT_FRAGMENT_SHADER = System::DEFAULT_SHADER_PATH / "defaultFragment.fs";
    inline static const std::string DEFAULT_SHADER_PROGRAM_NAME = "default-shader";

    std::filesystem::path vsPath;
    std::filesystem::path fsPath;

    Shader(
        const std::filesystem::path& vertexShaderPath = DEFAULT_VERTEX_SHADER,
        const std::filesystem::path& fragmentShaderPath = DEFAULT_FRAGMENT_SHADER) : vsPath{vertexShaderPath}, fsPath{fragmentShaderPath} {}

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

    friend struct fmt::formatter<Shader>;
};

}  // namespace Renderer

template <> struct fmt::formatter<Renderer::Shader> : fmt::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const Renderer::Shader& sh, FormatContext& ctx) -> decltype(ctx.out()) {
        return format_to(
            ctx.out(),
            "\nShader ID: {}\n\tVertex Path: {}\n\tFragment Path: {}",
            sh.shaderId,
            sh.vsPath,
            sh.fsPath
        );
    }
};
