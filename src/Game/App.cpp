//
// Created by Leon on 26/04/2025.
//
#include "App.h"

#include <iostream>

#include "Game.h"
#include "External/imgui_impl_dx11.h"
#include "Graphics/GfxDevice.h"
#include "Graphics/Plane.h"
#include "GameWorld.h"
#include "Graphics/Cube.h"
#include "Graphics/Renderer.h"
#include "Input/Input.h"
#include "Physics/Physics.h"
Fsm::Return App::UpdateState(const signed short state) {
    switch (state) {
        case INIT_SYSTEM:
            return Init();
        case RUN_GAME:
            return RunGame();
        case SHUTDOWN_SYSTEM:
            return Shutdown();
        default:
            return UNHANDLED;
    }
}

Fsm::Return App::Init() {
    GfxDevice::SetWindowTitle("Fuzzy");
    // GfxDevice::Init();
    Renderer::Init();
    Input::Init();
    for (const auto& fun : startFunctions) {
        fun();
    }
    SetState(RUN_GAME);
    return CONTINUE;
}

Fsm::Return App::Shutdown() {
    // GfxDevice::ShutdownClass();
    Renderer::Shutdown();
    GameWorld::DestroySingleton();
    return EXIT;
}


// ReSharper disable once CppDFAConstantFunctionResult
Fsm::Return App::RunGame() {
    MSG message;
    std::optional<int> exitCode = {};
    if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
        if (message.message == WM_QUIT) {
            exitCode = static_cast<int>(message.wParam);
            std::cout << "Exiting" << std::endl;
        }
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    if (exitCode == 0) {
        SetState(SHUTDOWN_SYSTEM);
        return CONTINUE;
    }
    for (const auto& fun : updateFunctions) {
        fun();
    }
    DoFrame();
    return CONTINUE;
}


void App::DoFrame() {
    Renderer::BeginFrame();
    auto projection = Renderer::GetProjection();
    GameWorld* world = GameWorld::GetInstance();
    // for (auto& light : world->GetAllLightSources()) {
    //     light->Bind();
    //     light->Draw();
    // }
    for (const auto& obj : world->GetAllObjects()) {
        obj->Draw(cam.GetMatrix(), projection);
    }
    cam.SpawnControlWindow();
    // Game::SpawnControlWindow();
    // Physics::SpawnControlWindow();
    Renderer::EndFrame();
}

void App::RegisterInitFunction(void(*fun)()) {
    startFunctions.push_back(fun);

}

void App::RegisterUpdateFunction(void(*fun)()) {
    updateFunctions.push_back(fun);
}

float App::DeltaTime() {
    return timer.Mark();
}

float App::Time() {
    return timer.Total();
}
