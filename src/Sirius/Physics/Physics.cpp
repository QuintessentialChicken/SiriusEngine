//
// Created by Leon on 30/04/2025.
//

#include "Physics.h"

#include <iostream>
#include <thread>
#include <array>

#include "External/imgui.h"
#include "Graphics/GfxDevice.h"
#include "../../Game/GameWorld.h"
Timer timer;
bool Physics::applyForce = false;
float Physics::force = 0.0f;
float Physics::privateForce = 0.0f;
float PI = 3.14159265358979323846;
float rads = PI / 180;
float dt = 0.0f;

void Physics::Init() {
}

// TODO Lock down delta time - Create some sort of fixed update
void Physics::Update() {
    dt = timer.Mark();
    const auto& objects = GameWorld::GetInstance()->GetAllObjects();
    for (size_t i = 0; i < objects.size(); ++i) {
        objects[i]->force = {force, 0.0f, 0.0f};
        // DirectX::XMFLOAT2 gravity = CalculateGravity(objects[i]->mass);
        // DirectX::XMFLOAT2 acceleration = {0.0f, gravity.y / objects[i]->mass};
        // objects[i]->linearVelocity.y += acceleration.y * dt;
        // objects[i]->position.y += objects[i]->linearVelocity.y * dt;
        // AngularVelocity(objects[i]);
        for (size_t j = 0; j < objects.size(); j++) {
            if (i == j) continue;
            if (objects[i]->position.x + objects[i]->width > objects[j]->position.x - objects[j]->width &&
                objects[i]->position.x - objects[i]->width < objects[j]->position.x + objects[j]->width &&
                objects[i]->position.y + objects[i]->height > objects[j]->position.y - objects[j]->height &&
                objects[i]->position.y - objects[i]->height < objects[j]->position.y + objects[j]->height
            ) {
                std::cout << "Collision" << std::endl;
                ElasticCollision(std::tie(objects[i], objects[j]));
            }
        }
        LinearVelocity(objects[i]);
        force *= -1;
    }
    force = 0.0f;
}

void Physics::ElasticCollision(std::pair<const std::unique_ptr<Drawable>&, const std::unique_ptr<Drawable>&> objects) {
    const float v1 = ((objects.first->mass - objects.second->mass) / (objects.first->mass + objects.second->mass)) * objects.first->linearVelocity.x
                + (2 * objects.second->mass) / (objects.first->mass + objects.second->mass) * objects.second->linearVelocity.x;
    const float v2 = ((objects.second->mass - objects.first->mass) / (objects.second->mass + objects.first->mass)) * objects.second->linearVelocity.x
                + (2 * objects.first->mass) / (objects.second->mass + objects.first->mass) * objects.first->linearVelocity.x;
    objects.first->linearVelocity.x = v1;
    objects.second->linearVelocity.x = v2;
}


void Physics::LinearVelocity(const std::unique_ptr<Drawable>& object) {
    DirectX::XMFLOAT2 linearAcceleration = {object->force.x / object->mass, object->force.y / object->mass};
    object->linearVelocity.x += linearAcceleration.x * dt;
    object->linearVelocity.y += linearAcceleration.y * dt;
    object->position.x += object->linearVelocity.x * dt;
    object->position.y += object->linearVelocity.y * dt;
}

void Physics::AngularVelocity(const std::unique_ptr<Drawable>& object) {
    CalculateTorque(object);
    const float angularAcceleration = object->torque / object->inertia;
    object->angularVelocity += angularAcceleration * dt;
    object->rotation.z += object->angularVelocity * dt;
}
// Mass cancels out
DirectX::XMFLOAT2 Physics::CalculateGravity(const float mass) {
    return {0.0f, -9.81f * mass};
}

void Physics::CalculateTorque(const std::unique_ptr<Drawable>& object) {
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