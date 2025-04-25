//
// Created by Leon on 08/02/2025.
//

#include "Pong.h"

#include "Core/App.h"
class Cube;

void Pong::Start() {
    for (float i = 0; i < 100; i++) {
        drawable.emplace_back(&App<Pong>::Instantiate(PrimitiveType::Cube));
        drawable.back()->SetTransform({0, 0, i});
    }
    timer = Timer{};
}


void Pong::Update() {
    float deltaTime = timer.Peek();
    float offset = 0.1;
    for (auto& drawable : drawable) {
        drawable->SetTransform({std::cos(2 * deltaTime - offset), std::sin(4 * deltaTime - offset), drawable->position.z});
        offset += 0.3;
    }
}

