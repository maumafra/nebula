#include "Texture.h"

namespace nebula {
    namespace graphics {

Texture::Texture(const char* texturePath) : id(NULL) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width {}, height {}, nrChannels {};
    unsigned char* data {};

    data::ImageFile* textImage = new data::ImageFile(texturePath);
    textImage->read(data, width, height, nrChannels);
    if (!data) {
        std::cout << "ERROR::TEXTURE::LOAD_TEXTURE" << "\n";
    }

    this->width = width;
    this->height = height;

    GLenum internalFormat = GL_RGB;
    if (nrChannels == 4) {
        internalFormat = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    delete textImage;
}
Texture::~Texture(){
}

int Texture::getId() {
    return id;
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, id);
}
}// graphics
}// nebula
