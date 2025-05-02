//
// Created by Leon on 26/04/2025.
//
#include "App.h"

#include <iostream>

#include "External/imgui_impl_dx11.h"
#include "Graphics/GfxDevice.h"
#include "Graphics/Plane.h"
#include "../../Game/GameWorld.h"
#include "Physics/Physics.h"

bool App::RunOneIteration() {
    // Checks if the state is Exit
    // Inside Update (from finite state machine class) the state is updated and the state evaluation function is called.
    // This function then calls Init, Update, etc. based on the state
    // Update()

    // For now, directly call update as the main game loop
    if (!isSystemInitialized) {
        Init();
        isSystemInitialized = true;
    }

    if (RunGame() == 0) {
        Shutdown();
        return false;
    }
    return true;
}

bool App::Init() {
    GfxDevice::SetWindowTitle("Fuzzy");
    GfxDevice::InitClass();
    for (const auto& fun : startFunctions) {
        fun();
    }
    return true;
}

bool App::Shutdown() {
    GfxDevice::ShutdownClass();
    GameWorld::DestroySingleton();
    return true;
}


bool App::RunGame() {
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
        return exitCode.value();
    }
    for (const auto& fun : updateFunctions) {
        fun();
    }
    DoFrame();
    return true;
}


void App::DoFrame() {
    GfxDevice::BeginFrame();
    GfxDevice::camera = cam.GetMatrix();
    GameWorld* world = GameWorld::GetInstance();
    for (const auto& obj : world->GetAllObjects()) {
        obj->Draw();
    }
    cam.SpawnControlWindow();
    Physics::SpawnControlWindow();
    GfxDevice::EndFrame();
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
