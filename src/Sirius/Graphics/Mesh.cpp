//
// Created by Leon on 11/05/2025.
//

#include "Mesh.h"

#include "Renderer.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
    : indexCount(static_cast<uint32_t>(indices.size())) {
    vertexBuffer = Renderer::CreateVertexBuffer(
        vertices.data(),
        vertices.size() * sizeof(Vertex),
        sizeof(Vertex)
    );

    indexBuffer = Renderer::CreateIndexBuffer(
        indices.data(),
        indices.size() * sizeof(uint16_t)
    );
}

void Mesh::Bind() const {
    vertexBuffer->Bind();
    indexBuffer->Bind();
}

std::shared_ptr<Mesh> Mesh::CreateCube() {
    constexpr float side = 0.5f;

    // std::vector<Vertex> vertices = {
    //     // Front face
    //     {{ -side, -side, -side }, { 0.0f, 0.0f, -1.0f }},
    //     {{ side, -side, -side }, { 0.0f, 0.0f, -1.0f }},
    //     {{ -side, side, -side }, { 0.0f, 0.0f, -1.0f }},
    //     {{ side, side, -side }, { 0.0f, 0.0f, -1.0f }},
    //
    //     // Back face
    //     {{ -side, -side, side }, { 0.0f, 0.0f, 1.0f }},
    //     {{ side, -side, side }, { 0.0f, 0.0f, 1.0f }},
    //     {{ -side, side, side }, { 0.0f, 0.0f, 1.0f }},
    //     {{ side, side, side }, { 0.0f, 0.0f, 1.0f }},
    //
    //     // Left face
    //     {{ -side, -side, -side }, { -1.0f, 0.0f, 0.0f }},
    //     {{ -side, side, -side }, { -1.0f, 0.0f, 0.0f }},
    //     {{ -side, -side, side }, { -1.0f, 0.0f, 0.0f }},
    //     {{ -side, side, side }, { -1.0f, 0.0f, 0.0f }},
    //
    //     // Right face
    //     {{ side, -side, -side }, { 1.0f, 0.0f, 0.0f }},
    //     {{ side, side, -side }, { 1.0f, 0.0f, 0.0f }},
    //     {{ side, -side, side }, { 1.0f, 0.0f, 0.0f }},
    //     {{ side, side, side }, { 1.0f, 0.0f, 0.0f }},
    //
    //     // Bottom face
    //     {{ -side, -side, -side }, { 0.0f, -1.0f, 0.0f }},
    //     {{ side, -side, -side }, { 0.0f, -1.0f, 0.0f }},
    //     {{ -side, -side, side }, { 0.0f, -1.0f, 0.0f }},
    //     {{ side, -side, side }, { 0.0f, -1.0f, 0.0f }},
    //
    //     // Top face
    //     {{ -side, side, -side }, { 0.0f, 1.0f, 0.0f }},
    //     {{ side, side, -side }, { 0.0f, 1.0f, 0.0f }},
    //     {{ -side, side, side }, { 0.0f, 1.0f, 0.0f }},
    //     {{ side, side, side }, { 0.0f, 1.0f, 0.0f }}
    // };


    // // Indices for a cube (6 faces, 2 triangles per face, 3 indices per triangle)
    // std::vector<uint16_t> indices = {
    //     0, 2, 1,    2, 3, 1,    // Front face
    //     4, 5, 7,    4, 7, 6,    // Back face
    //     8, 10, 9,   10, 11, 9,  // Left face
    //     12, 13, 15, 12, 15, 14, // Right face
    //     16, 17, 18, 18, 17, 19, // Bottom face
    //     20, 23, 21, 20, 22, 23  // Top face
    // };

    std::vector<Vertex> vertices {
            {{-0.5f, -0.5f, -0.5f}},
            {{0.5f, -0.5f, -0.5f}},
            {{-0.5f, 0.5f, -0.5f}},
            {{0.5f, 0.5f, -0.5f}},
            {{-0.5f, -0.5f, 0.5f}},
            {{0.5f, -0.5f, 0.5f}},
            {{-0.5f, 0.5f, 0.5f}},
            {{0.5f, 0.5f, 0.5f}}
    };
    const std::vector<unsigned short> indices{
        0, 2, 1, 2, 3, 1,
        1, 3, 5, 3, 7, 5,
        2, 6, 3, 3, 6, 7,
        4, 5, 7, 4, 7, 6,
        0, 4, 2, 2, 4, 6,
        0, 1, 4, 1, 5, 4
    };


    return std::make_shared<Mesh>(vertices, indices);
}