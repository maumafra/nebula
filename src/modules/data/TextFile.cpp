#include "TextFile.h"

namespace nebula {
    namespace data {

TextFile::TextFile(const char* filePath) : filePath(filePath){
}
TextFile::~TextFile() {
}

bool TextFile::read(std::string& text) {
    std::ifstream fStream {};
    fStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        fStream.open(filePath);
        std::stringstream sStream {};
        sStream << fStream.rdbuf();
        fStream.close();
        text = sStream.str();
    } catch(std::ifstream::failure e) {
        return false;
    }
    return true;
};

bool TextFile::write(const char* text) {
    //TODO
    return true;
}

}// data
}// nebula
