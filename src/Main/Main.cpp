#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>

#include "Pong/Pong.h"
#include "Core/App.h"
#include "Core/SiriusException.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    try {
        return App<Pong>{1000, 600, "Sirius 3D"}.Run();
    } catch (const SiriusException &e) {
        MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    } catch (const std::exception &e) {
        MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    } catch (...) {
        MessageBox(nullptr, "No details available", "Unknown exception", MB_OK | MB_ICONEXCLAMATION);
    }
    return -1;
}

int main() {
    return App<Pong>{1200, 600, "PONG"}.Run();
}