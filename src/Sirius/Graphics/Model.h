// Model.h
#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"

class Model {
public:
    enum class Primitives {
        CUBE
    };

    Model(std::unique_ptr<Mesh> mesh, std::unique_ptr<Material> material);

    static std::unique_ptr<Model> CreatePrimitive(Primitives primitive);

    void Draw(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection);
    void SetPosition(const DirectX::XMFLOAT3& pos);
    void Translate(const DirectX::XMFLOAT3& offset);
    void SetRotation(const DirectX::XMFLOAT3& rot);
    void SetScale(const DirectX::XMFLOAT3& scl);
    void Update(float dt);

    // Expose transform for manipulation

    Transform& GetTransform();

private:
    void UpdateTransform();


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
    std::unique_ptr<Mesh> mesh;
    std::unique_ptr<Material> material;
    Transform transform;
    std::unique_ptr<TransformBuffer> transformBuffer;
};


#endif // MODEL_H
