//
// Created by Leon on 07/02/2025.
//

#ifndef CAMERA_H
#define CAMERA_H
#include <DirectXMath.h>

#include "Core/Component.h"

class Camera : public Component {
public:
    void Init() override {};
    void Update() override {};


    static DirectX::XMFLOAT2 ScreenToWorld(int x, int y);
    static DirectX::XMFLOAT3 ScreenToWorldPerspective(int x, int y, DirectX::XMMATRIX view, DirectX::XMMATRIX projection);
};


#endif //CAMERA_H

