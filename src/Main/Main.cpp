#include "Core/App.h"
#include "Core/Entrypoint_Engine.h"

#define SET_APP_ENTRY_POINTS(loop) \
static struct app_entry_point_setter_t \
{ \
app_entry_point_setter_t() \
{ \
g_pProjectMainOrDoOneLoop = loop; \
} \
} app_entry_point_setter_instance; \

App my_app;

bool Main_OneLoopIteration() {
    return my_app.RunOneIteration();
}

SET_APP_ENTRY_POINTS(Main_OneLoopIteration);

int Main() {
    assert(false && "Should never reach here, Sirius is using App style entry points");
    return 0;
}