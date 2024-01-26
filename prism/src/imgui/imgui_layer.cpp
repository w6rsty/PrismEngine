#include "imgui/imgui_layer.hpp"

#include "core/application.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

namespace prism {

#define BING_IMGUI_EVENT_FN(x) std::bind(&ImGuiLayer::x, this, std::placeholders::_1)

ImGuiLayer::ImGuiLayer() 
: Layer("ImGuiLayer") {

}

ImGuiLayer::~ImGuiLayer() {
    
}

void ImGuiLayer::OnAttach() {
    PRISM_PROFILE_FUNCTION();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    io.FontDefault = io.Fonts->AddFontFromFileTTF("../../assets/fonts/JetBrainsMonoNerdFontMono-SemiBold.ttf", 24.0f);

    ImGui::StyleColorsDark();

    auto& app = Application::Instance();
    GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
    
}

void ImGuiLayer::OnDetach() {
    PRISM_PROFILE_FUNCTION();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown(); 
    ImGui::DestroyContext();
}

void ImGuiLayer::Begin() {
    PRISM_PROFILE_FUNCTION();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::End() {
    PRISM_PROFILE_FUNCTION();

    ImGuiIO& io = ImGui::GetIO();
    auto& app = Application::Instance();
    io.DisplaySize = ImVec2(static_cast<float>(app.GetWindow().GetWidth()), 
                            static_cast<float>(app.GetWindow().GetHeight()));

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

} // namespace prism