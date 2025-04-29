//
// Created by Leon on 26/04/2025.
//
#include "App.h"

#include <iostream>

#include "External/imgui_impl_dx11.h"
#include "Graphics/GfxDevice.h"
#include "Graphics/Plane.h"

bool App::RunOneIteration() {
    // Checks if the state is Exit
    // Inside Update (from finite state machine class) the state is updated and the state evaluation function is called.
    // This function then calls Init, Update, etc. based on the state
    // Update()

    // For now, directly call update as the main game loop
    if (!isInitialized) {
        Init();
        isInitialized = true;
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
    std::unique_ptr<Drawable> obj = std::make_unique<Plane>();
    obj->SetRotation({1.5f, 0.0f, 0.0f});
    drawables.push_back(std::move(obj));
    return true;
}

bool App::Shutdown() {
    GfxDevice::ShutdownClass();
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
    DoFrame();
    return true;
}


void App::DoFrame() {
    GfxDevice::BeginFrame();
    GfxDevice::camera = cam.GetMatrix();
    for (const auto& drawable : drawables) {
        drawable->Draw();
    }

    cam.SpawnControlWindow();
    GfxDevice::EndFrame();
}



float App::DeltaTime() {
    return timer.Mark();
}

float App::Time() {
    return timer.Total();
}
