#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace nebula {

namespace graphics {

class Shader {
public:
    unsigned int id;

    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void use();
    unsigned int getId();
    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
};

}
}

#endif