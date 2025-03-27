#include "system/window.h"
#include "renderer/shader.h"
#include "renderer/sprite_renderer.h"
#include "renderer/texture.h"
#include "renderer/sprite.h"

#include <string>
#include <glm/glm.hpp>
#include <windows.h>
#include <iostream>

/*
* Window Properties
*/
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400

// Function to render the right panel
void renderRightPanel() {
    // Set the panel to take up 1/4th of the screen width
    ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT));
    ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH * 3/4.0f, 0));

    ImGui::Begin("Altitude Indicator Controls", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse);

    // Altitude control
    static float altitude = 0.0f;
    ImGui::SliderFloat("Altitude", &altitude, 0.0f, 10000.0f, "%.1f m");

    // Pitch and roll controls
    static float pitch = 0.0f;
    static float roll = 0.0f;
    ImGui::SliderFloat("Pitch", &pitch, -90.0f, 90.0f, "%.1f°");
    ImGui::SliderFloat("Roll", &roll, -180.0f, 180.0f, "%.1f°");

    // Display settings
    ImGui::Separator();
    ImGui::Text("Display Settings");

    static bool showGrid = true;
    ImGui::Checkbox("Show Grid", &showGrid);

    static bool showCompass = true;
    ImGui::Checkbox("Show Compass", &showCompass);

    // Information display
    ImGui::Separator();
    ImGui::Text("Aircraft Information");
    ImGui::Text("Current Altitude: %.1f m", altitude);
    ImGui::Text("Current Pitch: %.1f°", pitch);
    ImGui::Text("Current Roll: %.1f°", roll);

    ImGui::End();
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#ifdef SHOW_CONSOLE
    AllocConsole();

    // Redirect stdout and stderr to the console
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif

    // Initialize window
    Window window("Altitude Indicator", SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!window.init()) {
        return -1;
    }

    // Initialize ImGui
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

    // Create sprite and set properties (this would be the scene code)
    Texture spriteTexture(ASSET_DIR "attitude-indicator.png");
    Sprite sprite(spriteTexture);

    // Time calculation
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Render loop
    while (!window.shouldClose()) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Listen for the user to close the window (ESC key)
        window.processInput();

        // Start ImGui frame
        window.beginImGuiFrame();

        // Render the right panel
        renderRightPanel();

        // Render the sprite(s)
        spriteRenderer.render();

        // Render ImGui
        window.renderImGui();

        // Swap buffers and poll events
        window.swapBuffersAndPollEvents();
    }

    return 0;
}
