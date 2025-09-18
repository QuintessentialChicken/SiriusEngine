//
// Created by Lms on 19/07/2025.
//

#ifndef MATERIALSYSTEM_H
#define MATERIALSYSTEM_H
#include <array>
#include <cassert>
#include <DirectXMath.h>
#include <vec3.hpp>
#include <vec2.hpp>
#include <vector>

#include "Shader_Vulkan.h"
#include "Types_Vulkan.h"

struct ShaderEffect;

enum class TransparencyMode:uint8_t {
    Opaque,
    Transparent,
    Masked
};

struct MaterialInfo {
    std::string baseEffect;
    std::unordered_map<std::string, std::string> textures; //name -> path
    std::unordered_map<std::string, std::string> customProperties;
    TransparencyMode transparency;
};

struct VertexInputDescription {
    std::vector<VkVertexInputBindingDescription> bindings;
    std::vector<VkVertexInputAttributeDescription> attributes;

    VkPipelineVertexInputStateCreateFlags flags = 0;
};

struct Vertex {
    glm::vec3 position;
    DirectX::XMFLOAT3 color;
    glm::vec<2, uint8_t> oct_normal; //color;
    glm::vec2 uv;

    static VertexInputDescription get_vertex_description();
};


class PipelineBuilder {
public:
    std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
    VertexInputDescription vertexDescription;
    VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
    VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
    VkViewport _viewport;
    VkRect2D _scissor;
    VkPipelineRasterizationStateCreateInfo _rasterizer;
    VkPipelineColorBlendAttachmentState _colorBlendAttachment;
    VkPipelineMultisampleStateCreateInfo _multisampling;
    VkPipelineLayout _pipelineLayout;
    VkPipelineDepthStencilStateCreateInfo _depthStencil;

    VkPipeline build_pipeline(VkDevice device, VkRenderPass pass);

    void clear_vertex_input();

    void setShaders(struct ShaderEffect* effect);
};

struct ShaderPass {
    ShaderEffect* effect{nullptr};
    VkPipeline pipeline{VK_NULL_HANDLE};
    VkPipelineLayout layout{VK_NULL_HANDLE};
};

struct SampledTexture {
    VkSampler sampler;
    VkImageView view;
};

struct ShaderParameters {
};

template<typename T>
struct PerPassData {
    T& operator[](MeshpassType pass) {
        switch (pass) {
            case MeshpassType::Forward:
                return data[0];
            case MeshpassType::Transparency:
                return data[1];
            case MeshpassType::DirectionalShadow:
                return data[2];
        }
        assert(false);
        return data[0];
    };

    void clear(T&& val) {
        for (int i = 0; i < 3; i++) {
            data[i] = val;
        }
    }

private:
    std::array<T, 3> data;
};

struct EffectTemplate {
    PerPassData<ShaderPass*> passShaders;

    ShaderParameters* defaultParameters;
    TransparencyMode transparency;
};

struct MaterialData {
    std::vector<SampledTexture> textures;
    ShaderParameters* parameters;
    std::string baseTemplate;

    size_t hash() const;
};

struct Material {
    EffectTemplate* original;
    PerPassData<VkDescriptorSet> passSets;

    std::vector<SampledTexture> textures;

    ShaderParameters* parameters;

    Material& operator=(const Material& other) = default;
};
class DescriptorAllocator {
public:

    struct PoolSizes {
        std::vector<std::pair<VkDescriptorType,float>> sizes =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 0.5f },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4.f },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 4.f },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1.f },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1.f },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1.f },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2.f },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2.f },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1.f },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1.f },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 0.5f }
        };
    };

    void reset_pools();
    bool allocate(VkDescriptorSet* set, VkDescriptorSetLayout layout);

    void init(VkDevice newDevice);

    void cleanup();

    VkDevice device;
private:
    VkDescriptorPool grab_pool();

    VkDescriptorPool currentPool{VK_NULL_HANDLE};
    PoolSizes descriptorSizes;
    std::vector<VkDescriptorPool> usedPools;
    std::vector<VkDescriptorPool> freePools;
};

class DescriptorLayoutCache {
public:
    void init(VkDevice newDevice);
    void cleanup();

    VkDescriptorSetLayout create_descriptor_layout(VkDescriptorSetLayoutCreateInfo* info);

    struct DescriptorLayoutInfo {
        //good idea to turn this into a inlined array
        std::vector<VkDescriptorSetLayoutBinding> bindings;

        bool operator==(const DescriptorLayoutInfo& other) const;

        size_t hash() const;
    };
private:

    struct DescriptorLayoutHash
    {

        std::size_t operator()(const DescriptorLayoutInfo& k) const
        {
            return k.hash();
        }
    };

    std::unordered_map<DescriptorLayoutInfo, VkDescriptorSetLayout, DescriptorLayoutHash> layoutCache;
    VkDevice device;
};


class DescriptorBuilder {
public:

    static DescriptorBuilder begin(DescriptorLayoutCache* layoutCache, DescriptorAllocator* allocator );

    DescriptorBuilder& bind_buffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo, VkDescriptorType type, VkShaderStageFlags stageFlags);

    DescriptorBuilder& bind_image(uint32_t binding, VkDescriptorImageInfo* imageInfo, VkDescriptorType type, VkShaderStageFlags stageFlags);

    bool build(VkDescriptorSet& set, VkDescriptorSetLayout& layout);
    bool build(VkDescriptorSet& set);
private:

    std::vector<VkWriteDescriptorSet> writes;
    std::vector<VkDescriptorSetLayoutBinding> bindings;


    DescriptorLayoutCache* cache;
    DescriptorAllocator* alloc;
};

class MaterialSystem {
public:
    MaterialSystem(VkDevice device, VkRenderPass renderPass, ShaderCache& shaderCache);

    void build_default_templates();

    ShaderPass* build_shader(PipelineBuilder& builder, ShaderEffect* effect);

    Material* build_material(const std::string& materialName, const MaterialData& info, DescriptorLayoutCache& layoutCache, DescriptorAllocator& allocator);

    Material* get_material(const std::string& materialName);

    void fill_builders();

private:
    struct MaterialInfoHash {
        std::size_t operator()(const MaterialData& k) const {
            return k.hash();
        }
    };

    PipelineBuilder forwardBuilder;
    PipelineBuilder shadowBuilder;
    std::unordered_map<MaterialData, Material*, MaterialInfoHash> materialCache;
    std::unordered_map<std::string, Material*> materials;
    std::unordered_map<std::string, EffectTemplate> templateCache;

    VkDevice device;
    VkRenderPass renderPass{};
    ShaderCache& shaderCache;
};


#endif //MATERIALSYSTEM_H
