//
// Created by Leon on 24/01/2025.
//

#ifndef DXGIINFOMANAGER_H
#define DXGIINFOMANAGER_H

#include <vector>
#include <string>

class DxgiInfoManager
{
public:
    DxgiInfoManager();
    ~DxgiInfoManager();
    DxgiInfoManager( const DxgiInfoManager& ) = delete;
    DxgiInfoManager& operator=( const DxgiInfoManager& ) = delete;
    void Set() noexcept;
    std::vector<std::string> GetMessages() const;
private:
    unsigned long long next = 0u;
    struct IDXGIInfoQueue* pDxgiInfoQueue = nullptr;
};

#endif //DXGIINFOMANAGER_H
