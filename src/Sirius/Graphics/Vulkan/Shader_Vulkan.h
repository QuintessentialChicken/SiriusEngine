//
// Created by Leon on 26/05/2025.
//

#ifndef SHADER_VULKAN_H
#define SHADER_VULKAN_H
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "Graphics/Shader.h"


class Shader_Vulkan : public IShader {
public:
    Shader_Vulkan(ShaderType type, const std::string& path, VkDevice device);

    void Bind() override;

    [[nodiscard]] const void* GetBytecode() const override;

    [[nodiscard]] size_t GetBytecodeSize() const override;

    [[nodiscard]] VkShaderModule GetShaderModule() const;

    static VkShaderModule CreateShaderModule(const std::vector<char>& code, VkDevice device);
    static std::vector<char> ReadFile(const std::string& filename);
private:
    ShaderType type;
    VkShaderModule vertShader;
    VkShaderModule fragShader;
};



#endif //SHADER_VULKAN_H
