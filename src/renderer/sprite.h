#pragma once

#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform {
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;

    Transform(glm::vec2 pos = glm::vec2(0.0f), glm::vec2 scl = glm::vec2(1.0f), float rot = 0.0f)
        : position(pos), scale(scl), rotation(rot) {}
};

struct Sprite {
    Transform transform;
    Texture texture;

    Sprite(const Texture& tex, const Transform& trans = Transform())
        : texture(tex), transform(trans) {}
};
