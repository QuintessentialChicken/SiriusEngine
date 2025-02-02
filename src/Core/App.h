//
// Created by Leon on 21/01/2025.
//

#ifndef APP_H
#define APP_H
#include "Timer.h"
#include "Window.h"
#include "Graphics/Box.h"


class App {
public:
    App();

    int Run();

private:
    void DoFrame();

    Window wnd;
    Timer timer;
    std::vector<std::unique_ptr<Box>> boxes;
};


#endif //APP_H
