//
// Created by Leon on 07/02/2025.
//

#include "Camera.h"
#include "External/Imgui.h"

namespace dx = DirectX;

DirectX::XMMATRIX Camera::GetMatrix() const noexcept {
    const auto pos = dx::XMVector3Transform(
        dx::XMVectorSet(0.0f, 0.0f, -r, 0.0f),
        dx::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f)
    );
    return dx::XMMatrixLookAtLH(
               pos, dx::XMVectorZero(),
               dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
           ) * dx::XMMatrixRotationRollPitchYaw(
               pitch, -yaw, roll
           );
}

void Camera::SpawnControlWindow() noexcept {
    if (ImGui::Begin("Camera")) {
        ImGui::Text("Position");
        ImGui::SliderFloat("R", &r, 0.1f, 80.0f, "%.1f");
        ImGui::SliderAngle("Theta", &theta, -180.0f, 180.0f);
        ImGui::SliderAngle("Phi", &phi, -89.0f, 89.0f);
        ImGui::Text("Orientation");
        ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
        ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
        ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
        if (ImGui::Button("Reset")) {
            Reset();
        }
    }
    ImGui::End();
}

DirectX::XMFLOAT2 Camera::ScreenToWorld(int x, int y) {
    int zoom = 1;
    float aspectRatio = 800.0f / 600.0f;
    float viewHeight = 5.0f;
    float viewWidth = viewHeight * aspectRatio;
    float ndcX = ((2.0f * x) / 800) - 1.0f;
    float ndcY = 1.0f - ((2.0f * y) / 600);

    float worldX = ndcX * (viewWidth / 2.0f) * zoom;
    float worldY = ndcY * (viewHeight / 2.0f) * zoom;
    return {worldX, worldY};
}

void Camera::Reset() noexcept {
    r = 20.0f;
    theta = 0.0f;
    phi = 0.0f;
    pitch = 0.0f;
    yaw = 0.0f;
    roll = 0.0f;
}
