//
// Created by Leon on 05/05/2025.
//

#ifndef FSM_H
#define FSM_H



class Fsm {
public:
    enum Return {
        UNHANDLED = 0,
        CONTINUE,
        EXIT,
    };
    Fsm();
    virtual ~Fsm() = default;

    Return Update();
    bool RunOneIteration() noexcept;

    void SetState(signed short state) noexcept;
    signed short GetState() const noexcept;
protected:
    // App implements this to define where to go from which state.
    virtual Return UpdateState(signed short state) = 0;

    bool currentStateNotYetEntered = true;
private:
    signed short currentState;
    signed short nextState;
};



#endif //FSM_H
