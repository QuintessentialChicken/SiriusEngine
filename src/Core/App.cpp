//
// Created by Leon on 21/01/2025.
//

#include <thread>
#include "App.h"
#include "Window.h"
#include "Graphics/Cube.h"

App::Factory::Factory(Graphics& gfx) : gfx{gfx} {
}

std::unique_ptr<Drawable> App::Factory::operator()() {
    // switch (typedist(rng)) {
    //     case 0:
    //         return std::make_unique<Cube>(gfx, adist(rng), ddist(rng), rdist(rng));
    //     case 1:
    //         return std::make_unique<Cube>(gfx, adist(rng), ddist(rng), rdist(rng));
    //     default:
    //         return std::make_unique<Cube>(gfx, adist(rng), ddist(rng), rdist(rng));;
    // }
    return std::make_unique<Cube>(gfx);
    // return std::make_unique<Box>(gfx, rng, adist, ddist, odist, rdist, bdist);
}

App::App(const int width, const int height, const std::string& title) : wnd{width, height, title.c_str()} {
    const Factory f{wnd.GetGraphics()};
    drawables.reserve(numDrawables);
    // Generate Drawables using f and appends them to the end of the drawables vector
    std::generate_n(std::back_inserter(drawables), numDrawables, f);
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
    for (const auto& drawable: drawables) {
        drawable->Update(dt);
        drawable->Draw(wnd.GetGraphics());
    }
    wnd.GetGraphics().EndFrame();
}
