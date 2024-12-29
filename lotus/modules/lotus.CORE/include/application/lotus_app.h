#pragma once

#include "../platform/lotus_memory.h"
#include "../platform/lotus_platform.h"

#include "../graphics/lotus_gl.h"
#include "../platform/lotus_input.h"
#include "../platform/lotus_event.h"

struct Lotus_Application {
    struct info {
        char* name;
    } info;

    struct state {
        ubyte running;
        Lotus_Platform_State* platform;
    } state;

    struct resource {
        Lotus_Window window;
    } resource;

    Lotus_Preframe_Callback preframe_callback;
    Lotus_Fixedframe_Callback fixedframe_callback;
    Lotus_Midframe_Callback midframe_callback;
    Lotus_Postframe_Callback postframe_callback;
};

LOTUS_API_ENTRY ubyte lotus_init_application(Lotus_Application* instance, char* name);
LOTUS_API_ENTRY void lotus_exit_application(Lotus_Application* instance);

LOTUS_API_ENTRY ubyte lotus_make_application_window(Lotus_Application* instance, char* title, ubyte4 size[2]);
LOTUS_API_ENTRY void lotus_destroy_application_window(Lotus_Application* instance);

LOTUS_API_ENTRY ubyte lotus_run_application(Lotus_Application* instance);

LOTUS_API_ENTRY ubyte lotus_set_application_preframe(Lotus_Application* instance, Lotus_Preframe_Callback callback);
LOTUS_API_ENTRY ubyte lotus_set_application_fixedframe(Lotus_Application* instance, Lotus_Fixedframe_Callback callback);
LOTUS_API_ENTRY ubyte lotus_set_application_midframe(Lotus_Application* instance, Lotus_Midframe_Callback callback);
LOTUS_API_ENTRY ubyte lotus_set_application_postframe(Lotus_Application* instance, Lotus_Postframe_Callback callback);
