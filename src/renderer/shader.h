#pragma once

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class Shader {
public:
    Shader(const std::string& vertexCode, const std::string& fragmentCode);
    ~Shader();

    void use() const;
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;

private:
    unsigned int m_ID;

    unsigned int compileShader(unsigned int type, const char* source, int *status);
    void linkProgram(unsigned int vertexShader, unsigned int fragmentShader, int *status);
    int loadShaderSource(const std::string& path, std::string& sourceCode);
};
