#ifndef CUBE_H
#define CUBE_H

#include <memory>
#include <random>
#include "Model.h"

class Cube {
public:
    explicit Cube();
    
    void Update(float dt);
    void Draw(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection);
    
    // Transform manipulation methods
    void SetPosition(const DirectX::XMFLOAT3& pos);
    void Translate(const DirectX::XMFLOAT3& offset);
    void SetRotation(const DirectX::XMFLOAT3& rot);
    void SetScale(const DirectX::XMFLOAT3& scl);
    
private:
    std::unique_ptr<Model> model;
    
    // For animated rotation
    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float theta = 0.0f;
    float phi = 0.0f;
    float chi = 0.0f;
    float droll = 0.0f;
    float dpitch = 0.0f;
    float dyaw = 0.0f;
    float dtheta = 0.0f;
    float dphi = 0.0f;
    float dchi = 0.0f;
    float r = 0.0f;
    
    // Animation update
    void UpdateTransform();
};

#endif // CUBE_H