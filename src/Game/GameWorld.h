//
// Created by Leon on 30/04/2025.
//

#ifndef GAMEWORLD_H
#define GAMEWORLD_H
#include <string>
#include <vector>

#include "Graphics/Drawable.h"


class GameWorld {
public:
    static void InitSingleton();
    static void DestroySingleton();
    static GameWorld* GetInstance();

    void AddObject(std::unique_ptr<Drawable> object);

    [[nodiscard]] Drawable& GetObjectAtIndex(int index) const;
    std::vector<std::unique_ptr<Drawable>>& GetAllObjects();
private:
    static GameWorld* instance;
    std::vector<std::unique_ptr<Drawable>> objects;
};



#endif //GAMEWORLD_H
