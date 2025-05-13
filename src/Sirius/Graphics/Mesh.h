#ifndef MESH_H
#define MESH_H

#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "Buffer.h"
#include "Core/Component.h"

enum class Primitives;

class Mesh : public Component {
public:
    struct Vertex {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT3 n;
        struct
        {
            float u;
            float v;
        } tex;
    };

    void Init() override {}

    void Update() override {}

    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);

    void Bind() const;
    [[nodiscard]] uint32_t GetIndexCount() const { return indexCount; }
    
    // Factory method for creating a cube mesh
    static std::unique_ptr<Mesh> CreateCube();
    static std::unique_ptr<Mesh> CreateSphere();

private:
    std::unique_ptr<IVertexBuffer> vertexBuffer;
    std::unique_ptr<IIndexBuffer> indexBuffer;
    uint32_t indexCount;
};

#endif // MESH_H