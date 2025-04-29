//
// Created by Leon on 21/01/2025.
//

#ifndef APP_H
#define APP_H
#include <thread>

#include "GDIPlusManager.h"
#include "Timer.h"
#include "Graphics/Camera.h"
#include "Graphics/Drawable.h"
#include "Graphics/ImguiManager.h"


enum class PrimitiveType {
    Cube,
    Plane,
    Sphere
    // Add more as needed
};

class App {
public:
    // Implement state machine here later to handle Initialization, Game loop and teardown
    // App can inherit from state machine class, RunOneIteration is part of state machine class
    bool RunOneIteration();
    bool Init();
    bool Shutdown();
    bool RunGame();
    void DoFrame();

    float DeltaTime();

    float Time();

private:
    bool isInitialized = false;
    Timer timer;
    GDIPlusManager gdipm;
    float speed_factor = 1.0f;
    ImguiManager imgui;
    Camera cam;
    std::vector<std::unique_ptr<Drawable>> drawables;




};


#endif //APP_H
