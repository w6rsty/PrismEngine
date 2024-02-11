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

    io.Fonts->AddFontFromFileTTF("../../editor/assets/fonts/NotoSansMono/NotoSansMono-Bold.ttf", 24.0f);
    io.FontDefault = io.Fonts->AddFontFromFileTTF("../../editor/assets/fonts/NotoSansMono/NotoSansMono-Regular.ttf", 24.0f);

    SetDarkThemeColors();

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

void ImGuiLayer::OnEvent(Event& event) {
    if (m_Blockevents) {
        ImGuiIO& io = ImGui::GetIO();
        event.m_Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
        event.m_Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
    }
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

void ImGuiLayer::SetDarkThemeColors() {
    ImGuiStyle& style = ImGui::GetStyle();
    auto& colors = style.Colors;
    static ImVec4 gruvbox_5_purple= ImVec4{ 0xb1/255.0f, 0x62/255.0f, 0x86/255.0f, 1.0f };

    colors[ImGuiCol_WindowBg] = ImVec4{ 0x1d/255.0f, 0x20/255.0f, 0x21/255.0f, 1.0f };

    colors[ImGuiCol_Button] = ImVec4{ 0x42/255.0f, 0x42/255.0f, 0x42/255.0f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0x42/255.0f, 0x42/255.0f, 0x42/255.0f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0x42/255.0f, 0x42/255.0f, 0x42/255.0f, 1.0f };

    colors[ImGuiCol_Header] = ImVec4{ 0x83/255.0f, 0xa5/255.0f, 0x98/255.0f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0x2f/255.0f, 0xa5/255.0f, 0x77/255.0f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0x83/255.0f, 0xa5/255.0f, 0x98/255.0f, 1.0f };

    colors[ImGuiCol_Tab] = ImVec4{ 0x28/255.0f, 0x28/255.0f, 0x28/255.0f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0x28/255.0f, 0x28/255.0f, 0x28/255.0f, 1.0f }    ;
    colors[ImGuiCol_TabActive] = ImVec4{ 0x28/255.0f, 0x28/255.0f, 0x28/255.0f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0x28/255.0f, 0x28/255.0f, 0x28/255.0f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0x28/255.0f, 0x28/255.0f, 0x28/255.0f, 1.0f };

    colors[ImGuiCol_TitleBg] = ImVec4{ 0x83/255.0f, 0xa5/255.0f, 0x98/255.0f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0x2f/255.0f, 0xa5/255.0f, 0x77/255.0f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = gruvbox_5_purple;

    colors[ImGuiCol_FrameBg] = ImVec4{ 0x42/255.0f, 0x42/255.0f, 0x42/255.0f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0x42/255.0f, 0x42/255.0f, 0x42/255.0f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0x42/255.0f, 0x42/255.0f, 0x42/255.0f, 1.0f };

    colors[ImGuiCol_ResizeGrip] = ImVec4{ 0x45/255.0f, 0x85/255.0f, 0x88/255.0f, 1.0f };
    colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0x45/255.0f, 0x85/255.0f, 0x88/255.0f, 1.0f };
    colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0x45/255.0f, 0x85/255.0f, 0x88/255.0f, 1.0f };
    
    colors[ImGuiCol_PopupBg] = ImVec4{ 0x28/255.0f, 0x28/255.0f, 0x28/255.0f, 1.0f };
    colors[ImGuiCol_CheckMark] = gruvbox_5_purple;
    colors[ImGuiCol_DockingPreview] = ImVec4{ 0x45/255.0f, 0x85/255.0f, 0x88/255.0f, 1.0f };
    
    // shape
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;
    style.WindowRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.ChildRounding = 4.0f;
    style.PopupRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Right;
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.DisplaySafeAreaPadding = ImVec2(4.0f, 4.0f);
    style.DisplayWindowPadding = ImVec2(4.0f, 4.0f);
    style.DisplaySafeAreaPadding = ImVec2(4.0f, 4.0f);
}

} // namespace prism