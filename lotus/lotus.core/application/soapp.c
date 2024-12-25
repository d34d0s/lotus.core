#include "soapp.h"

#include "../platform/sologger.h"

ubyte sogl_init_app(sogl_application* instance, char* name) {
    sogl_set_log_level(SOGL_LOG_FATAL);
    if (!instance) return SOGL_FALSE;
    if (!name) name = "SoGL Application";

    instance->info.name = name;
    
    if (!sogl_platform_init(&instance->state.platform)) {
        sogl_log_fatal("Failed to initialize platform layer!");
    }
    
    if (!sogl_event_init(&instance->state.event)) {
        sogl_log_fatal("Failed to initialize event layer!");
    }
    instance->preframe_callback = NULL;
    instance->midframe_callback = NULL;
    instance->postframe_callback = NULL;
    instance->fixedframe_callback = NULL;
    instance->state.running = SOGL_TRUE;

    return SOGL_TRUE;
}

ubyte sogl_set_preframe(sogl_application* instance, sogl_preframe_callback callback) {
    if (!instance || !instance->state.running) return SOGL_FALSE;
    instance->preframe_callback = callback;
    return SOGL_TRUE;
}

ubyte sogl_set_fixedframe(sogl_application* instance, sogl_fixedframe_callback callback) {
    if (!instance || !instance->state.running) return SOGL_FALSE;
    instance->fixedframe_callback = callback;
    return SOGL_TRUE;
}

ubyte sogl_set_midframe(sogl_application* instance, sogl_midframe_callback callback) {
    if (!instance || !instance->state.running) return SOGL_FALSE;
    instance->midframe_callback = callback;
    return SOGL_TRUE;
}

ubyte sogl_set_postframe(sogl_application* instance, sogl_postframe_callback callback) {
    if (!instance || !instance->state.running) return SOGL_FALSE;
    instance->postframe_callback = callback;
    return SOGL_TRUE;
}

ubyte sogl_make_window(sogl_application* instance, char* title, ubyte4 location[2], ubyte4 size[2]) {
    sogl_set_log_level(SOGL_LOG_FATAL);
    if (!instance || !instance->state.running) return SOGL_FALSE;
    instance->resource.window = sogl_platform_make_window((title != NULL) ? title : instance->info.name, location[0], location[1], size[0], size[1]);

    if (!sogl_platform_make_glcontext(&instance->resource.window)) {
        sogl_log_fatal("Failed to create GL Context!");
        return SOGL_FALSE;
    }
    
    if (!sogl_load_fptrs()) {
        sogl_log_fatal("Failed to load GL function pointers!");
        return SOGL_FALSE;
    }

    return SOGL_TRUE;
}

ubyte sogl_run_application(sogl_application* instance) {
    sogl_set_log_level(SOGL_LOG_FATAL);
    ubyte result = SOGL_FALSE;
    if (!instance || !instance->state.running) return result;
    
    while (instance->state.running) {
        if (instance->midframe_callback == NULL) {
            sogl_log_fatal("SoGL Application Missing Mid/Post Frame Callback Pointer!");
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

void sogl_exit_app(sogl_application* instance) {
    if (!instance) return;
    instance->info.name = "NULL";
    instance->preframe_callback = NULL;
    instance->midframe_callback = NULL;
    instance->postframe_callback = NULL;
    instance->fixedframe_callback = NULL;
    instance->state.running = SOGL_FALSE;

    sogl_event_exit(&instance->state.event);
    sogl_platform_exit(&instance->state.platform);
}
