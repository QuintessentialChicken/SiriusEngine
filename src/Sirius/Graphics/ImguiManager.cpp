//
// Created by Leon on 07/02/2025.
//

#include "ImguiManager.h"
#include "External/imgui.h"

ImguiManager::ImguiManager() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
}

ImguiManager::~ImguiManager() {
    ImGui::DestroyContext();
}
