#ifndef INPUT_LAYOUT_H
#define INPUT_LAYOUT_H

#include <string>

// API-agnostic format description
enum class DataFormat {
    Float1,
    Float2,
    Float3,
    Float4,
    Int1,
    Int2,
    Int3,
    Int4,
    UInt1,
    UInt2,
    UInt3,
    UInt4
};

// Input layout element description
struct InputLayoutElement {
    std::string semanticName;
    unsigned int semanticIndex;
    DataFormat format;
    unsigned int inputSlot;
    unsigned int alignedByteOffset;
    bool perInstance = false;
    unsigned int instanceDataStepRate = 0;
};

// Input layout interface
class IInputLayout {
public:
    virtual ~IInputLayout() = default;
    virtual void Bind() = 0;
};



#endif // INPUT_LAYOUT_H