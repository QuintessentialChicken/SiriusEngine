//
// Created by Leon on 21/01/2025.
//

#include <thread>
#include "App.h"

#include <iostream>

#include "GDIPlusManager.h"
#include "Window.h"
#include "External/imgui_impl_dx11.h"
#include "External/imgui_impl_win32.h"
#include "Graphics/Cube.h"
#include "Graphics/Plane.h"

GDIPlusManager gdipm;

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
    drawables.emplace_back(std::make_unique<Cube>(wnd.GetGraphics()));
    drawables.emplace_back(std::make_unique<Plane>(wnd.GetGraphics()));
    drawables.emplace_back(std::make_unique<Plane>(wnd.GetGraphics()));
    drawables[1]->SetRotation({1.571f, 0.0f, 0.0f});
    drawables[2]->SetRotation({3.14159f, 0.0f, 0.0f});
    drawables[2]->SetTransform({0.0f, -2.0f, -2.0f});
    // wnd.GetGraphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
    // wnd.GetGraphics().SetProjection(DirectX::XMMatrixOrthographicLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
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
    const auto dt = timer.Mark() * speed_factor;
    wnd.GetGraphics().BeginFrame(0.07f, 0.0f, 0.12f);
    wnd.GetGraphics().SetCamera(cam.GetMatrix());
    // drawables[0]->SetRotation({timer.Peek(), timer.Peek(), 0});
    drawables[0]->SetRotation({dt, dt, dt});

    for (const auto& drawable: drawables) {
        drawable->Draw(wnd.GetGraphics());
    }

    if (ImGui::Begin("Simulation Speed")) {
        ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 4.0f);
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::End();
    cam.SpawnControlWindow();
    wnd.GetGraphics().EndFrame();
}
