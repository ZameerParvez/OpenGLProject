#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace Renderer;

const std::unordered_map<std::string, const Texture> Texture::GetDefaults() {
    std::unordered_map<std::string, const Texture> tmap {};

    auto t1 = std::make_pair(Texture::DEFAULT_TEXTURE1_NAME, Texture(Texture::DEFAULT_TEXTURE1, TexUnit::T0));
    auto t2 = std::make_pair(Texture::DEFAULT_TEXTURE2_NAME, Texture(Texture::DEFAULT_TEXTURE2, TexUnit::T1));

    tmap.insert(std::move(t1));
    tmap.insert(std::move(t2));

    for (const auto& tex : DEFAULTS) tex.second.Build();

    return tmap;
}

const std::unordered_map<std::string, const Texture> Texture::DEFAULTS = Texture::GetDefaults();

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
    // TODO: this->path.string().c_str(), this isn't a good thing to do, but using c_str() directly doesn't work because of w_char 
    unsigned char* data = stbi_load(this->path.string().c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        // TODO: find out what this is actually doing, look in the opengl core specs
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // to make textures which don't have sizes powers of 2 sizes (or whatever the size has to be) work correctly
        glTexImage2D(textureSize, 0, static_cast<GLenum>(this->imgType), width, height, 0, static_cast<GLenum>(this->imgType), GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(textureSize);
    } else {
        fmt::print(stdout, "Failed to load texture: {}\n", this->path);
    }

    stbi_image_free(data);

    return texId;
}

Texture& Texture::Build() {
    if (this->texId != 0) glDeleteTextures(1, &this->texId);
    fmt::print(stdout, "\n== Building Texture ==\n");
    this->texId = LoadTexture();
    fmt::print(stdout, "{}\n-- Done --\n", *this);
    return *this;
}

const Texture& Texture::Build() const {
    if (this->texId != 0) glDeleteTextures(1, &this->texId);
    fmt::print(stdout, "\n== Building Texture ==\n");
    this->texId = LoadTexture();
    fmt::print(stdout, "{}\n-- Done --\n", *this);
    return *this;
}
