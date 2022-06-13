#include "core/dawn_ui_system.h"

namespace dawn_engine {

    DawnUISystem::DawnUISystem(int glslVersion) : glslVersion(glslVersion) {
        IMGUI_CHECKVERSION();
        // Setup Dear ImGui context
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        io.Fonts->AddFontFromFileTTF("../deps/imgui/misc/fonts/DroidSans.ttf", 32);
        // Setup Dear ImGui style
        ImGui::StyleColorsLight();
        ImGuiStyle *style = &ImGui::GetStyle();
    }

    DawnUISystem::~DawnUISystem() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void DawnUISystem::initialize(GLFWwindow *glfwWindow) {
        std::string glsl_version = fmt::format("#version {}", this->glslVersion);
        ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
        ImGui_ImplOpenGL3_Init(glsl_version.c_str());
    }

    void DawnUISystem::start(DawnEngine *enginePtr) {}

    void DawnUISystem::preUpdate() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void DawnUISystem::postUpdate() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void DawnUISystem::update(DawnEngine *enginePtr) {}

    void DawnUISystem::render(DawnEngine *enginePtr) {
        this->preUpdate();
        this->update(enginePtr);
        this->postUpdate();
    }

} // namespace dawn_engine