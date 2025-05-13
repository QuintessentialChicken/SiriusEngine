// Transform.h
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <DirectXMath.h>

class Transform {
public:
    Transform() = default;
    
    void SetPosition(const DirectX::XMFLOAT3& pos) { position = pos; UpdateMatrix(); }
    void SetRotation(const DirectX::XMFLOAT3& rot) { rotation = rot; UpdateMatrix(); }
    void SetScale(const DirectX::XMFLOAT3& scl) { scale = scl; UpdateMatrix(); }
    
    void Translate(const DirectX::XMFLOAT3& offset) {
        position.x += offset.x;
        position.y += offset.y;
        position.z += offset.z;
        UpdateMatrix();
    }
    
    void Rotate(const DirectX::XMFLOAT3& angles) {
        rotation.x += angles.x;
        rotation.y += angles.y;
        rotation.z += angles.z;
        UpdateMatrix();
    }
    
    [[nodiscard]] DirectX::XMFLOAT3 GetPosition() const { return position; }
    [[nodiscard]] DirectX::XMFLOAT3 GetRotation() const { return rotation; }
    [[nodiscard]] DirectX::XMFLOAT3 GetScale() const { return scale; }
    
    [[nodiscard]] DirectX::XMMATRIX GetMatrix() const {
        return worldMatrix;
    }
    
    DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
private:
    void UpdateMatrix() {
        worldMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
                     DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
                     DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    }

    DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
    DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
};

#endif // TRANSFORM_H