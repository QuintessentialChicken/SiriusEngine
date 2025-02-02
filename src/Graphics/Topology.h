//
// Created by Leon on 02/02/2025.
//

#ifndef TOPOLOGY_H
#define TOPOLOGY_H
#include "Bindable.h"


class Topology : public Bindable {
public:
    Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);

    void Bind(Graphics& gfx) noexcept override;

protected:
    D3D11_PRIMITIVE_TOPOLOGY type;
};


#endif //TOPOLOGY_H
