#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

#include "data/ImageFile.h"

#include <iostream>

namespace nebula {
    namespace graphics {

class Texture {
public:
    Texture(const char* texturePath);
    ~Texture();
    void use();

private:
    unsigned int id;
};

}// graphics
}// nebula

#endif
