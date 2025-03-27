#include "sprite_renderer.h"
#include <glad/glad.h>

SpriteRenderer::SpriteRenderer(Shader& shader, int width, int height) : m_shader(shader) {
    m_viewportWidth = width;
    m_viewportHeight = height;
    initRenderer();
}

SpriteRenderer::~SpriteRenderer() {
    // Cleanup vertex array and buffer objects
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void SpriteRenderer::addSprite(Sprite* sprite) {
    if (sprite) {
        m_sprites.push_back(sprite);
    }
}

void SpriteRenderer::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    m_shader.use();
    glBindVertexArray(m_VAO);

    for (const auto& sprite : m_sprites) {
        if (!sprite || !sprite->texture || !sprite->renderSprite) {
            continue; // Ignore this sprite
        }

        glm::mat4 model = glm::mat4(1.0f);

        // First, translate the sprite to the origin (its center), rotate, then translate back to the original position
        model = glm::translate(model, glm::vec3(sprite->transform.position, 0.0f));  // Translate to position (top-left)
        model = glm::translate(model, glm::vec3(sprite->transform.scale.x / 2.0f, sprite->transform.scale.y / 2.0f, 0.0f)); // Translate to center
        model = glm::rotate(model, glm::radians(sprite->transform.rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // Apply rotation around the center
        model = glm::translate(model, glm::vec3(-sprite->transform.scale.x / 2.0f, -sprite->transform.scale.y / 2.0f, 0.0f)); // Translate back to the original position
        model = glm::scale(model, glm::vec3(sprite->transform.scale, 1.0f)); // Apply scaling

        m_shader.setMat4("projection", m_projection);
        m_shader.setMat4("model", model);
        sprite->texture->bind(0);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glBindVertexArray(0);
}

void SpriteRenderer::initRenderer() {
    // Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup viewport properties
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, m_viewportWidth, m_viewportHeight);
    m_projection = glm::ortho(0.0f, (float)m_viewportWidth, (float)m_viewportHeight, 0.0f, -1.0f, 1.0f);

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

    // Create mesh buffers
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
