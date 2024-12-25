#include "lotus_app.h"

#include "../platform/lotus_logger.h"

ubyte lotus_init_app(lotus_application* instance, char* name) {
    lotus_set_log_level(LOTUS_LOG_FATAL);
    if (!instance) return LOTUS_FALSE;
    if (!name) name = "SoGL Application";

    instance->info.name = name;
    
    if (!lotus_platform_init(&instance->state.platform)) {
        lotus_log_fatal("Failed to initialize platform layer!");
    }
    
    if (!lotus_event_init(&instance->state.event)) {
        lotus_log_fatal("Failed to initialize event layer!");
    }
    instance->preframe_callback = NULL;
    instance->midframe_callback = NULL;
    instance->postframe_callback = NULL;
    instance->fixedframe_callback = NULL;
    instance->state.running = LOTUS_TRUE;

    return LOTUS_TRUE;
}

ubyte lotus_set_preframe(lotus_application* instance, lotus_preframe_callback callback) {
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->preframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte lotus_set_fixedframe(lotus_application* instance, lotus_fixedframe_callback callback) {
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->fixedframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte lotus_set_midframe(lotus_application* instance, lotus_midframe_callback callback) {
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->midframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte lotus_set_postframe(lotus_application* instance, lotus_postframe_callback callback) {
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->postframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte lotus_make_window(lotus_application* instance, char* title, ubyte4 location[2], ubyte4 size[2]) {
    lotus_set_log_level(LOTUS_LOG_FATAL);
    if (!instance || !instance->state.running) return LOTUS_FALSE;
    instance->resource.window = lotus_platform_make_window((title != NULL) ? title : instance->info.name, location[0], location[1], size[0], size[1]);

    if (!lotus_platform_make_glcontext(&instance->resource.window)) {
        lotus_log_fatal("Failed to create GL Context!");
        return LOTUS_FALSE;
    }
    
    if (!lotus_load_fptrs()) {
        lotus_log_fatal("Failed to load GL function pointers!");
        return LOTUS_FALSE;
    }

    return LOTUS_TRUE;
}

ubyte lotus_run_application(lotus_application* instance) {
    lotus_set_log_level(LOTUS_LOG_FATAL);
    ubyte result = LOTUS_FALSE;
    if (!instance || !instance->state.running) return result;
    
    while (instance->state.running) {
        if (instance->midframe_callback == NULL) {
            lotus_log_fatal("SoGL Application Missing Mid/Post Frame Callback Pointer!");
            return result;
        }
        
        if (instance->preframe_callback != NULL) {
            result = instance->preframe_callback();
        }
        
        if (instance->fixedframe_callback != NULL) {
            result = instance->fixedframe_callback();
        }

        result = instance->midframe_callback();
        
        if (instance->postframe_callback != NULL) {
            result = instance->postframe_callback();
        }
    }

    return result;
}

void lotus_exit_app(lotus_application* instance) {
    if (!instance) return;
    instance->info.name = "NULL";
    instance->preframe_callback = NULL;
    instance->midframe_callback = NULL;
    instance->postframe_callback = NULL;
    instance->fixedframe_callback = NULL;
    instance->state.running = LOTUS_FALSE;

    lotus_event_exit(&instance->state.event);
    lotus_platform_exit(&instance->state.platform);
}
