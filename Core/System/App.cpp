//
// Created by Leon on 21/01/2025.
//

#include <thread>
#include "App.h"
#include "Window.h"

App::App(): wnd{800, 600, "Sirius Game Engine"} {
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
    wnd.GetGraphics().ClearBuffer(1, 1, 1);
    wnd.GetGraphics().DrawTestTriangle(45, wnd.mouse.GetPosX() / 400.0f - 1.0f, -wnd.mouse.GetPosY() / 300.0f + 1.0f);
    wnd.GetGraphics().DrawTestTriangle(45, 0.0f, 0.0f);
    wnd.GetGraphics().EndFrame();
    
}
