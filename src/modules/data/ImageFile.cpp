#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "ImageFile.h"

namespace nebula {
    namespace data {

ImageFile::ImageFile(const char* filePath) 
            : filePath(filePath)
            , data(nullptr) {
}
ImageFile::~ImageFile() {
    stbi_image_free(data);
}

void ImageFile::read(unsigned char*& data, int& width, int& height, int& nrChannels) {
    data = stbi_load(filePath, &width, &height, &nrChannels, 0);
    this->data = data;
}

}// data
}// nebula
