//
// Created by Leon on 02/02/2025.
//

#ifndef TOPOLOGY_H
#define TOPOLOGY_H
#include <d3d11.h>

#include "Bindable.h"


class Topology : public Bindable {
public:
    explicit Topology(D3D11_PRIMITIVE_TOPOLOGY type);

    void Bind() noexcept override;

protected:
    D3D11_PRIMITIVE_TOPOLOGY type{};
};


#endif //TOPOLOGY_H
