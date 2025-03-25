#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    // Read files
    std::string vertexCode;
    std::string fragmentCode;
    int fileStatus = 0;

    fileStatus = loadShaderSource(vertexPath, vertexCode);
    if (fileStatus != 0) {
        std::cerr << "Failed to read vertex shader file" << "\n";
        return;
    }

    fileStatus = loadShaderSource(fragmentPath, fragmentCode);
    if (fileStatus != 0) {
        std::cerr << "Failed to read fragment shader file" << "\n";
        return;
    }

    // Compile, link, and cleanup
    int compileStatus = 0;
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode.c_str(), &compileStatus);
    if (compileStatus != 0) {
        std::cerr << "Vertex shader compilation failed" << "\n";
        return; // Exit if compilation fails
    }

    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str(), &compileStatus);
    if (compileStatus != 0) {
        std::cerr << "Fragment shader compilation failed" << "\n";
        glDeleteShader(vertexShader); // Clean up already compiled shaders
        return;
    }

    int linkStatus = 0;
    linkProgram(vertexShader, fragmentShader, &linkStatus);
    if (linkStatus != 0) {
        std::cerr << "Program linking failed" << "\n";
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
    }

    // Clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Check for OpenGL errors after linking
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << error << "\n";
    }
}

Shader::~Shader() {
    if (glIsProgram(m_ID)) {
        glDeleteProgram(m_ID);
    } else {
        std::cerr << "ERROR::SHADER::PROGRAM_NOT_DELETED: Program ID is not valid" << "\n";
    }
}

/*
* Shader management
*/
void Shader::use() const {
    if (glIsProgram(m_ID)) {
        glUseProgram(m_ID);
    } else {
        // std::cerr << "ERROR::SHADER::USE_FAILED: Program ID is not valid" << "\n";
    }
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
