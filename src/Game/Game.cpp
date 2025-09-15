//
// Created by Leon on 06/05/2025.
//

#include "Game.h"

#include <random>
#include <External/imgui.h>

#include "GameWorld.h"
#include "Graphics/PointLight.h"
#include "Physics/Physics.h"
static constexpr float PI = 3.14159265f;
static constexpr float dtTarget = 16.67f;

class Camera;
class Drawable;

Timer Game::timer;
float Game::speedFactor = 1.0f;

void Game::Init() {
    // for (int i = 0; i < 1; i++) {
    //     auto model = Model::CreatePrimitive(Primitives::CUBE_PHONG);
    //     model->SetPosition({10.0f, 0.0f, 10.0f});
    //     GameWorld::GetInstance()->AddObject(std::move(model));
    // }
    // auto light = Model::CreatePrimitive(Primitives::POINTLIGHT);
    // light->SetPosition({0.0f, 0.0f, 7.0f});
    // GameWorld::GetInstance()->AddObject(std::move(light));
    // timer.Mark();
}

void Game::Update() {
    // const auto dt = timer.Mark() * speedFactor;
    // for (auto& cube: GameWorld::GetInstance()->GetAllObjects()) {
    //     cube->Update(dt);
    // }
}

void Game::SpawnControlWindow() {
    if (ImGui::Begin("Simulation Speed")) {
        ImGui::SliderFloat("Speed Factor", &speedFactor, 0.0f, 4.0f);
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::End();
}
