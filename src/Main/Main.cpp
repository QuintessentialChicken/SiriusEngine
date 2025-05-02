#include <iostream>
#include <random>

#include "Game/GameWorld.h"
#include "Core/App.h"
#include "Core/Entrypoint_Engine.h"
#include "Graphics/Cube.h"
#include "Physics/Physics.h"

// TODO Most of this should happen in App which should be part of game, not the engine
// App will inherit from the finite state machine which will be part of the engine
App my_app;

void Init() {
    constexpr float LO = 0.1;
    constexpr float HI = 2.0;
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution bdist{LO, HI};
    std::uniform_real_distribution adist{-5.0f, 5.0f};

    std::unique_ptr<Drawable> cube = std::make_unique<Cube>();
    cube->SetTransform({-2.0f, 0.0f, 0.0f});
    cube->initialPosition = cube->position;
    GameWorld::GetInstance()->AddObject(std::move(cube));
    std::unique_ptr<Drawable> cube2 = std::make_unique<Cube>();
    cube2->SetTransform({2.0f, 0.0f, 0.0f});
    cube2->SetRotation({0.0f, 1.5708f * 2, 0.0f});
    cube2->initialPosition = cube2->position;
    GameWorld::GetInstance()->AddObject(std::move(cube2));
}

void Update() {
}

bool Main_Prologue() {
    std::cout << "Prologue: Registering Init Functions" << std::endl;
    my_app.RegisterInitFunction(Init);
    my_app.RegisterInitFunction(Physics::Init);
    std::cout << "Prologue: Registering Update Functions" << std::endl;
    my_app.RegisterUpdateFunction(Update);
    my_app.RegisterUpdateFunction(Physics::Update);

    return true;
}

bool Main_OneLoopIteration() {
    return my_app.RunOneIteration();
}



SET_APP_ENTRY_POINTS(Main_Prologue, Main_OneLoopIteration)

int Main() {
    assert(false && "Should never reach here, Sirius is using App style entry points");
    return 0;
}