//
// Created by Leon on 21/01/2025.
//

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
    const float t = timer.Peek();
    std::ostringstream ss;
    ss << t << "\n";
    wnd.SetTitle(ss.str());
}
