//
// Created by Leon on 21/01/2025.
//

#ifndef APP_H
#define APP_H
#include <random>

#include "Timer.h"
#include "Window.h"
#include "Graphics/Camera.h"
#include "Graphics/Drawable.h"
#include "Graphics/ImguiManager.h"


class App {
public:
    class Factory {
    public:
        explicit Factory(Graphics& gfx);

        std::unique_ptr<Drawable> operator()();

    private:
        Graphics& gfx;
        std::mt19937 rng{std::random_device{}()};
        std::uniform_real_distribution<float> adist{0.0f, 3.1415f * 2.0f};
        std::uniform_real_distribution<float> ddist{0.0f, 3.1415f * 0.5f};
        std::uniform_real_distribution<float> odist{0.0f, 3.1415f * 0.08f};
        std::uniform_real_distribution<float> rdist{6.0f, 20.0f};
        std::uniform_real_distribution<float> bdist{1.0f, 20.0f};
        std::uniform_int_distribution<> typedist{0, 2};
    };

    explicit App(int width = 800, int height = 600, const std::string& title = "Default Title");

    int Run();

private:
    void DoFrame();
    float speed_factor = 1.0f;

    ImguiManager imgui;
    Window wnd;
    Camera cam;
    Timer timer;
    std::vector<std::unique_ptr<Drawable> > drawables;
    static constexpr size_t numDrawables = 1;
};


#endif //APP_H
