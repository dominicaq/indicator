#include "sprite_renderer.h"
#include <glad/glad.h>

SpriteRenderer::SpriteRenderer(Shader& shader) : m_shader(shader) {
    InitRenderData();
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void SpriteRenderer::AddSprite(const Sprite& sprite) {
    m_sprites.push_back(sprite);
}

void SpriteRenderer::Render() {
    // m_shader.Use();
    glBindVertexArray(m_VAO);

    for (const auto& sprite : m_sprites) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(sprite.transform.position, 0.0f));
        model = glm::rotate(model, glm::radians(sprite.transform.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(sprite.transform.scale, 1.0f));

        // m_shader.SetMat4("model", model);
        // sprite.texture.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glBindVertexArray(0);
}

void SpriteRenderer::InitRenderData() {
    // Quad to draw textures on
    float vertices[] = {
        // Pos      // Tex
        0.0f, 1.0f,  0.0f, 1.0f,
        1.0f, 0.0f,  1.0f, 0.0f,
        0.0f, 0.0f,  0.0f, 0.0f,

        0.0f, 1.0f,  0.0f, 1.0f,
        1.0f, 1.0f,  1.0f, 1.0f,
        1.0f, 0.0f,  1.0f, 0.0f
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
