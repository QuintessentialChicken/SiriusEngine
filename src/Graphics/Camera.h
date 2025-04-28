//
// Created by Leon on 07/02/2025.
//

#ifndef CAMERA_H

#define CAMERA_H

#include <DirectXMath.h>

class Camera {
public:
    [[nodiscard]] DirectX::XMMATRIX GetMatrix() const noexcept;
    void SpawnControlWindow() noexcept;
    void Reset() noexcept;
    float yaw = 0.0f;
private:
    float theta = 0.0f;
    float r = 2.5f;
    float phi = 0.0f;
    float pitch = 0.0f;
    float roll = 0.0f;
};


#endif //CAMERA_H

