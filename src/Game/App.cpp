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
#include "Graphics/Renderer.h"
#include "Input/Input.h"
#include "Input/Mouse.h"

static bool spawnWindow = false;

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
    Renderer::Init();
    Input::Init();
    cam = std::make_unique<Model>(nullptr, nullptr);
    cam->AddComponent(std::make_unique<Camera>());

    for (const auto& fun : startFunctions) {
        fun();
    }
    SetState(RUN_GAME);
    return CONTINUE;
}

Fsm::Return App::Shutdown() {
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
    for (auto& light : world->GetLights()) {
        light->Draw(cam->transform.GetMatrix(), projection);
    }
    for (const auto& obj : world->GetAllObjects()) {
        obj->Draw(cam->transform.GetMatrix(), projection);
    }
    if (Mouse::RightIsPressed()) {
        DirectX::XMFLOAT3 worldCoords = Camera::ScreenToWorldPerspective(Mouse::GetX(), Mouse::GetY(), cam->transform.GetMatrix(), projection);
        GameWorld::GetInstance()->GetAllObjects()[0]->SetPosition(worldCoords);
        std::cout << worldCoords.x << " " << worldCoords.y << " " << worldCoords.z << "\n";
        // float closestDist = INFINITY;
        // for (auto& obj : world->GetAllObjects()) {
            // obj->SpawnControlWindow();
            // float current;
            // DirectX::XMFLOAT2 vec1 = {obj->transform.position.x, obj->transform.position.y};
            // DirectX::XMVECTOR diff = DirectX::XMVectorSubtract(DirectX::XMLoadFloat2(&vec1), DirectX::XMLoadFloat2(&worldCoords));
            // DirectX::XMStoreFloat(&current,DirectX::XMVector2Length(diff));
            // if (current < closestDist) {
            //     closestDist = current;
            //     closestModel = obj.get();
            // }
        // }
        // if (closestModel != nullptr && closestDist < 7.0f) {
        //     spawnWindow = true;
        // }
    }
    // if (spawnWindow) closestModel->SpawnControlWindow();
    // cam.SpawnControlWindow();
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
