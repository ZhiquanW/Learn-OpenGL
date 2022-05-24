#include "dawn_ui_manager.h"

namespace dawn_engine {

DawnUIManager::DawnUIManager(GLFWwindow *glfwWindow) {
    const char *glsl_version = "#version 430";
    IMGUI_CHECKVERSION();
    // Setup Dear ImGui context
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.Fonts->AddFontFromFileTTF("../deps/imgui/misc/fonts/DroidSans.ttf", 32);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGuiStyle *style = &ImGui::GetStyle();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}
DawnUIManager::~DawnUIManager() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
} // namespace dawn_engine