//
// Created by Leon on 06/05/2025.
//

#include "Game.h"

#include <random>

#include "GameWorld.h"
#include "Graphics/Cube.h"
#include "Input/Keyboard.h"
#include "Physics/Physics.h"

class Drawable;


void Game::Init() {
    constexpr float LO = 0.1;
    constexpr float HI = 2.0;
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution bdist{LO, HI};
    std::uniform_real_distribution adist{-5.0f, 5.0f};

    std::unique_ptr<Drawable> cube = std::make_unique<Cube>();
    cube->SetTransform({-3.0f, 0.0f, 0.0f});
    cube->SetRotation({0.0f, 1.5708f * 2, 0.0f});
    cube->SetScale({0.5f, 1.0f, 1.0f});
    cube->initialPosition = cube->position;
    cube->playerControlled = true;
    GameWorld::GetInstance()->AddObject(std::move(cube));

    std::unique_ptr<Drawable> cube2 = std::make_unique<Cube>();
    cube2->SetTransform({3.0f, 0.0f, 0.0f});
    cube2->SetRotation({0.0f, 1.5708f * 2, 0.0f});
    cube2->SetScale({0.5f, 1.0f, 1.0f});
    cube2->initialPosition = cube2->position;
    cube2->playerControlled = true;
    GameWorld::GetInstance()->AddObject(std::move(cube2));

    std::unique_ptr<Drawable> ball = std::make_unique<Cube>();
    ball->physicsEnabled = true;
    ball->SetTransform({0.0f, 0.0f, 0.0f});
    ball->initialPosition = ball->position;
    ball->mass = 0.1f;
    ball->SetScale({0.2f, 0.2f, 0.2f});
    GameWorld::GetInstance()->AddObject(std::move(ball));
}

void Game::Update() {
    for (auto& obj : GameWorld::GetInstance()->GetAllObjects()) {
        if (!obj->playerControlled) continue;
        if (Keyboard::IsKeyPressed('W')) obj->linearVelocity.y = 0.05f;
        if (Keyboard::IsKeyPressed('S')) obj->linearVelocity.y = -0.05f;
        if (!Keyboard::IsKeyPressed('W') && !Keyboard::IsKeyPressed('S')) obj->linearVelocity.y = 0;
        obj->AddTransform({obj->linearVelocity.x, obj->linearVelocity.y, 0});
    }
    // GameWorld::GetInstance()->GetObjectAtIndex(0).SetTransform({-15 + static_cast<float>(Mouse::GetX()) * 0.05f, 15 + static_cast<float>(Mouse::GetY()) * -0.05f, 0});
}
