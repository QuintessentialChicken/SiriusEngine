//
// Created by Leon on 30/04/2025.
//

#ifndef GAMEWORLD_H
#define GAMEWORLD_H
#include <memory>
#include <vector>

#include "../Sirius/Core/Model.h"
#include "Graphics/PointLight.h"



class GameWorld {
public:
    static void InitSingleton();
    static void DestroySingleton();
    static GameWorld* GetInstance();

    void AddLightSource(std::unique_ptr<Model> light);
    void AddObject(std::unique_ptr<Model> object);

    std::vector<std::unique_ptr<Model>>& GetAllLightSources();

    [[nodiscard]] Model& GetObjectAtIndex(int index) const;
    std::vector<std::unique_ptr<Model>>& GetAllObjects();
private:
    static GameWorld* instance;
    std::vector<std::unique_ptr<Model>> objects;
    std::vector<std::unique_ptr<Model>> lights;
};



#endif //GAMEWORLD_H
