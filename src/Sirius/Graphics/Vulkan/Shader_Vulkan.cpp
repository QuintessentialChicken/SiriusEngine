//
// Created by Leon on 26/05/2025.
//

#include "Shader_Vulkan.h"

#include <fstream>
#include <stdexcept>

Shader_Vulkan::Shader_Vulkan(ShaderType type, const std::string& path, VkDevice device) {
    std::vector<char> code = ReadFile(path);

    switch (type) {
        case ShaderType::Vertex:
            vertShader = CreateShaderModule(code, device);
            break;
        case ShaderType::Pixel:
            fragShader = CreateShaderModule(code, device);
        default: break;
    }
}

VkShaderModule Shader_Vulkan::CreateShaderModule(const std::vector<char>& code, VkDevice device) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module!");
    }
    return shaderModule;
}

std::vector<char> Shader_Vulkan::ReadFile(const std::string& filename) {
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