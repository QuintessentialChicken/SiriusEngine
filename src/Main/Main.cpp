#include <iostream>
#include <random>

#include "GameWorld.h"
#include "Core/App.h"
#include "Core/Entrypoint_Engine.h"
#include "Graphics/Cube.h"
#include "Physics/Physics.h"

App my_app;

void Init() {
    constexpr float LO = -2.0;
    constexpr float HI = 2.0;
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution bdist{LO, HI};
    for (int i = 0; i < 10; i++) {
        float scale = bdist(rng);
        std::unique_ptr<Drawable> cube = std::make_unique<Cube>();
        cube->SetTransform({bdist(rng), bdist(rng), 0.0f});
        cube->SetScale({scale, scale, 0.0f});
        cube->mass = scale;
        cube->inertia = cube->mass * (cube->width * cube->width + cube->height * cube->height) / 12.0f;
        GameWorld::GetInstance()->AddObject(std::move(cube));
    }
}

void Update() {
}

bool Main_Prologue() {
    std::cout << "Prologue" << "\n" << "Registering Init Functions" << std::endl;
    my_app.RegisterInitFunction(Init);
    my_app.RegisterInitFunction(Physics::Init);
    std::cout << "Prologue" << "\n" << "Registering Update Functions" << std::endl;
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