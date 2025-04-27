#include "Core/App.h"


App my_app;

bool Main_OneLoopIteration() {
    return my_app.RunOneIteration();
}



int main() {
    bool carryOn = true;
    while (carryOn) {
        carryOn = Main_OneLoopIteration();
    }
    return 0;
}


