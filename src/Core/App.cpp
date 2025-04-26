//
// Created by Leon on 26/04/2025.
//
#include "App.h"

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
    return RunGame();
}

bool App::Init() {
    static Window wnd(800, 600, "Fuzzy");
    window = std::unique_ptr<Window>( &wnd );
    window->GetGraphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
    std::unique_ptr<Drawable> obj = std::make_unique<Cube>(wnd.GetGraphics());
    drawables.push_back(std::move(obj));
    return true;
}

bool App::RunGame() {
    if (const auto exitCode = Window::ProcessMessage()) {
        return exitCode.value();
    }
    Game::Update();
    DoFrame();
    return true;
}


void App::DoFrame() {
    window->GetGraphics().BeginFrame(0.0f, 0.0f, 0.0f);
    window->GetGraphics().SetCamera(cam.GetMatrix());
    for (const auto& drawable : drawables) {
        drawable->Draw(window->GetGraphics());
    }
    cam.SpawnControlWindow();
    window->GetGraphics().EndFrame();
}



float App::DeltaTime() {
    return timer.Mark();
}

float App::Time() {
    return timer.Total();
}
