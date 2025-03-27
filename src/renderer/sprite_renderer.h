#pragma once

#include "shader.h"
#include "texture.h"
#include "sprite.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SpriteRenderer {
public:
    SpriteRenderer(Shader& shader, int width, int height);
    ~SpriteRenderer();

    void addSprite(const Sprite& sprite);
    void render();

private:
    // Rendering
    std::vector<Sprite> m_sprites;
    Shader& m_shader;
    unsigned int m_VAO, m_VBO;
    void initRenderer();

    // Viewport
    glm::mat4 m_projection;
    int m_viewportWidth, m_viewportHeight;
};
