//
// Created by Leon on 30/04/2025.
//

#ifndef PHYSICS_H
#define PHYSICS_H
#include <DirectXMath.h>

#include "Core/Timer.h"
#include "Graphics/Drawable.h"


class Physics {
public:
    static float privateForce;

    static void Init();
    static void Update();

    static void SpawnControlWindow() noexcept;

private:
    static DirectX::XMFLOAT2 CalculateGravity(float mass);

    static void CalculateTorque(const std::unique_ptr<Drawable>& object);

    static float force;
    static bool applyForce;
};



#endif //PHYSICS_H
