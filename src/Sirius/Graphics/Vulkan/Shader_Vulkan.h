//
// Created by Leon on 26/05/2025.
//

#ifndef SHADER_VULKAN_H
#define SHADER_VULKAN_H
#include <array>
#include <string>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "Graphics/Shader.h"


class Shader_Vulkan : public IShader {
public:
    Shader_Vulkan(ShaderType type, std::string  path, VkDevice device);

    void Bind() override;

    [[nodiscard]] const void* GetBytecode() const override;

    [[nodiscard]] size_t GetBytecodeSize() const override;

    [[nodiscard]] VkShaderModule GetShaderModule() const;

    static VkShaderModule CreateShaderModule(const std::vector<char>& code, VkDevice device);
    static std::vector<char> ReadFile(const std::string& filename);

    struct ShaderPaths {
        std::string vertex;
        std::string fragment;
    };
private:
    VkDevice device;
    ShaderType type;
    ShaderPaths shaderPaths;
};

struct ShaderModule {
    std::vector<uint32_t> code;
    VkShaderModule module;
};

namespace vkutil {
	bool load_shader_module(VkDevice device, const char* filePath, ShaderModule* outShaderModule);
    uint32_t hash_descriptor_layout_info(VkDescriptorSetLayoutCreateInfo* info);
}

struct DescriptorSetLayoutData {
    uint32_t set_number;
    VkDescriptorSetLayoutCreateInfo create_info;
    std::vector<VkDescriptorSetLayoutBinding> bindings;
};

struct ShaderEffect {
    struct ReflectionOverrides {
        const char* name;
        VkDescriptorType overriddenType;
    };

    VkPipelineLayout builtLayout;
    std::array<VkDescriptorSetLayout, 4> setLayouts;

    void add_stage(ShaderModule* shaderModule, VkShaderStageFlagBits stage);
    void fill_stages(std::vector<VkPipelineShaderStageCreateInfo>& pipelineStages);
	void reflect_layout(VkDevice device, ReflectionOverrides* overrides, int overrideCount);


    struct ReflectedBinding {
        uint32_t set;
        uint32_t binding;
        VkDescriptorType type;
    };
	std::unordered_map<std::string, ReflectedBinding> bindings;
    std::array<uint32_t, 4> setHashes;
private:
    struct ShaderStage {
        ShaderModule* shaderModule;
        VkShaderStageFlagBits stage;
    };

    std::vector<ShaderStage> stages;

};

class ShaderCache {
public:
	void init(VkDevice vk_device) { device = vk_device; };

    ShaderModule* get_shader(const std::string& path);

private:
    VkDevice device;
	std::unordered_map<std::string, ShaderModule> module_cache;
};


#endif //SHADER_VULKAN_H
