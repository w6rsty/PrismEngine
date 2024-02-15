#pragma once

#include "imgui.h"
#include "imgui/palette.hpp"

namespace prism {

static void DrawWelcomePanel() {
    ImGuiIO& io = ImGui::GetIO();
    auto& headline = io.Fonts->Fonts[1];


    ImVec2 windowCenter = ImGui::GetMainViewport()->GetCenter();
    ImVec2 panelSize = ImVec2{ ImGui::GetWindowSize().x * 0.5f, ImGui::GetWindowSize().y * 0.5f };
    ImGui::SetNextWindowSize(panelSize);
    ImGui::SetNextWindowPos(windowCenter, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse 
        | ImGuiWindowFlags_NoResize 
        | ImGuiWindowFlags_NoMove 
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoDocking;
    ImGui::Begin("Welcome Panel", nullptr, windowFlags);

    ImGui::PushFont(headline);
    ImGui::PushStyleColor(ImGuiCol_Text, GruvboxTheme(GruvboxPalette::Blue));
    ImGui::Text("Prism Engine Editor");
    ImGui::PopStyleColor();
    ImGui::PopFont();

    ImGui::Separator();

    ImGui::BulletText("`File > New`\t to create a new scene.");
    ImGui::BulletText("`File > Open`\t to open an existing scene.");

    ImGui::End();
}

} // namespace prism