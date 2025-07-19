//
// Created by Leon on 11/05/2025.
//

#ifndef SHADER_H
#define SHADER_H

#include <string>

enum class ShaderType {
    Vertex,
    Pixel,
    Geometry,
    Compute,
    Hull,
    Domain
};

class Shader {
public:
    Shader(ShaderType type, std::string path, VkDevice device);

    [[nodiscard]] VkShaderModule GetShaderModule() const;

    static VkShaderModule CreateShaderModule(const std::vector<char> &code, VkDevice device);

    static std::vector<char> ReadFile(const std::string &filename);

    struct ShaderPaths {
        std::string vertex;
        std::string fragment;
    };

private:
    VkDevice device;
    ShaderType type;
    ShaderPaths shaderPaths;
};


#endif //SHADER_H
