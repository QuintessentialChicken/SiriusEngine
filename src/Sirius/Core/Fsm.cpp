//
// Created by Leon on 05/05/2025.
//

#include "Fsm.h"

Fsm::Fsm() {
    currentState = 0;
    nextState = 0;
}

Fsm::Return Fsm::Update() {
    Return ret = UNHANDLED;
    while (ret == UNHANDLED) {
        // If the next state differs from the current state, there is a request to transition to the next state
        // Update the current state and let the app handle it
        if (currentState != nextState) {
            currentState = nextState;
            ret = UpdateState(currentState);
        // If the next state is the same as the current, we might be in a scenario where the state hasn't been updated yet but we also weren't in the current state before. This occurs in the very first run for example.
        // Check if we enter for the first time (we could send an event or different state to allow for some setup of the state)
        // If yes, clear the flag. If not, do a regular update. This probably leads to the main game loop
        } else {
            if (currentStateNotYetEntered) {
                currentStateNotYetEntered = false;
                ret = UpdateState(currentState);
            } else {
                ret = UpdateState(currentState);
            }
        }
    }
    return ret;
}

bool Fsm::RunOneIteration() noexcept {
    if (Update() == EXIT) {
        return false;
    }
    return true;
}

void Fsm::SetState(const signed short state) noexcept {
    nextState = state;
}

signed short Fsm::GetState() const noexcept {
    return currentState;
}
