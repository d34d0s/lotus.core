#pragma once

#include "../memory/somemory.h"
#include "../platform/soplatform.h"

#include "../graphics/sogfx.h"
#include "../platform/soinput.h"
#include "../platform/soevent.h"

// TODO: merge this header/source functionality into sogfx module
#include "../graphics/soglapi.h"

struct sogl_application {
    struct info {
        char* name;
    } info;

    struct state {
        ubyte running;
        sogl_event_state event;
        sogl_platform_state platform;
    } state;

    struct resource {
        sogl_window window;
    } resource;

    sogl_preframe_callback preframe_callback;
    sogl_fixedframe_callback fixedframe_callback;
    sogl_midframe_callback midframe_callback;
    sogl_postframe_callback postframe_callback;
};

SOGL_API_ENTRY ubyte sogl_init_app(sogl_application* instance, char* name);

SOGL_API_ENTRY ubyte sogl_make_window(sogl_application* instance, char* title, ubyte4 location[2], ubyte4 size[2]);

SOGL_API_ENTRY ubyte sogl_run_application(sogl_application* instance);

SOGL_API_ENTRY ubyte sogl_set_preframe(sogl_application* instance, sogl_preframe_callback callback);
SOGL_API_ENTRY ubyte sogl_set_fixedframe(sogl_application* instance, sogl_fixedframe_callback callback);
SOGL_API_ENTRY ubyte sogl_set_midframe(sogl_application* instance, sogl_midframe_callback callback);
SOGL_API_ENTRY ubyte sogl_set_postframe(sogl_application* instance, sogl_postframe_callback callback);

SOGL_API_ENTRY void sogl_exit_app(sogl_application* instance);
