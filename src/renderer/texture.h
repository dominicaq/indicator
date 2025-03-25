#pragma once

#include <glad/glad.h>
#include <string>

class Texture {
public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(unsigned int unit = 0) const;
    void Unbind() const;

private:
    unsigned int m_ID;
    int m_Width, m_Height, m_Channels;
};
