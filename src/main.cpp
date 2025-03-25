#include "system/window.h"
#include "renderer/shader.h"
#include "renderer/sprite_renderer.h"
#include "renderer/texture.h"
#include "renderer/sprite.h"

#include <string>
#include <glm/gtc/matrix_transform.hpp>  // For ortho projection

/*
* Asset paths
*/
static const std::string TEXTURE_DIR = ASSET_DIR "textures/";

int main() {
    // Initialize window
    Window window("Altitude Indicator", 800, 600);
    if (!window.init()) {
        return -1;
    }

    // Enable depth test (not needed for 2D rendering, but might be useful for future)
    glEnable(GL_DEPTH_TEST);

    // Set up the viewport after window initialization
    glViewport(0, 0, 800, 600);

    // Vertex and fragment shaders
    // TODO: Compile and embed the shader code
    const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 TexCoord;

    uniform mat4 model;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
        TexCoord = aTexCoord;
    }
    )";

    const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    in vec2 TexCoord;

    uniform sampler2D spriteTexture;

    void main()
    {
        FragColor = texture(spriteTexture, TexCoord);
    }
    )";

    // Create the sprite shader using the embedded source code
    Shader spriteShader(vertexShaderSource, fragmentShaderSource);

    // Load texture for sprite
    std::string texturePath = TEXTURE_DIR + "sprite.png";
    Texture spriteTexture(texturePath);

    // Create SpriteRenderer
    SpriteRenderer spriteRenderer(spriteShader);

    // Create sprite and set properties
    Sprite sprite;

    // Set up the orthographic projection (2D)
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

    // Variables for delta time calculation
    float deltaTime = 0.0f;  // Time between current frame and last frame
    float lastFrame = 0.0f;

    // Render loop
    while (!window.shouldClose()) {
        // Calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        window.processInput();

        // Clear screen and depth buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Use sprite shader and set uniform for projection matrix
        spriteShader.use();
        spriteShader.setMat4("projection", projection);

        // Render the sprite(s)
        spriteRenderer.Render();

        window.swapBuffersAndPollEvents();
    }

    return 0;
}
