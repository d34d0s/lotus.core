#include "../include/application/lotus_app.h"

#include "../include/graphics/lotus_glapi.h"
#include "../include/platform/lotus_logger.h"

// internal application instance pointer
static Lotus_Application* instance_ptr;

// default application event callbacks
ubyte quit_callback(Lotus_Event data, ubyte2 event_code) {
    if (event_code == LOTUS_EVENT_APP_QUIT) {
        instance_ptr->state.running = LOTUS_FALSE;
    }
    return LOTUS_TRUE;
}

ubyte lotus_init_application(Lotus_Application* instance, char* name) {
    lotus_set_log_level(LOTUS_LOG_FATAL);
    if (!instance) return LOTUS_FALSE;
    if (!name) name = "SoGL Application";

    instance->info.name = name;

    instance->state.platform = lotus_init_platform();
    if (!instance->state.platform) {
        lotus_log_fatal("Failed to initialize platform layer!");
    }
    
    instance->preframe_callback = NULL;
    instance->midframe_callback = NULL;
    instance->postframe_callback = NULL;
    instance->fixedframe_callback = NULL;
    instance->state.running = LOTUS_TRUE;

    instance_ptr = instance;

    // register default event callbacks
    lotus_register_event(LOTUS_EVENT_APP_QUIT, quit_callback);

    return LOTUS_TRUE;
}

ubyte lotus_set_application_preframe(Lotus_Application* instance, Lotus_Preframe_Callback callback) {
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->preframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte lotus_set_application_fixedframe(Lotus_Application* instance, Lotus_Fixedframe_Callback callback) {
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->fixedframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte lotus_set_application_midframe(Lotus_Application* instance, Lotus_Midframe_Callback callback) {
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->midframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte lotus_set_application_postframe(Lotus_Application* instance, Lotus_Postframe_Callback callback) {
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->postframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte lotus_make_application_window(Lotus_Application* instance, char* title, ubyte4 size[2]) {
    lotus_set_log_level(LOTUS_LOG_FATAL);
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->resource.window = lotus_create_platform_window((title != NULL) ? title : instance->info.name, size[0], size[1]);

    if (!lotus_create_platform_gl_context(&instance->resource.window)) {
        lotus_log_fatal("Failed to create GL Context!");
        return LOTUS_FALSE;
    }
    
    if (!lotus_load_fptrs()) {
        lotus_log_fatal("Failed to load GL function pointers!");
        return LOTUS_FALSE;
    }

    return LOTUS_TRUE;
}

void lotus_destroy_application_window(Lotus_Application* instance) {
    if (!instance || !instance->state.running) return;
    lotus_destroy_platform_window(&instance->resource.window);
    lotus_destroy_platform_gl_context(&instance->resource.window);
}

ubyte lotus_run_application(Lotus_Application* instance) {
    lotus_set_log_level(LOTUS_LOG_FATAL);
    ubyte result = LOTUS_FALSE;
    if (!instance || !instance->state.running) return result;
    
    while (instance->state.running) {
        if (instance->midframe_callback == NULL) {
            lotus_log_fatal("SoGL Application Missing Mid/Post Frame Callback Pointer!");
            return result;
        }
        
        // preframe logic
        lotus_pump_platform_messages();
        if (instance->preframe_callback != NULL) {
            result = instance->preframe_callback();
        }
        
        if (instance->fixedframe_callback != NULL) {
            result = instance->fixedframe_callback();
        }

        result = instance->midframe_callback();
        
        // postframe logic
        if (instance->postframe_callback != NULL) {
            result = instance->postframe_callback();
        }
        lotus_swap_platform_buffers(&instance->resource.window);
        lotus_update_input(0);
    }

    return result;
}

void lotus_exit_application(Lotus_Application* instance) {
    if (!instance) return;
    instance->info.name = "NULL";
    instance->state.platform = NULL;
    instance->preframe_callback = NULL;
    instance->midframe_callback = NULL;
    instance->postframe_callback = NULL;
    instance->fixedframe_callback = NULL;
    instance->state.running = LOTUS_FALSE;
    lotus_exit_platform();
}
