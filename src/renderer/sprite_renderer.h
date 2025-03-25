#pragma once

#include "shader.h"
#include "texture.h"
#include "sprite.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SpriteRenderer {
public:
    SpriteRenderer(Shader& shader);
    ~SpriteRenderer();

    void AddSprite(const Sprite& sprite);
    void Render();

private:
    Shader& m_shader;
    std::vector<Sprite> m_sprites;
    unsigned int m_VAO, m_VBO;

    void InitRenderData();
};
