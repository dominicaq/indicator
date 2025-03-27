#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#ifdef APIENTRY
#undef APIENTRY
#endif

// ImGui headers
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

class Window {
public:
    Window(const char *title, unsigned int width, unsigned int height);
    ~Window();

    /*
     * Initialization
     */
    bool init();
    bool initImGui();

    /*
     * Input Handling
     */
    void processInput();

    /*
     * Window State
     */
    bool shouldClose() const;
    GLFWwindow* getGLFWwindow() { return m_window; }

    /*
     * Rendering
     */
    void swapBuffersAndPollEvents();

    /*
     * Window Management
     */
    void setTitle(const char *newTitle);
    void resize(unsigned int newWidth, unsigned int newHeight);
    void getSize(int &width, int &height) const;

    /*
     * ImGui Support
     */
    void beginImGuiFrame();
    void renderImGui();
    void cleanupImGui();

private:
    // GLFW window object
    GLFWwindow *m_window;

    // Window settings
    const char *title;
    unsigned int width;
    unsigned int height;

    // Static callback function for resizing
    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
};

#endif // WINDOW_H
