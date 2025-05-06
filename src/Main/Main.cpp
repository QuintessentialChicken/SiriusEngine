#include <iostream>
#include <random>

#include "Game/GameWorld.h"
#include "../Game/App.h"
#include "Core/Entrypoint_Engine.h"
#include "Game/Game.h"
#include "Graphics/Cube.h"
#include "Input/Mouse.h"
#include "Physics/Physics.h"

App my_app;


// TODO Eventually move the Register functions to Gameskeleton and call them from Game
// Keep this as empty as possible, it's just glue between the engine and the game
bool Main_Prologue() {
    std::cout << "Prologue: Registering Init Functions" << std::endl;
    my_app.RegisterInitFunction(Game::Init);
    my_app.RegisterInitFunction(Physics::Init);
    std::cout << "Prologue: Registering Update Functions" << std::endl;
    my_app.RegisterUpdateFunction(Game::Update);
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