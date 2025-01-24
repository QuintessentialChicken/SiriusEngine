//
// Created by Leon on 21/01/2025.
//

#include <thread>
#include "App.h"
#include "Window.h"

App::App(): wnd{800, 600, "Default Window"} {
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
    const float c = sin(timer.Peek()) / 2.0f + 0.5f;
    wnd.GetGraphics().ClearBuffer(c, c, 1.0f);
    wnd.GetGraphics().EndFrame();

}
