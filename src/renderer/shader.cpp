#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    // Read shader sources
    std::string vertexCode = readShaderFile(vertexPath);
    std::string fragmentCode = readShaderFile(fragmentPath);

    if (vertexCode.empty() || fragmentCode.empty()) {
        throw std::runtime_error("Failed to read shader files");
    }

    // Compile shaders
    unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexCode.c_str());
    unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

    // Link program
    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertexShader);
    glAttachShader(m_ID, fragmentShader);
    glLinkProgram(m_ID);

    // Check linking errors
    int success;
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_ID, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
        throw std::runtime_error("Shader program linking failed");
    }

    // Clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

std::string Shader::readShaderFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n";
        std::cerr << "PATH: " << path << "\n";
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int Shader::createShader(GLenum type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Check compilation errors
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << "\n";
        glDeleteShader(shader);
        throw std::runtime_error("Shader compilation failed");
    }

    return shader;
}

Shader::~Shader() {
    if (glIsProgram(m_ID)) {
        glDeleteProgram(m_ID);
    }
}

void Shader::use() const {
    glUseProgram(m_ID);
}

/*
* Uniform data
*/
void Shader::setBool(const std::string& name, bool value) const {
    GLint location = glGetUniformLocation(m_ID, name.c_str());
    if (location != -1) {
        glUniform1i(location, (int)value);
    } else {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << "\n";
    }
}

void Shader::setInt(const std::string& name, int value) const {
    GLint location = glGetUniformLocation(m_ID, name.c_str());
    if (location != -1) {
        glUniform1i(location, value);
    } else {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << "\n";
    }
}

void Shader::setFloat(const std::string& name, float value) const {
    GLint location = glGetUniformLocation(m_ID, name.c_str());
    if (location != -1) {
        glUniform1f(location, value);
    } else {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << "\n";
    }
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    GLint location = glGetUniformLocation(m_ID, name.c_str());
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    } else {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << "\n";
    }
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    GLint location = glGetUniformLocation(m_ID, name.c_str());
    if (location != -1) {
        glUniform3fv(location, 1, glm::value_ptr(value));
    } else {
        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << "\n";
    }
}

/*
* Shader creation
*/
unsigned int Shader::compileShader(unsigned int type, const char* source, int *status) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int compileStatus;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (!compileStatus) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << "\n";
        *status = -1;
    } else {
        *status = 0;
    }

    return shader;
}

void Shader::linkProgram(unsigned int vertexShader, unsigned int fragmentShader, int *status) {
    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertexShader);
    glAttachShader(m_ID, fragmentShader);
    glLinkProgram(m_ID);

    int linkStatus;
    char infoLog[512];
    glGetProgramiv(m_ID, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus) {
        glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
        *status = -1;
    } else {
        *status = 0;
    }
}

int Shader::loadShaderSource(const std::string &path, std::string &sourceCode) {
    std::ifstream shaderFile;
    std::stringstream shaderStream;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(path);
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        sourceCode = shaderStream.str();
        return 0; // Success
    } catch (std::ifstream::failure &e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << e.what() << "\n";
        return -1; // Failure
    }
}
