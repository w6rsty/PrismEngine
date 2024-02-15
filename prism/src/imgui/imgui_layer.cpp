#include "imgui/imgui_layer.hpp"

#include "imgui/palette.hpp"
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

    io.IniFilename = "editor/imgui.ini";

    io.Fonts->AddFontFromFileTTF("editor/assets/fonts/NotoSansMono/NotoSansMono-Bold.ttf", 24.0f);
    // Headline Font
    io.Fonts->AddFontFromFileTTF("editor/assets/fonts/NotoSansMono/NotoSansMono-Bold.ttf", 50.0f);
    io.FontDefault = io.Fonts->AddFontFromFileTTF("editor/assets/fonts/NotoSansMono/NotoSansMono-Regular.ttf", 24.0f);

    SetStyleShape();
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

    colors[ImGuiCol_WindowBg]           = GruvboxTheme(GruvboxPalette::Bg);
    colors[ImGuiCol_WindowBg].w        = 0.8f;

    colors[ImGuiCol_Button]             = GruvboxTheme(GruvboxPalette::Fg4);
    colors[ImGuiCol_ButtonHovered]      = GruvboxTheme(GruvboxPalette::Fg3);
    colors[ImGuiCol_ButtonActive]       = GruvboxTheme(GruvboxPalette::Fg4);

    colors[ImGuiCol_Header]             = GruvboxTheme(GruvboxPalette::Fg4);
    colors[ImGuiCol_HeaderHovered]      = GruvboxTheme(GruvboxPalette::Fg3);
    colors[ImGuiCol_HeaderActive]       = GruvboxTheme(GruvboxPalette::Fg4);

    colors[ImGuiCol_Tab]                = GruvboxTheme(GruvboxPalette::Bg3);
    colors[ImGuiCol_TabHovered]         = GruvboxTheme(GruvboxPalette::Bg3);
    colors[ImGuiCol_TabActive]          = GruvboxTheme(GruvboxPalette::Bg3);
    colors[ImGuiCol_TabUnfocused]       = GruvboxTheme(GruvboxPalette::Bg3);
    colors[ImGuiCol_TabUnfocusedActive] = GruvboxTheme(GruvboxPalette::Bg3);

    colors[ImGuiCol_TitleBg]            = GruvboxTheme(GruvboxPalette::Fg4);
    colors[ImGuiCol_TitleBgActive]      = GruvboxTheme(GruvboxPalette::Fg3);
    colors[ImGuiCol_TitleBgCollapsed]   = GruvboxTheme(GruvboxPalette::Fg4);

    colors[ImGuiCol_FrameBg]            = GruvboxTheme(GruvboxPalette::Bg1);
    colors[ImGuiCol_FrameBgHovered]     = GruvboxTheme(GruvboxPalette::Bg2);
    colors[ImGuiCol_FrameBgActive]      = GruvboxTheme(GruvboxPalette::Bg1);

    colors[ImGuiCol_ResizeGrip]         = GruvboxTheme(GruvboxPalette::Blue);
    colors[ImGuiCol_ResizeGripHovered]  = GruvboxTheme(GruvboxPalette::Blue);
    colors[ImGuiCol_ResizeGripActive]   = GruvboxTheme(GruvboxPalette::Blue);
    
    colors[ImGuiCol_PopupBg]            = GruvboxTheme(GruvboxPalette::Bg1);
    colors[ImGuiCol_CheckMark]          = GruvboxTheme(GruvboxPalette::Blue);
    colors[ImGuiCol_DockingPreview]     = GruvboxTheme(GruvboxPalette::Blue);

    colors[ImGuiCol_SliderGrab]         = GruvboxTheme(GruvboxPalette::Blue);
    colors[ImGuiCol_SliderGrabActive]   = GruvboxTheme(GruvboxPalette::LightBlue);
}

void ImGuiLayer::SetStyleShape() {
    ImGuiStyle& style = ImGui::GetStyle();
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