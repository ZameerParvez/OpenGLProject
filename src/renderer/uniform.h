#pragma once

#include <string>

#include "shader.h"
#include "texture.h"

namespace Renderer {

// NOTE: the shader program must be currently in use for the uniform to be updated
struct Uniform {
    static void Set(const Shader& shader, const std::string& name, bool value) {
        glUniform1i(glGetUniformLocation(shader.shaderId, name.c_str()), static_cast<int>(value));
    }

    static void Set(const Shader& shader, const std::string& name, int value) {
        glUniform1i(glGetUniformLocation(shader.shaderId, name.c_str()), value);
    }

    static void Set(const Shader& shader, const std::string& name, float value) {
        glUniform1f(glGetUniformLocation(shader.shaderId, name.c_str()), value);
    }

    static void Set(const Shader& shader, const std::string& name, const Texture& tex) {
        // NOTE: the uniform texture sampler needs the texture units index, not the actual value of the texture unit
        Uniform::Set(shader, name, static_cast<int>(tex.texUnit) - static_cast<int>(TexUnit::T0));
        tex.Use();
    }

};

}
