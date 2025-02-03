//
// Created by Leon on 21/01/2025.
//

#include <thread>
#include "App.h"
#include "Window.h"

App::App(): wnd{800, 600, "Sirius Game Engine"} {
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution adist{0.0f, 3.1415f * 2.0f};
    std::uniform_real_distribution ddist{0.0f, 3.1415f * 0.5f};
    std::uniform_real_distribution odist{0.0f, 3.1415f * 0.08f};
    std::uniform_real_distribution rdist{6.0f, 20.0f};
    for (auto i = 0; i < 80; i++) {
        boxes.push_back(std::make_unique<Box>(wnd.GetGraphics(), rng, adist, ddist, odist, rdist));
    }
    wnd.GetGraphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

[[noreturn]] int App::Run() {
    while (true) {
        if (const auto exitCode = Window::ProcessMessage()) {
            return exitCode.value();
        }
        DoFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void App::DoFrame() {
    const auto dt{timer.Mark()};
    wnd.GetGraphics().ClearBuffer(0.07f, 0.0f, 0.12f);
    for (const auto& box: boxes) {
        box->Update(dt);
        box->Draw(wnd.GetGraphics());
    }
    wnd.GetGraphics().EndFrame();
}
