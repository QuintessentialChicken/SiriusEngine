//
// Created by Leon on 10/05/2025.
//

#include "RenderApi.h"
#include "RenderApi_D3D11.h"

IRenderApi::Api IRenderApi::api = Api::D3D11;

std::unique_ptr<IRenderApi> IRenderApi::Create() {
    switch (api) {
        case Api::D3D11:
            return std::make_unique<RenderApi_D3D11>();
        default:
            return nullptr;
    }
}