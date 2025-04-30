#include <iostream>

#include "Core/App.h"
#include "Core/Entrypoint_Engine.h"

#define SET_APP_ENTRY_POINTS(pro, loop) \
static struct app_entry_point_setter_t \
{ \
app_entry_point_setter_t() \
{ \
g_pProjectMainPrologue = pro; \
g_pProjectMainOrDoOneLoop = loop; \
} \
} app_entry_point_setter_instance; \

App my_app;

void Update() {
    std::cout << "Update" << std::endl;
}

bool Main_Prologue() {
    std::cout << "Prologue" << "\n" << "Registering Update Function" << std::endl;
    my_app.RegisterUpdateFunction(Update);
    return true;
}

bool Main_OneLoopIteration() {
    return my_app.RunOneIteration();
}



SET_APP_ENTRY_POINTS(Main_Prologue, Main_OneLoopIteration);

int Main() {
    assert(false && "Should never reach here, Sirius is using App style entry points");
    return 0;
}