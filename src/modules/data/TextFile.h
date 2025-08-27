#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <string>
#include <fstream>
#include <sstream>

namespace nebula {
    namespace data {

class TextFile {
public:
    TextFile(const char* filePath);
    ~TextFile();
    bool read(std::string& text);
    bool write(const char* text);
private:
    const char* filePath;
};

}// data
}// nebula
#endif