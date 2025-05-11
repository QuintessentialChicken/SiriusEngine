#ifndef MESH_H
#define MESH_H

#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "Buffer.h"



class Mesh {
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

    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);
    
    void Bind() const;
    [[nodiscard]] uint32_t GetIndexCount() const { return indexCount; }
    
    // Factory method for creating a cube mesh
    static std::shared_ptr<Mesh> CreateCube();
    
private:
    std::unique_ptr<IVertexBuffer> vertexBuffer;
    std::unique_ptr<IIndexBuffer> indexBuffer;
    uint32_t indexCount;
};

#endif // MESH_H