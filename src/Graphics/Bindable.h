//
// Created by Leon on 02/02/2025.
//

#ifndef BINDABLE_H
#define BINDABLE_H


class Bindable {
public:
    virtual void Bind() noexcept = 0;

    virtual ~Bindable() = default;

};

#endif //BINDABLE_H
