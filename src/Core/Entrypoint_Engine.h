//
// Created by Leon on 29/04/2025.
//

#ifndef ENTRYPOINT_ENGINE_H
#define ENTRYPOINT_ENGINE_H
extern bool (*g_pProjectMainPrologue)();
extern bool (*g_pProjectMainOrDoOneLoop)();
extern int Main();

#define SET_APP_ENTRY_POINTS(loop) \
static struct app_entry_point_setter_t \
{ \
app_entry_point_setter_t() \
{ \
g_pProjectMainPrologue = pro \
g_pProjectMainOrDoOneLoop = loop; \
} \
} app_entry_point_setter_instance; \

#endif //ENTRYPOINT_ENGINE_H
