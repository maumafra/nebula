#include "Shader.h"

namespace nebula {
    namespace graphics {

Shader::Shader(const char* vertexPath, const char* fragmentPath)
        : id(NULL) {
    std::string vertexCode {};
    std::string fragmentCode {};

    data::TextFile* vertexFile = new data::TextFile(vertexPath);
    data::TextFile* fragFile = new data::TextFile(fragmentPath);

    if(!vertexFile->read(vertexCode)) {
        std::cout << "ERROR::SHADER::VERT::READ_FILE" << "\n";
    }
    if(!fragFile->read(fragmentCode)) {
        std::cout << "ERROR::SHADER::FRAG::READ_FILE" << "\n";
    }

    delete vertexFile;
    delete fragFile;

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex {}, fragment {};
    int success {};
    char infoLog[512] {};

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog << "\n";
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog << "\n";
    }

    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    glGetShaderiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED" << infoLog << "\n";
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(id);
}

void Shader::use() {
    glUseProgram(id);
}

unsigned int Shader::getId() {
    return id;
}

void Shader::setBool(const std::string &name, bool value) {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setInt(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, float x, float y) const { 
    glUniform2f(glGetUniformLocation(id, name.c_str()), x, y); 
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const { 
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z); 
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const { 
    glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w); 
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

} // graphics
} // nebula
