//
// Created by Leon on 21/01/2025.
//

#ifndef APP_H
#define APP_H
#include <iostream>
#include <random>

#include "Game.h"
#include "GDIPlusManager.h"
#include "Timer.h"
#include "Window.h"
#include "Graphics/Camera.h"
#include "Graphics/Drawable.h"
#include "Graphics/ImguiManager.h"
#include "External/imgui_impl_win32.h"
#include "Graphics/Cube.h"
#include "Graphics/Plane.h"

enum class PrimitiveType {
    Cube,
    Plane,
    Sphere
    // Add more as needed
};

template<typename GameType>
class App {
    static_assert(std::is_base_of_v<Game, GameType>, "GameType must derive from Game");

public:
    // Delete copy and move constructors to enforce singleton
    App(const App&) = delete;
    App& operator=(const App&) = delete;
    App(App&&) = delete;
    App& operator=(App&&) = delete;

    // Singleton accessor
    static App& GetInstance(int width = 800, int height = 600, const std::string& title = "Default Title") {
        static App instance(width, height, title);
        return instance;
    }

    int Run() {
        // game->Start();
        while (true) {
            if (const auto exitCode = Window::ProcessMessage()) {
                return exitCode.value();
            }
            DoFrame();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    static Drawable& Instantiate(PrimitiveType type) {
        return GetInstance().InstantiateImpl(type);
    }

    static float DeltaTime() {
        return GetInstance().timer.Mark();
    }

    static float Time() {
        return GetInstance().timer.Total();
    }

private:
    Timer timer;
    GDIPlusManager gdipm;
    float speed_factor = 1.0f;
    ImguiManager imgui;
    Window wnd;
    Camera cam;
    std::vector<std::unique_ptr<Drawable>> drawables;

    // Private constructor (singleton pattern)
    explicit App(int width, int height, const std::string& title)
        : wnd{width, height, title.c_str()} {
        game = std::make_unique<GameType>();
        wnd.GetGraphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
    }

    ~App() = default;

    void DoFrame() {
        wnd.GetGraphics().BeginFrame(0.0f, 0.0f, 0.0f);
        wnd.GetGraphics().SetCamera(cam.GetMatrix());
        game->Update();
        for (const auto& drawable : drawables) {
            drawable->Draw(wnd.GetGraphics());
        }

        // if (ImGui::Begin("Simulation Speed")) {
        //     ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 4.0f);
        //     ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        // }
        // ImGui::End();
        cam.SpawnControlWindow();
        wnd.GetGraphics().EndFrame();
    }

    // Unique game instance
    std::unique_ptr<GameType> game;

    // Private function to instantiate objects
    Drawable& InstantiateImpl(PrimitiveType type) {
        std::unique_ptr<Drawable> obj;

        switch (type) {
            case PrimitiveType::Cube:
                obj = std::make_unique<Cube>(wnd.GetGraphics());
                break;
            case PrimitiveType::Plane:
                obj = std::make_unique<Plane>(wnd.GetGraphics());
                break;
            case PrimitiveType::Sphere:
                // obj = std::make_unique<Sphere>(position);
                break;
            default:
                throw std::runtime_error("Unknown primitive type");
        }
        drawables.push_back(std::move(obj));
        return *drawables.back();
    }
};


#endif //APP_H
