//
// Created by Leon on 06/05/2025.
//

#ifndef GAME_H
#define GAME_H
#include "Core/Timer.h"


/*
 * This is where the meat of the game will be handled from.
 */
class Game {
public:
    static void Init();
    static void Update();

    static void SpawnControlWindow();

private:
    static Timer timer;
    static float speedFactor;
};



#endif //GAME_H
