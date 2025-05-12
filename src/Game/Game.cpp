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
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution adist{0.0f, PI * 2.0f};
    std::uniform_real_distribution ddist{0.0f, PI * 0.5f};
    std::uniform_real_distribution odist{0.0f, PI * 0.08f};
    std::uniform_real_distribution rdist{6.0f, 20.0f};
    std::uniform_int_distribution bdist{0, 1};

    for (int i = 0; i < 100; i++) {
        auto model = Model::CreatePrimitive(Model::Primitives::CUBE);
        model->r = rdist(rng);
        model->droll = ddist(rng);
        model->dpitch = ddist(rng);
        model->dyaw = ddist(rng);
        model->dphi = odist(rng);
        model->dtheta = odist(rng);
        model->dchi = odist(rng);
        model->chi = adist(rng);
        model->theta = adist(rng);
        model->phi = adist(rng);
        GameWorld::GetInstance()->AddObject(std::move(model));
    }
    timer.Mark();
}

void Game::Update() {
    const auto dt = timer.Mark() * speedFactor;
    for (auto& cube: GameWorld::GetInstance()->GetAllObjects()) {
        cube->Update(dt);
    }
    // for (auto& obj: GameWorld::GetInstance()->GetAllObjects()) {
    //     if (!obj->playerControlled) {
    //         // DirectX::XMFLOAT2 worldCoords = Camera::ScreenToWorld(0, Mouse::GetY());
    //         // obj->SetTransform({worldCoords.x, worldCoords.y, 0});
    //         // std::cout << "World X: " << obj->position.x << " World Y: " << obj->position.y << " Screen X: " << Mouse::GetX() << " Screen Y: " << Mouse::GetY() << "\n";
    //         obj->Update(dt);
    //         continue;
    //     }
    //
    //     if (Keyboard::IsKeyPressed('W')) obj->linearVelocity.y = 0.05f;
    //     if (Keyboard::IsKeyPressed('S')) obj->linearVelocity.y = -0.05f;
    //     if (!Keyboard::IsKeyPressed('W') && !Keyboard::IsKeyPressed('S')) obj->linearVelocity.y = 0;
    //     obj->AddTransform({obj->linearVelocity.x, obj->linearVelocity.y, 0});
    // }


    // GameWorld::GetInstance()->GetObjectAtIndex(0).SetTransform({-15 + static_cast<float>(Mouse::GetX()) * 0.05f, 15 + static_cast<float>(Mouse::GetY()) * -0.05f, 0});
}

void Game::SpawnControlWindow() {
    if (ImGui::Begin("Simulation Speed")) {
        ImGui::SliderFloat("Speed Factor", &speedFactor, 0.0f, 4.0f);
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::End();
}
