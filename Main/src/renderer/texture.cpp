#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace Renderer;

GLuint Texture::LoadTexture() const {
    constexpr GLenum textureSize = static_cast<GLenum>(TextureSize::T2D);

    glActiveTexture(static_cast<GLenum>(texUnit));

    GLuint texId = 0;

    glGenTextures(1, &texId);
    glBindTexture(textureSize, texId);

    // set wrapping/filtering options on the currently bound texture
    glTexParameteri(textureSize, GL_TEXTURE_WRAP_S, static_cast<GLenum>(this->sWrap));
    glTexParameteri(textureSize, GL_TEXTURE_WRAP_T, static_cast<GLenum>(this->tWrap));
    glTexParameteri(textureSize, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(this->minFilter));
    glTexParameteri(textureSize, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(this->magFilter));

    if (sWrap == TextureWrapping::CLAMP_TO_BORDER || tWrap == TextureWrapping::CLAMP_TO_BORDER) {
        // NOTE: this may not work correctly if the colour type had more colours, because border colour might not be the correct size
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, this->borderColour);
    }

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);  // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load(this->path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(textureSize, 0, static_cast<GLenum>(this->imgType), width, height, 0, static_cast<GLenum>(this->imgType), GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(textureSize);
    } else {
        std::cout << "Failed to load texture: " << this->path << std::endl;
    }

    stbi_image_free(data);

    return texId;
}

Texture& Texture::Build() {
    if (this->texId != 0) glDeleteTextures(1, &this->texId);
    std::cout << "\n== Building Texture =="
              << std::endl;
    this->texId = LoadTexture();
    std::cout << *this << "\n-- Done --" << std::endl;
    return *this;
}

const Texture& Texture::Build() const {
    if (this->texId != 0) glDeleteTextures(1, &this->texId);
    std::cout << "\n== Building Texture =="
              << std::endl;
    this->texId = LoadTexture();
    std::cout << *this << "\n-- Done --" << std::endl;
    return *this;
}

std::ostream& Renderer::operator<<(std::ostream& os, const Texture& tex) {
    os << "Texture ID: " << tex.texId << "\n"
       << "   Texture Unit (offset): " << static_cast<int>(tex.texUnit) - static_cast<int>(TexUnit::T0) << "\n"
       << "   Texture Path: " << tex.path;

    return os;
}