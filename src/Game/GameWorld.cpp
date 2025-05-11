//
// Created by Leon on 30/04/2025.
//

#include "GameWorld.h"

#include <random>

#include "Graphics/Cube.h"

GameWorld* GameWorld::instance = nullptr;

void GameWorld::InitSingleton() {
    if (instance == nullptr) {
        instance = new GameWorld();
    }
}

void GameWorld::DestroySingleton() {
    delete instance;
}

GameWorld* GameWorld::GetInstance() {
    if (instance == nullptr) {
        InitSingleton();
    }
    return instance;
}

// void GameWorld::AddLightSource(std::unique_ptr<PointLight> light) {
//     lights.push_back(std::move(light));
// }

void GameWorld::AddObject(std::unique_ptr<Cube> object) {
    objects.push_back(std::move(object));
}

// std::vector<std::unique_ptr<PointLight>>& GameWorld::GetAllLightSources() {
//     return lights;
// }

// Drawable& GameWorld::GetObjectAtIndex(int index) const {
//     return *objects.at(index);
// }

std::vector<std::unique_ptr<Cube>>& GameWorld::GetAllObjects() {
    return objects;
}
