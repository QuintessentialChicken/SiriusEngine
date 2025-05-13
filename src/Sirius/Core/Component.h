//
// Created by Leon on 12/05/2025.
//

#ifndef COMPONENT_H
#define COMPONENT_H


class Model;

class Component {
public:
    Component() = default;

    virtual ~Component() = default;

    virtual void Init() {}

    virtual void Update() {}

    void SetOwner(Model* owner) { this->owner = owner; };

    [[nodiscard]] Model* GetOwner() const { return owner; };

private:
    Model* owner = nullptr;
};


#endif //COMPONENT_H
