//
// Created by Leon on 21/01/2025.
//

#ifndef APP_H
#define APP_H
#include <thread>

#include "Core/Fsm.h"
#include "Core/GDIPlusManager.h"
#include "Core/Timer.h"
#include "Graphics/Camera.h"
#include "Graphics/ImguiManager.h"


enum class PrimitiveType {
    Cube,
    Plane,
    Sphere
    // Add more as needed
};

class App : public Fsm {
public:

    enum {
        INIT_SYSTEM = 0,
        RUN_GAME,
        SHUTDOWN_SYSTEM
    };

    Return UpdateState(signed short state) override;
    // Implement state machine here later to handle Initialization, Game loop and teardown
    // App can inherit from state machine class, RunOneIteration is part of state machine class
    Return Init();

    Return Shutdown();

    Return RunGame();
    void DoFrame();
    void RegisterInitFunction(void (*)());
    void RegisterUpdateFunction(void (*)());
    float DeltaTime();
    float Time();

private:
    std::vector<void (*)()> startFunctions;
    std::vector<void (*)()> updateFunctions;
    bool isSystemInitialized = false;
    Timer timer;
    GDIPlusManager gdipm;
    float speed_factor = 1.0f;
    ImguiManager imgui;
    Camera cam;
};


#endif //APP_H
