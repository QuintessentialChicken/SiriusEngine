//
// Created by Leon on 07/02/2025.
//

#include "Camera.h"

#include "External/Imgui.h"
#include "WndProc.h"

namespace dx = DirectX;

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

DirectX::XMFLOAT3 Camera::ScreenToWorldPerspective(int x, int y, DirectX::XMMATRIX view, DirectX::XMMATRIX projection) {
    using namespace DirectX;
    float ndcX = (2.0f * static_cast<float>(x) / static_cast<float>(windowWidth)) - 1.0f;
    float ndcY = 1.0f - (2.0f * static_cast<float>(y) / static_cast<float>(windowHeight));
    XMVECTOR nearPoint = XMVectorSet(ndcX, ndcY, 0.0f, 1.0f); // z=0 for near plane
    XMVECTOR farPoint  = XMVectorSet(ndcX, ndcY, 1.0f, 1.0f); // z=1 for far plane

    auto invViewProj = XMMatrixInverse(nullptr, projection * view);

    XMVECTOR worldNear = XMVector4Transform(nearPoint, invViewProj);
    XMVECTOR worldFar  = XMVector4Transform(farPoint,  invViewProj);

    worldNear = XMVectorScale(worldNear, 1.0f / XMVectorGetW(worldNear));
    worldFar  = XMVectorScale(worldFar,  1.0f / XMVectorGetW(worldFar));

    XMVECTOR rayDir = XMVector3Normalize(XMVectorSubtract(worldFar, worldNear));
    XMVECTOR rayOrigin = worldNear;
    float originZ = XMVectorGetZ(rayOrigin);
    float dirZ = XMVectorGetZ(rayDir);
    float t = (10.0f - originZ) / dirZ;
    XMVECTOR hitPoint = XMVectorAdd(rayOrigin, XMVectorScale(rayDir, t));

    XMFLOAT3 worldPos{};
    XMStoreFloat3(&worldPos, hitPoint);
    return worldPos;
}
