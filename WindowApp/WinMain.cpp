#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <sstream>
#include <Windows.h>

#include "System/App.h"
#include "System/SiriusException.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    try {
        return App{}.Run();
    } catch (const SiriusException &e) {
        MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    } catch (const std::exception &e) {
        MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    } catch (...) {
        MessageBox(nullptr, "No details available", "Unknown exception", MB_OK | MB_ICONEXCLAMATION);
    }
}
