//
// Created by Leon on 29/04/2025.
//

#include "Entrypoint_Engine.h"

static int g_MainReturnValue = 0;


static bool CallMainOnce()
{
    g_MainReturnValue = Main();
    return false;
}

// Call Main if game does not assign Entry point
bool (*g_pProjectMainPrologue)() = nullptr;
bool (*g_pProjectMainOrDoOneLoop)() = CallMainOnce;

int main() {
    if (g_pProjectMainPrologue()) {
        bool carryOn = true;
        while (carryOn) {
            carryOn = g_pProjectMainOrDoOneLoop();
        }
    }
    return 0;
}
