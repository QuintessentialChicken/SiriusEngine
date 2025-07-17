//
// Created by Leon on 30/04/2025.
//

#ifndef GAMEWORLD_H
#define GAMEWORLD_H
#include <memory>
#include <span>
#include <vector>

#include "../Sirius/Core/Model.h"
#include "Graphics/PointLight.h"



class GameWorld {
public:
    static void InitSingleton();
    static void DestroySingleton();
    static GameWorld* GetInstance();

    void AddObject(Model object);

    std::vector<Model>& GetLights();
    [[nodiscard]] Model& GetObjectAtIndex(int index);
    std::vector<Model>& GetAllObjects();
private:
    static GameWorld* instance;
    std::vector<Model> objects;
    std::vector<Model> lights;
};



#endif //GAMEWORLD_H
