#ifndef IMAGEFILE_H
#define IMAGEFILE_H

namespace nebula {
    namespace data {

class ImageFile {
public:
    ImageFile(const char* filePath);
    ~ImageFile();
    void read(unsigned char*& data, int& width, int& height, int& nrChannels);
private:
    const char* filePath;
    unsigned char* data;
};

}// data
}// nebula

#endif