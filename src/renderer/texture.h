#pragma once

#include <glad/glad.h>
#include <string>

class Texture {
public:
    Texture(const std::string& path);
    ~Texture();

    void bind(unsigned int unit = 0) const;
    void unbind(unsigned int unit = 0) const;

private:
    unsigned int m_ID;
    int m_Width, m_Height, m_Channels;
};
