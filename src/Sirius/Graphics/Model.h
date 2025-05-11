// Model.h
#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"

class Model {
public:
    Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);

    void Draw(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection);

    // Expose transform for manipulation
    Transform& GetTransform();

private:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    Transform transform;
    std::unique_ptr<TransformBuffer> transformBuffer;
};


#endif // MODEL_H
