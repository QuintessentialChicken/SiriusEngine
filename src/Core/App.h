//
// Created by Leon on 21/01/2025.
//

#ifndef APP_H
#define APP_H
#include <random>

#include "Game.h"
#include "GDIPlusManager.h"
#include "Timer.h"
#include "Window.h"
#include "Graphics/Camera.h"
#include "Graphics/Drawable.h"
#include "Graphics/ImguiManager.h"
#include "External/imgui_impl_win32.h"


template<typename GameType>
class App {
    static_assert(std::is_base_of_v<Game, GameType>, "T must derive from Game");

public:
    explicit App(int width = 800, int height = 600, const std::string& title = "Default Title") : wnd{width, height, title.c_str()} {
        game = std::make_unique<GameType>();
    }

    ~App() {
        game.release();
    }

    int Run() {
        game->Start();
        while (true) {
            if (const auto exitCode = Window::ProcessMessage()) {
                return exitCode.value();
            }
            DoFrame();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

private:
    GDIPlusManager gdipm;

    void DoFrame() {
        wnd.GetGraphics().BeginFrame(0.0f, 0.0f, 0.0f);
        wnd.GetGraphics().SetCamera(cam.GetMatrix());

        game->Update();
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

    std::unique_ptr<GameType> game;

    float speed_factor = 1.0f;

    ImguiManager imgui;
    Window wnd;
    Camera cam;
    Timer timer;
    std::vector<std::unique_ptr<Drawable> > drawables;
    static constexpr size_t numDrawables = 1;
};


#endif //APP_H
