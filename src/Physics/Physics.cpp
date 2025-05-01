//
// Created by Leon on 30/04/2025.
//

#include "Physics.h"

#include <thread>

#include "External/imgui.h"
#include "Main/GameWorld.h"
Timer timer;
bool Physics::applyForce = false;
float Physics::force = 0.0f;
float Physics::privateForce = 0.0f;

void Physics::Init() {
}

// TODO Lock down delta time - Create some sort of fixed update
void Physics::Update() {
    auto dt = timer.Mark();
    for (const auto& obj : GameWorld::GetInstance()->GetAllObjects()) {
        // DirectX::XMFLOAT2 gravity = CalculateGravity(obj->mass);
        // DirectX::XMFLOAT2 acceleration = {0.0f, gravity.y / obj->mass};
        // obj->linearVelocity.y += acceleration.y * dt;
        // obj->position.y += obj->linearVelocity.y * dt;
        DirectX::XMFLOAT2 linearAcceleration = {obj->force.x / obj->mass, obj->force.y / obj->mass};
        obj->linearVelocity.x += linearAcceleration.x * dt;
        obj->linearVelocity.y += linearAcceleration.y * dt;
        obj->position.x += obj->linearVelocity.x * dt;
        obj->position.y += obj->linearVelocity.y * dt;
        CalculateTorque(obj);
        float angularAcceleration = obj->torque / obj->inertia;
        obj->angularVelocity += angularAcceleration * dt;
        obj->rotation.z += obj->angularVelocity * dt;
    }
    force = 0.0f;
}

// Mass cancels out
DirectX::XMFLOAT2 Physics::CalculateGravity(const float mass) {
    return {0.0f, -9.81f * mass};
}

void Physics::CalculateTorque(const std::unique_ptr<Drawable>& object) {
    object->force = {0.0f, force, 0.0f};
    DirectX::XMFLOAT2 hitOffset = {1, 1};//{object->position.x / 2, object->position.y / 2};
    object->torque = (hitOffset.x * object->force.y - hitOffset.y * object->force.x) * 1;
}

void Physics::SpawnControlWindow() noexcept {
    if (ImGui::Begin("Physics")) {
        ImGui::SliderFloat("Force", &privateForce, 0.1f, 100.0f, "%.1f");
        if (ImGui::Button("Apply force")) {
            force = privateForce;
        }
    }
    ImGui::End();
}