//
// Created by Leon on 11/05/2025.
//

#include "Mesh.h"

#include "Renderer.h"
static constexpr float PI = 3.14159265f;

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

std::unique_ptr<Mesh> Mesh::CreateCube() {
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

    std::vector<Vertex> vertices{
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


    return std::make_unique<Mesh>(vertices, indices);
}

std::unique_ptr<Mesh> Mesh::CreateSphere() {
    namespace dx = DirectX;

    int latDiv = 12;
    int longDiv = 24;
    constexpr float radius = 1.0f;
    const auto base = dx::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
    const float lattitudeAngle = PI / latDiv;
    const float longitudeAngle = 2.0f * PI / longDiv;
    std::vector<Vertex> vertices;

    for (int iLat = 1; iLat < latDiv; iLat++) {
        const auto latBase = dx::XMVector3Transform(
            base,
            dx::XMMatrixRotationX(lattitudeAngle * iLat)
        );
        for (int iLong = 0; iLong < longDiv; iLong++) {
            vertices.emplace_back();
            auto v = dx::XMVector3Transform(
                latBase,
                dx::XMMatrixRotationZ(longitudeAngle * iLong)
            );
            dx::XMStoreFloat3(&vertices.back().pos, v);
        }
    }

    // add the cap vertices
    const auto iNorthPole = static_cast<unsigned short>(vertices.size());
    vertices.emplace_back();
    dx::XMStoreFloat3(&vertices.back().pos, base);
    const auto iSouthPole = static_cast<unsigned short>(vertices.size());
    vertices.emplace_back();
    dx::XMStoreFloat3(&vertices.back().pos, dx::XMVectorNegate(base));

    const auto calcIdx = [latDiv,longDiv](unsigned short iLat, unsigned short iLong) { return iLat * longDiv + iLong; };
    std::vector<unsigned short> indices;
    for (unsigned short iLat = 0; iLat < latDiv - 2; iLat++) {
        for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++) {
            indices.push_back(calcIdx(iLat, iLong));
            indices.push_back(calcIdx(iLat + 1, iLong));
            indices.push_back(calcIdx(iLat, iLong + 1));
            indices.push_back(calcIdx(iLat, iLong + 1));
            indices.push_back(calcIdx(iLat + 1, iLong));
            indices.push_back(calcIdx(iLat + 1, iLong + 1));
        }
        // wrap band
        indices.push_back(calcIdx(iLat, longDiv - 1));
        indices.push_back(calcIdx(iLat + 1, longDiv - 1));
        indices.push_back(calcIdx(iLat, 0));
        indices.push_back(calcIdx(iLat, 0));
        indices.push_back(calcIdx(iLat + 1, longDiv - 1));
        indices.push_back(calcIdx(iLat + 1, 0));
    }

    // cap fans
    for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++) {
        // north
        indices.push_back(iNorthPole);
        indices.push_back(calcIdx(0, iLong));
        indices.push_back(calcIdx(0, iLong + 1));
        // south
        indices.push_back(calcIdx(latDiv - 2, iLong + 1));
        indices.push_back(calcIdx(latDiv - 2, iLong));
        indices.push_back(iSouthPole);
    }
    // wrap triangles
    // north
    indices.push_back(iNorthPole);
    indices.push_back(calcIdx(0, longDiv - 1));
    indices.push_back(calcIdx(0, 0));
    // south
    indices.push_back(calcIdx(latDiv - 2, 0));
    indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
    indices.push_back(iSouthPole);
    return std::make_unique<Mesh>(vertices, indices);
}
