//
// Created by Leon on 26/05/2025.
//

#include "Shader_Vulkan.h"

#include <cassert>
#include <fstream>
#include <stdexcept>
#include <utility>
#include "External/spirv_reflect.h"

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

void ShaderEffect::add_stage(ShaderModule *shaderModule, VkShaderStageFlagBits stage) {
    ShaderStage newStage = {shaderModule, stage};
    stages.push_back(newStage);
}

void ShaderEffect::fill_stages(std::vector<VkPipelineShaderStageCreateInfo> &pipelineStages) {
    for (auto &s: stages) {
        VkPipelineShaderStageCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        info.pNext = nullptr;

        //shader stage
        info.stage = s.stage;
        //module containing the code for this shader stage
        info.module = s.shaderModule->module;
        //the entry point of the shader
        info.pName = "main";
        pipelineStages.push_back(info);
    }
}

void ShaderEffect::reflect_layout(VkDevice device, ReflectionOverrides* overrides, int overrideCount) {
    	std::vector<DescriptorSetLayoutData> set_layouts;

	std::vector<VkPushConstantRange> constant_ranges;

	for (auto& s : stages) {

		SpvReflectShaderModule spvmodule;
		SpvReflectResult result = spvReflectCreateShaderModule(s.shaderModule->code.size() * sizeof(uint32_t), s.shaderModule->code.data(), &spvmodule);

		uint32_t count = 0;
		result = spvReflectEnumerateDescriptorSets(&spvmodule, &count, NULL);
		assert(result == SPV_REFLECT_RESULT_SUCCESS);

		std::vector<SpvReflectDescriptorSet*> sets(count);
		result = spvReflectEnumerateDescriptorSets(&spvmodule, &count, sets.data());
		assert(result == SPV_REFLECT_RESULT_SUCCESS);

		for (size_t i_set = 0; i_set < sets.size(); ++i_set) {

			const SpvReflectDescriptorSet& refl_set = *(sets[i_set]);

			DescriptorSetLayoutData layout = {};

			layout.bindings.resize(refl_set.binding_count);
			for (uint32_t i_binding = 0; i_binding < refl_set.binding_count; ++i_binding) {
				const SpvReflectDescriptorBinding& refl_binding = *(refl_set.bindings[i_binding]);
				VkDescriptorSetLayoutBinding& layout_binding = layout.bindings[i_binding];
				layout_binding.binding = refl_binding.binding;
				layout_binding.descriptorType = static_cast<VkDescriptorType>(refl_binding.descriptor_type);

				for (int ov = 0; ov < overrideCount; ov++)
				{
					if (strcmp(refl_binding.name, overrides[ov].name) == 0) {
						layout_binding.descriptorType = overrides[ov].overriddenType;
					}
				}

				layout_binding.descriptorCount = 1;
				for (uint32_t i_dim = 0; i_dim < refl_binding.array.dims_count; ++i_dim) {
					layout_binding.descriptorCount *= refl_binding.array.dims[i_dim];
				}
				layout_binding.stageFlags = static_cast<VkShaderStageFlagBits>(spvmodule.shader_stage);

				ReflectedBinding reflected;
				reflected.binding = layout_binding.binding;
				reflected.set = refl_set.set;
				reflected.type = layout_binding.descriptorType;

				bindings[refl_binding.name] = reflected;
			}
			layout.set_number = refl_set.set;
			layout.create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layout.create_info.bindingCount = refl_set.binding_count;
			layout.create_info.pBindings = layout.bindings.data();

			set_layouts.push_back(layout);
		}

		//pushconstants

		result = spvReflectEnumeratePushConstantBlocks(&spvmodule, &count, NULL);
		assert(result == SPV_REFLECT_RESULT_SUCCESS);

		std::vector<SpvReflectBlockVariable*> pconstants(count);
		result = spvReflectEnumeratePushConstantBlocks(&spvmodule, &count, pconstants.data());
		assert(result == SPV_REFLECT_RESULT_SUCCESS);

		if (count > 0) {
			VkPushConstantRange pcs{};
			pcs.offset = pconstants[0]->offset;
			pcs.size = pconstants[0]->size;
			pcs.stageFlags = s.stage;

			constant_ranges.push_back(pcs);
		}
	}




	std::array<DescriptorSetLayoutData,4> merged_layouts;

	for (int i = 0; i < 4; i++) {

		DescriptorSetLayoutData &ly = merged_layouts[i];

		ly.set_number = i;

		ly.create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

		std::unordered_map<int,VkDescriptorSetLayoutBinding> binds;
		for (auto& s : set_layouts) {
			if (s.set_number == i) {
				for (auto& b : s.bindings)
				{
					auto it = binds.find(b.binding);
					if (it == binds.end())
					{
						binds[b.binding] = b;
						//ly.bindings.push_back(b);
					}
					else {
						//merge flags
						binds[b.binding].stageFlags |= b.stageFlags;
					}

				}
			}
		}
		for (auto [k, v] : binds)
		{
			ly.bindings.push_back(v);
		}
		//sort the bindings, for hash purposes
		std::sort(ly.bindings.begin(), ly.bindings.end(), [](VkDescriptorSetLayoutBinding& a, VkDescriptorSetLayoutBinding& b) {
			return a.binding < b.binding;
		});


		ly.create_info.bindingCount = (uint32_t)ly.bindings.size();
		ly.create_info.pBindings = ly.bindings.data();
		ly.create_info.flags = 0;
		ly.create_info.pNext = 0;


		if (ly.create_info.bindingCount > 0) {
			setHashes[i] = vkutil::hash_descriptor_layout_info(&ly.create_info);
			vkCreateDescriptorSetLayout(device, &ly.create_info, nullptr, &setLayouts[i]);
		}
		else {
			setHashes[i] = 0;
			setLayouts[i] = VK_NULL_HANDLE;
		}
	}

	//we start from just the default empty pipeline layout info
	VkPipelineLayoutCreateInfo mesh_pipeline_layout_info = vkinit::pipeline_layout_create_info();

	mesh_pipeline_layout_info.pPushConstantRanges = constant_ranges.data();
	mesh_pipeline_layout_info.pushConstantRangeCount = (uint32_t)constant_ranges.size();

	std::array<VkDescriptorSetLayout,4> compactedLayouts;
	int s = 0;
	for (int i = 0; i < 4; i++) {
		if (setLayouts[i] != VK_NULL_HANDLE) {
			compactedLayouts[s] = setLayouts[i];
			s++;
		}
	}

	mesh_pipeline_layout_info.setLayoutCount = s;
	mesh_pipeline_layout_info.pSetLayouts = compactedLayouts.data();


	vkCreatePipelineLayout(device, &mesh_pipeline_layout_info, nullptr, &builtLayout);

}
