//
// Created by Leon on 21/01/2025.
//

#include <thread>
#include "App.h"

#include <iostream>

#include "Window.h"
#include "Graphics/Cube.h"
#include "Graphics/Plane.h"

App::Factory::Factory(Graphics& gfx) : gfx{gfx} {
}

std::unique_ptr<Drawable> App::Factory::operator()() {
    return std::make_unique<Cube>(gfx);
}

App::App(const int width, const int height, const std::string& title) : wnd{width, height, title.c_str()} {
    const Factory f{wnd.GetGraphics()};
    drawables.reserve(numDrawables);
    // Generate Drawables using f and appends them to the end of the drawables vector
    // std::generate_n(std::back_inserter(drawables), numDrawables, f);
    // drawables.emplace_back(std::make_unique<Cube>(wnd.GetGraphics()));
    drawables.emplace_back(std::make_unique<Plane>(wnd.GetGraphics()));
    wnd.GetGraphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
    timer = Timer{};
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
    wnd.GetGraphics().ClearBuffer(0.07f, 0.0f, 0.12f);
    // drawables[0]->AddTransform({cos(timer.Peek()) / 30.0f, -sin(timer.Peek() * 1.2f) / 35.0f, 0});
    for (const auto& drawable: drawables) {
        drawable->Draw(wnd.GetGraphics());
    }
    wnd.GetGraphics().EndFrame();
}
