//
// Created by Leon on 04/05/2025.
//

#ifndef INPUT_H
#define INPUT_H
#include <intsafe.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT InputWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// TODO Implement Control Manager to abstract raw input data from in-game keybindings.
// TODO Include some functionality to map mouse position to screen space coordinates
class Input {
public:
    static void Init();

};



#endif //INPUT_H
