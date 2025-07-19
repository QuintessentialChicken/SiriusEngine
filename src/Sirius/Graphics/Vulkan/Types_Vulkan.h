//
// Created by Lms on 19/07/2025.
//

#ifndef TYPES_VULKAN_H
#define TYPES_VULKAN_H
#include <cstdint>
#include <vulkan/vulkan_core.h>

enum class MeshpassType : uint8_t {
    None = 0,
    Forward = 1,
    Transparency = 2,
    DirectionalShadow = 3
};

#endif //TYPES_VULKAN_H
