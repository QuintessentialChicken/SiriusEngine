//
// Created by Leon on 26/05/2025.
//

#include "Shader_Vulkan.h"

#include <fstream>
#include <stdexcept>
#include <utility>

// Can just be an empty shell containing type and path.
// Since shader modules only need to live until the pipeline is created, PipelineState can create them on the spot and destroy them afterwards
Shader_Vulkan::Shader_Vulkan(ShaderType type, std::string path, VkDevice device): type{type}, device{device} {
    switch (type) {
        case ShaderType::Vertex:
            shaderPaths.vertex = std::move(path);
            break;
        case ShaderType::Pixel:
            shaderPaths.fragment = std::move(path);
            break;
        default: break;
    }
}

void Shader_Vulkan::Bind() {
    // Not used in Vulkan
    throw std::runtime_error("Bind is not used in Vulkan Shaders");
}

const void *Shader_Vulkan::GetBytecode() const {
    throw std::runtime_error("Not used in Vulkan Shaders");
}

size_t Shader_Vulkan::GetBytecodeSize() const {
    throw std::runtime_error("Not used in Vulkan Shaders");
}

VkShaderModule Shader_Vulkan::GetShaderModule() const {
    switch (type) {
        case ShaderType::Vertex:
            return CreateShaderModule(ReadFile(shaderPaths.vertex), device);
        case ShaderType::Pixel:
            return CreateShaderModule(ReadFile(shaderPaths.fragment), device);
        default: return nullptr;
    }
}

VkShaderModule Shader_Vulkan::CreateShaderModule(const std::vector<char> &code, VkDevice device) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module!");
    }
    return shaderModule;
}

std::vector<char> Shader_Vulkan::ReadFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file!");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}
