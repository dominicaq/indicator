#include "window.h"

// Constructor and Destructor
Window::Window(const char *title, unsigned int width, unsigned int height)
    : title(title), width(width), height(height), m_window(nullptr) {}

Window::~Window() {
    cleanupImGui();
    glfwTerminate();
}

/*
 * Initialization
 */
bool Window::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << "\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << "\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << "\n";
        return false;
    }

    return true;
}

/*
 * ImGui Initialization and Support
 */
bool Window::initImGui() {
    // Verify window is initialized
    if (!m_window) {
        std::cerr << "Cannot initialize ImGui: Window not created" << std::endl;
        return false;
    }

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    return true;
}

void Window::beginImGuiFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Window::renderImGui() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::cleanupImGui() {
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

/*
 * Input Handling
 */
void Window::processInput() {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, true);
    }
}

/*
 * Window State
 */
bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

/*
 * Rendering
 */
void Window::swapBuffersAndPollEvents() {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

/*
 * Window Management
 */
void Window::setTitle(const char *newTitle) {
    title = newTitle;
    glfwSetWindowTitle(m_window, title);
}

void Window::resize(unsigned int newWidth, unsigned int newHeight) {
    width = newWidth;
    height = newHeight;
    glfwSetWindowSize(m_window, width, height);
    glViewport(0, 0, width, height);
}

void Window::getSize(int &width, int &height) const {
    glfwGetWindowSize(m_window, &width, &height);
}

// Static callback for when the window is resized
void Window::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
