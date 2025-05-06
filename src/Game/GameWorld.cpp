//
// Created by Leon on 30/04/2025.
//

#include "GameWorld.h"

#include <random>

#include "Graphics/Cube.h"
#include "Graphics/Drawable.h"

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

void GameWorld::AddObject(std::unique_ptr<Drawable> object) {
    objects.push_back(std::move(object));
}

Drawable& GameWorld::GetObjectAtIndex(int index) const {
    return *objects.at(index);
}

std::vector<std::unique_ptr<Drawable>>& GameWorld::GetAllObjects() {
    return objects;
}
