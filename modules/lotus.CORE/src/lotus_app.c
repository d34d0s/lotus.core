#include "../include/application/lotus_app.h"

#include "../include/graphics/lotus_glapi.h"
#include "../include/platform/lotus_logger.h"

// internal application instance pointer
static lotus_application* instance_ptr;

// default application event callbacks
ubyte quit_callback(lotus_event data, ubyte2 event_code) {
    if (event_code == LOTUS_EVENT_APP_QUIT) {
        instance_ptr->state.running = LOTUS_FALSE;
    }
    return LOTUS_TRUE;
}

ubyte lotus_app_init(lotus_application* instance, char* name) {
    lotus_set_log_level(LOTUS_LOG_FATAL);
    if (!instance) return LOTUS_FALSE;
    if (!name) name = "SoGL Application";

    instance->info.name = name;

    instance->state.platform = platform_init();
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

ubyte lotus_app_set_preframe(lotus_application* instance, lotus_preframe_callback callback) {
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->preframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte lotus_app_set_fixedframe(lotus_application* instance, lotus_fixedframe_callback callback) {
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->fixedframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte lotus_app_set_midframe(lotus_application* instance, lotus_midframe_callback callback) {
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->midframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte lotus_app_set_postframe(lotus_application* instance, lotus_postframe_callback callback) {
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->postframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte lotus_app_make_window(lotus_application* instance, char* title, ubyte4 size[2]) {
    lotus_set_log_level(LOTUS_LOG_FATAL);
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->resource.window = platform_create_window((title != NULL) ? title : instance->info.name, size[0], size[1]);

    if (!platform_create_gl_context(&instance->resource.window)) {
        lotus_log_fatal("Failed to create GL Context!");
        return LOTUS_FALSE;
    }
    
    if (!lotus_load_fptrs()) {
        lotus_log_fatal("Failed to load GL function pointers!");
        return LOTUS_FALSE;
    }

    return LOTUS_TRUE;
}

void lotus_app_destroy_window(lotus_application* instance) {
    if (!instance || !instance->state.running) return;
    platform_destroy_window(&instance->resource.window);
    platform_destroy_gl_context(&instance->resource.window);
}

ubyte lotus_app_run(lotus_application* instance) {
    lotus_set_log_level(LOTUS_LOG_FATAL);
    ubyte result = LOTUS_FALSE;
    if (!instance || !instance->state.running) return result;
    
    while (instance->state.running) {
        if (instance->midframe_callback == NULL) {
            lotus_log_fatal("SoGL Application Missing Mid/Post Frame Callback Pointer!");
            return result;
        }
        
        // preframe logic
        platform_pump();
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
        platform_swap_buffers(&instance->resource.window);
        lotus_input_update(0);
    }

    return result;
}

void lotus_app_exit(lotus_application* instance) {
    if (!instance) return;
    instance->info.name = "NULL";
    instance->state.platform = NULL;
    instance->preframe_callback = NULL;
    instance->midframe_callback = NULL;
    instance->postframe_callback = NULL;
    instance->fixedframe_callback = NULL;
    instance->state.running = LOTUS_FALSE;
    platform_exit();
}
