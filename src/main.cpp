#include "system/window.h"
#include "renderer/shader.h"
#include "renderer/sprite_renderer.h"
#include "renderer/texture.h"
#include "renderer/sprite.h"

#include <string>
#include <glm/glm.hpp>
#include <windows.h>
#include <iostream>
#include <vector>

/*
* Window Properties
*/
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400

#define INDICATOR_PX_SIZE 350

// Function to render the right panel
void renderRightPanel(float& pitch, float& roll, bool& showStationary) {
    ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT));
    ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH * 3/4.0f, 0));

    ImGui::Begin("Attitude Indicator Controls", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse);

    // Control Section
    if (ImGui::CollapsingHeader("Orientation Controls", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SliderFloat("Pitch", &pitch, -40.0f, 40.0f, "%.1f°");
        ImGui::SliderFloat("Roll", &roll, -90.0f, 90.0f, "%.1f°");
    }

    // Visualization Section
    if (ImGui::CollapsingHeader("Display Options", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox("Show Stationary Elements", &showStationary);
    }

    // Current State Section
    if (ImGui::CollapsingHeader("Current State", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Pitch: %.1f°", pitch);
        ImGui::Text("Roll: %.1f°", roll);
        ImGui::Text("Stationary: %s", showStationary ? "Yes" : "No");
    }

    // Reset Button
    ImGui::Separator();
    if (ImGui::Button("Reset to Neutral Position", ImVec2(-1, 0))) {
        pitch = 0.0f;
        roll = 0.0f;
        showStationary = true;
    }

    ImGui::End();
}

std::vector<Sprite*> initeAttitudeSprites(SpriteRenderer& spriteRenderer, float availableWidth, float availableHeight) {
    // Calculate center position based on available space
    glm::vec2 pxScale = glm::vec2(INDICATOR_PX_SIZE, INDICATOR_PX_SIZE);
    glm::vec2 centerPos = glm::vec2(
        availableWidth / 2.0f - pxScale.x / 2.0f,
        availableHeight / 2.0f - pxScale.y / 2.0f
    );

    // Create textures
    Texture* outerTexture = new Texture(ASSET_DIR "outer.png");
    Texture* innerTexture = new Texture(ASSET_DIR "inner.png");
    Texture* centerTexture = new Texture(ASSET_DIR "center.png");
    Texture* topTexture = new Texture(ASSET_DIR "top.png");

    // Create and add sprites to renderer
    Sprite* innerSprite = new Sprite(innerTexture, Transform(centerPos, pxScale));
    spriteRenderer.addSprite(innerSprite);

    Sprite* outerSprite = new Sprite(outerTexture, Transform(centerPos, pxScale));
    spriteRenderer.addSprite(outerSprite);

    Sprite* centerSprite = new Sprite(centerTexture, Transform(centerPos, pxScale));
    spriteRenderer.addSprite(centerSprite);

    Sprite* topSprite = new Sprite(topTexture, Transform(centerPos, pxScale));
    spriteRenderer.addSprite(topSprite);

    // Return pointers to sprites
    return {innerSprite, outerSprite, centerSprite, topSprite};
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#ifdef SHOW_CONSOLE
    AllocConsole();

    // Redirect stdout and stderr to the console
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif

    // Initialize window
    Window window("Attitude Indicator", SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!window.init()) {
        return -1;
    }

    if (!window.initImGui()) {
        return -1;
    }

    // Vertex and fragment shaders (this would normally be embedded)
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
    SpriteRenderer spriteRenderer(spriteShader, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Initialize sprites and get pointers to them
    std::vector<Sprite*> attitudeSprites = initeAttitudeSprites(spriteRenderer, SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT);

    // Panel variables
    float pitch = 0.0f, roll = 0.0f;
    bool showStationary = true;

    // Soft charcoal background color
    glClearColor(0.10f, 0.10f, 0.12f, 1.0f);

    // Render loop
    while (!window.shouldClose()) {
        // Listen for the user to close the window (ESC key)
        window.processInput();
        window.beginImGuiFrame();

        renderRightPanel(pitch, roll, showStationary);

        // Access sprites from the vector
        Sprite* innerSprite = attitudeSprites[0];
        Sprite* outerSprite = attitudeSprites[1];
        Sprite* centerSprite = attitudeSprites[2];
        Sprite* topSprite = attitudeSprites[3];

        // Set indicator properties
        topSprite->renderSprite = showStationary;
        centerSprite->renderSprite = showStationary;

        // Compute rotation matrix for the current roll
        float rollRadians = glm::radians(roll);
        glm::mat2 parentRotationMatrix = glm::mat2(
            glm::cos(rollRadians), glm::sin(rollRadians),
            -glm::sin(rollRadians), glm::cos(rollRadians)
        );

        glm::vec2 localOffset = glm::vec2(0, pitch);
        glm::vec2 globalOffset = parentRotationMatrix * localOffset;
        innerSprite->transform.position = outerSprite->transform.position + globalOffset;

        innerSprite->transform.rotation = roll;
        outerSprite->transform.rotation = roll;

        // Render the sprites
        spriteRenderer.render();
        window.renderImGui();

        // Swap buffers and poll events
        window.swapBuffersAndPollEvents();
    }

    return 0;
}
