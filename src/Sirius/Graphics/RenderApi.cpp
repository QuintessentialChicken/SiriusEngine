//
// Created by Leon on 10/05/2025.
//

#include "RenderApi.h"
#include "D3D11/RenderApi_D3D11.h"
#include "Vulkan/RenderApi_Vulkan.h"

IRenderApi::Api IRenderApi::api = Api::Vulkan;

std::unique_ptr<IRenderApi> IRenderApi::Create() {
    switch (api) {
        case Api::D3D11:
            return std::make_unique<RenderApi_D3D11>();
        case Api::Vulkan:
            return std::make_unique<RenderApi_Vulkan>();
        default:
            return nullptr;
    }
}