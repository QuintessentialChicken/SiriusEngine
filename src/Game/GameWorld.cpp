//
// Created by Leon on 30/04/2025.
//

#include "GameWorld.h"
#include <random>

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

void GameWorld::AddObject(Model object) {
    if (object.GetComponent<PointLight>()) lights.push_back(object);
    else objects.push_back(std::move(object));
}

std::vector<Model>& GameWorld::GetLights() {
    return lights;
}

Model& GameWorld::GetObjectAtIndex(int index) {
    return objects.at(index);
}

std::vector<Model>& GameWorld::GetAllObjects() {
    return objects;
}
