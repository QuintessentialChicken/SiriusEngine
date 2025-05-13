// Model.h
#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <typeindex>
#include <unordered_map>

#include "Component.h"
#include "Graphics/Mesh.h"
#include "Graphics/Material.h"
#include "Graphics/Transform.h"
enum class Primitives {
    CUBE = 0,
    SPHERE = 1,
    CUBE_PHONG = 2,
};
class Model {
public:


    Model(std::unique_ptr<Mesh> mesh, std::unique_ptr<Material> material);

    static std::unique_ptr<Model> CreatePrimitive(Primitives primitive);

    void Draw(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& projection);
    void Update(float dt);

    void SetPosition(const DirectX::XMFLOAT3& pos);
    void Translate(const DirectX::XMFLOAT3& offset);
    void SetRotation(const DirectX::XMFLOAT3& rot);
    void SetScale(const DirectX::XMFLOAT3& scl);

       // Component management
    template<typename T>
    T* AddComponent(std::unique_ptr<T> comp) {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

        // Check if component of this type already exists
        auto typeId = std::type_index(typeid(T));
        if (componentMap.contains(typeId)) {
            return static_cast<T*>(componentMap[typeId]);
        }

        // Move component
        T* componentPtr = comp.get();

        // Set owner and initialize
        componentPtr->SetOwner(this);
        componentPtr->Init();

        // Store in both containers
        components.push_back(std::move(comp));
        componentMap[typeId] = components.back().get();

        return componentPtr;
    }

    template<typename T>
    T* GetComponent() {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

        auto typeId = std::type_index(typeid(T));
        auto it = componentMap.find(typeId);
        if (it != componentMap.end()) {
            return static_cast<T*>(it->second);
        }
        return nullptr;
    }

    template<typename T>
    bool RemoveComponent() {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

        auto typeId = std::type_index(typeid(T));
        auto it = componentMap.find(typeId);
        if (it != componentMap.end()) {
            Component* componentToRemove = it->second;
            componentMap.erase(typeId);

            // Find and remove from components vector
            components.erase(
                std::remove_if(components.begin(), components.end(),
                    [componentToRemove](const std::unique_ptr<Component>& comp) {
                        return comp.get() == componentToRemove;
                    }),
                components.end()
            );
            return true;
        }
        return false;
    }

    // Expose transform for manipulation

    Transform& GetTransform();

    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float theta = 0.0f;
    float phi = 0.0f;
    float chi = 0.0f;
    float droll = 0.0f;
    float dpitch = 0.0f;
    float dyaw = 0.0f;
    float dtheta = 0.0f;
    float dphi = 0.0f;
    float dchi = 0.0f;
    float r = 0.0f;
private:
    void UpdateTransform();


    std::unique_ptr<Material> material;
    Transform transform;
    std::unique_ptr<TransformBuffer> transformBuffer;
    std::vector<std::unique_ptr<Component>> components;
    std::unordered_map<std::type_index, Component*> componentMap;

};


#endif // MODEL_H
