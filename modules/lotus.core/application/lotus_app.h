#pragma once

#include "../memory/lotus_memory.h"
#include "../platform/lotus_platform.h"

#include "../graphics/lotus_gfx.h"
#include "../platform/lotus_input.h"
#include "../platform/lotus_event.h"

// TODO: merge this header/source functionality into sogfx module
#include "../graphics/lotus_glapi.h"

struct lotus_application {
    struct info {
        char* name;
    } info;

    struct state {
        ubyte running;
        lotus_event_state event;
        lotus_platform_state platform;
    } state;

    struct resource {
        lotus_window window;
    } resource;

    lotus_preframe_callback preframe_callback;
    lotus_fixedframe_callback fixedframe_callback;
    lotus_midframe_callback midframe_callback;
    lotus_postframe_callback postframe_callback;
};

LOTUS_API_ENTRY ubyte lotus_init_app(lotus_application* instance, char* name);

LOTUS_API_ENTRY ubyte lotus_make_window(lotus_application* instance, char* title, ubyte4 location[2], ubyte4 size[2]);

LOTUS_API_ENTRY ubyte lotus_run_application(lotus_application* instance);

LOTUS_API_ENTRY ubyte lotus_set_preframe(lotus_application* instance, lotus_preframe_callback callback);
LOTUS_API_ENTRY ubyte lotus_set_fixedframe(lotus_application* instance, lotus_fixedframe_callback callback);
LOTUS_API_ENTRY ubyte lotus_set_midframe(lotus_application* instance, lotus_midframe_callback callback);
LOTUS_API_ENTRY ubyte lotus_set_postframe(lotus_application* instance, lotus_postframe_callback callback);

LOTUS_API_ENTRY void lotus_exit_app(lotus_application* instance);
