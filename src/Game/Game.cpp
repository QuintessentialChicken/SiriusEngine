//
// Created by Leon on 06/05/2025.
//

#include "Game.h"

#include <iostream>
#include <random>
#include <External/imgui.h>

#include "GameWorld.h"
#include "Graphics/Camera.h"
#include "Graphics/Cube.h"
#include "Graphics/PointLight.h"
#include "Graphics/Sphere.h"
#include "Graphics/WndProc.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Physics/Physics.h"

class Camera;
class Drawable;

Timer Game::timer;
float Game::speedFactor = 1.0f;

void Game::Init() {
    // GameWorld::GetInstance()->AddLightSource(std::make_unique<PointLight>());
    // for (int i = 0; i < 100; i++) {
    //     GameWorld::GetInstance()->AddObject(std::make_unique<Cube>());
    // }
    std::make_unique<Cube>();
    timer.Mark();
}

void Game::Update() {
    const auto dt = timer.Mark() * speedFactor;

    for (auto& obj: GameWorld::GetInstance()->GetAllObjects()) {
        if (!obj->playerControlled) {
            // DirectX::XMFLOAT2 worldCoords = Camera::ScreenToWorld(0, Mouse::GetY());
            // obj->SetTransform({worldCoords.x, worldCoords.y, 0});
            // std::cout << "World X: " << obj->position.x << " World Y: " << obj->position.y << " Screen X: " << Mouse::GetX() << " Screen Y: " << Mouse::GetY() << "\n";
            obj->Update(dt);
            continue;
        }

        if (Keyboard::IsKeyPressed('W')) obj->linearVelocity.y = 0.05f;
        if (Keyboard::IsKeyPressed('S')) obj->linearVelocity.y = -0.05f;
        if (!Keyboard::IsKeyPressed('W') && !Keyboard::IsKeyPressed('S')) obj->linearVelocity.y = 0;
        obj->AddTransform({obj->linearVelocity.x, obj->linearVelocity.y, 0});
    }


    // GameWorld::GetInstance()->GetObjectAtIndex(0).SetTransform({-15 + static_cast<float>(Mouse::GetX()) * 0.05f, 15 + static_cast<float>(Mouse::GetY()) * -0.05f, 0});
}

void Game::SpawnControlWindow() {
    if (ImGui::Begin("Simulation Speed")) {
        ImGui::SliderFloat("Speed Factor", &speedFactor, 0.0f, 4.0f);
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::End();
}
