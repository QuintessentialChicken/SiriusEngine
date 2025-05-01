//
// Created by Leon on 30/04/2025.
//

#include "Physics.h"

#include <iostream>
#include <thread>
#include <array>

#include "External/imgui.h"
#include "Graphics/GfxDevice.h"
#include "Main/GameWorld.h"
Timer timer;
float runningSum = 0.0f;
bool Physics::applyForce = false;
float Physics::force = 0.0f;
float Physics::privateForce = 0.0f;
float PI = 3.14159265358979323846;
float rads = PI / 180;
void Physics::Init() {
}

// TODO Lock down delta time - Create some sort of fixed update
void Physics::Update() {
    auto dt = timer.Mark();
    const auto& objects = GameWorld::GetInstance()->GetAllObjects();
    for (size_t i = 0; i < objects.size(); ++i) {
        // DirectX::XMFLOAT2 gravity = CalculateGravity(objects[i]->mass);
        // DirectX::XMFLOAT2 acceleration = {0.0f, gravity.y / objects[i]->mass};
        // objects[i]->linearVelocity.y += acceleration.y * dt;
        // objects[i]->position.y += objects[i]->linearVelocity.y * dt;
        DirectX::XMFLOAT2 linearAcceleration = {objects[i]->force.x / objects[i]->mass, objects[i]->force.y / objects[i]->mass};
        objects[i]->linearVelocity.x += linearAcceleration.x * dt;
        objects[i]->linearVelocity.y += linearAcceleration.y * dt;
        objects[i]->position.x += objects[i]->linearVelocity.x * dt;
        objects[i]->position.y += objects[i]->linearVelocity.y * dt;
        CalculateTorque(objects[i]);
        // float angularAcceleration = objects[i]->torque / objects[i]->inertia;
        // objects[i]->angularVelocity += angularAcceleration * dt;
        // objects[i]->rotation.z += objects[i]->angularVelocity * dt;
        for (size_t j = 0; j < objects.size(); j++) {
            if (i == j) continue;
            if (objects[i]->position.x + objects[i]->width > objects[j]->position.x - objects[j]->width &&
                objects[i]->position.x - objects[i]->width < objects[j]->position.x + objects[j]->width &&
                objects[i]->position.y + objects[i]->height > objects[j]->position.y - objects[j]->height &&
                objects[i]->position.y - objects[i]->height < objects[j]->position.y + objects[j]->height
            ) {
                std::cout << "Collision" << std::endl;
                runningSum += dt * 2;
                GfxDevice::SetBackgroundColor({std::sin(runningSum) * 0.3f, std::cos(runningSum) * 0.3f, -std::sin(runningSum) * 0.3f, 1.0f});
            }
        }
        force *= -1;
    }
    force = 0.0f;
}

// Mass cancels out
DirectX::XMFLOAT2 Physics::CalculateGravity(const float mass) {
    return {0.0f, -9.81f * mass};
}

void Physics::CalculateTorque(const std::unique_ptr<Drawable>& object) {
    object->force = {force, 0.0f, 0.0f};
    DirectX::XMFLOAT2 hitOffset = {1, 1};//{object->position.x / 2, object->position.y / 2};
    object->torque = (hitOffset.x * object->force.y - hitOffset.y * object->force.x) * 1;
}

void Physics::SpawnControlWindow() noexcept {
    if (ImGui::Begin("Physics")) {
        ImGui::SliderFloat("Force", &privateForce, -100.0f, 100.0f, "%.1f");
        if (ImGui::Button("Apply force")) {
            force = privateForce;
        }
        if (ImGui::Button("Reset")) {
            force = 0.0f;
            for (auto& object : GameWorld::GetInstance()->GetAllObjects()) {
                object->force = {0.0f, 0.0f, 0.0f};
                object->torque = 0.0f;
                object->angularVelocity = 0;
                object->linearVelocity = {0.0f, 0.0f, 0.0f};
                object->SetTransform(object->initialPosition);
            }
        }
    }
    ImGui::End();
}