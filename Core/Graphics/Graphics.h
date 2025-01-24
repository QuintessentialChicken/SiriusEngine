//
// Created by Lms on 24/01/2025.
//

#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <d3d11.h>


class Graphics {
public:
    explicit Graphics(HWND hWnd);

    ~Graphics();

    Graphics(const Graphics &) = delete;

    Graphics &operator=(Graphics &) = delete;

    void EndFrame();

    void ClearBuffer(float r, float g, float b) noexcept;
private:
    ID3D11Device* device;
    IDXGISwapChain* swapChain;
    ID3D11DeviceContext* context;
    ID3D11RenderTargetView* target;
};


#endif //GRAPHICS_H
