#pragma once

#include <string>
#include <iostream>

#include "openglLoader.h"

namespace Renderer {

enum class ImageDataType {
    RGB = GL_RGB,
    RGBA = GL_RGBA
};

enum class TextureWrapping {
    REPEAT = GL_REPEAT,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER  // TODO: need to make sure border colour can be defined for this
};

enum class TextureFiltering {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};

enum class TextureSize {
    T1D = GL_TEXTURE_1D,
    T2D = GL_TEXTURE_2D,
    T3D = GL_TEXTURE_3D
};

// NOTE: the other texture unit values are offsets from GL_TEXTURE0. When used with a shader, the uniform texture sampler needs the offset of the texture unit, not the actual value here
enum class TexUnit {
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

// TODO: Add a default texture

// NOTE: this only supports 2d textures
class Texture {
   private:
    friend std::ostream& operator<<(std::ostream& os, const Texture& tex);

    GLuint LoadTexture() const;
 

   public:
    inline static const std::string DEFAULT_TEXTURE = "src/renderer/default-resources/textures/default.png";

    mutable GLuint texId = 0;

    std::string path;
    TexUnit texUnit;  // IDEA: should this be private and not changable?
    ImageDataType imgType = ImageDataType::RGBA;
    TextureWrapping sWrap = TextureWrapping::REPEAT;
    TextureWrapping tWrap = TextureWrapping::REPEAT;
    TextureFiltering magFilter = TextureFiltering::LINEAR;
    TextureFiltering minFilter = TextureFiltering::NEAREST_MIPMAP_LINEAR;
    float borderColour[4] = {0, 0, 0, 0}; // TODO: allow changing this in the constructor and the parameter settings function

    Texture(
        const std::string& path = DEFAULT_TEXTURE,
        TexUnit texUnit = TexUnit::T0,
        ImageDataType imgType = ImageDataType::RGBA,
        TextureWrapping sWrap = TextureWrapping::REPEAT,
        TextureWrapping tWrap = TextureWrapping::REPEAT,
        TextureFiltering magFilter = TextureFiltering::LINEAR,
        TextureFiltering minFilter = TextureFiltering::NEAREST_MIPMAP_LINEAR) : path{path}, texUnit{texUnit}, imgType{imgType}, sWrap{sWrap}, tWrap{tWrap}, magFilter{magFilter}, minFilter{minFilter} {}

    ~Texture() {
        if (texId == 0 ) return;
        glDeleteTextures(1, &texId);
    }

    Texture(Texture& tex) = delete;

    Texture(Texture&& tex) {
        this->path = std::move(tex.path);
        this->texId = tex.texId;
        this->texUnit = tex.texUnit;
        this->imgType = tex.imgType;
        this->sWrap = tex.sWrap;
        this->tWrap = tex.tWrap;
        this->magFilter = tex.magFilter;
        this->minFilter = tex.minFilter;

        tex.texId = 0;
    }

    Texture& operator=(Texture&& tex) {
        this->path = std::move(tex.path);
        this->texId = tex.texId;
        this->texUnit = tex.texUnit;
        this->imgType = tex.imgType;
        this->sWrap = tex.sWrap;
        this->tWrap = tex.tWrap;
        this->magFilter = tex.magFilter;
        this->minFilter = tex.minFilter;

        tex.texId = 0;

        return *this;
    }

    void SetTextureParams(
        const std::string& path,
        TexUnit texUnit = TexUnit::T0,
        ImageDataType imgType = ImageDataType::RGBA,
        TextureWrapping sWrap = TextureWrapping::REPEAT,
        TextureWrapping tWrap = TextureWrapping::REPEAT,
        TextureFiltering magFilter = TextureFiltering::LINEAR,
        TextureFiltering minFilter = TextureFiltering::NEAREST_MIPMAP_LINEAR) {
        
        this->path = path;
        this->texUnit = texUnit;
        this->imgType = imgType;
        this->sWrap = sWrap;
        this->tWrap = tWrap;
        this->magFilter = magFilter;
        this->minFilter = minFilter;
    }

    // build and rebuild the Texture
    Texture& Build();
    const Texture& Build() const;

    void Use() const {
        glActiveTexture(static_cast<int>(this->texUnit));
        glBindTexture(GL_TEXTURE_2D, this->texId);
    }
};

std::ostream& operator<<(std::ostream& os, const Texture& tex);

}  // namespace Renderer