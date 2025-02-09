//
// Created by Leon on 08/02/2025.
//
#ifndef GAME_H
#define GAME_H

class Game {
public:
    virtual ~Game() = default;
    virtual void Start() {}  // Called once when the game starts
    virtual void Update() {}  // Called every frame

};

#endif // GAME_H
