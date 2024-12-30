#include "../include/application/lotus_app.h"

#include "../include/graphics/lotus_glapi.h"
#include "../include/platform/lotus_logger.h"

// internal application instance pointer
static Lotus_Application_API internal_application_api;
static Lotus_Application internal_application_instance;

// default application event callbacks
ubyte quit_callback(Lotus_Event data, ubyte2 event_code) {
    if (event_code == LOTUS_EVENT_APP_QUIT) {
        internal_application_instance.state.running = LOTUS_FALSE;
        return LOTUS_FALSE;
    }; return LOTUS_TRUE;
}


// application API implementation
ubyte _application_set_preframe_impl(Lotus_Application_Preframe_Callback callback) {
    if (!internal_application_instance.state.running) return LOTUS_FALSE;
    internal_application_instance.preframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte _application_set_fixedframe_impl(Lotus_Application_Fixedframe_Callback callback) {
    if (!internal_application_instance.state.running) return LOTUS_FALSE;
    internal_application_instance.fixedframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte _application_set_midframe_impl(Lotus_Application_Midframe_Callback callback) {
    if (!internal_application_instance.state.running) return LOTUS_FALSE;
    internal_application_instance.midframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte _application_set_postframe_impl(Lotus_Application_Postframe_Callback callback) {
    if (!internal_application_instance.state.running) return LOTUS_FALSE;
    internal_application_instance.postframe_callback = callback;
    return LOTUS_TRUE;
}

ubyte _application_create_window_impl(const char* title, ubyte4 size[2]) {
    lotus_set_log_level(LOTUS_LOG_FATAL);
    if (!internal_application_instance.state.running) return LOTUS_FALSE;
    internal_application_instance.resource.window = internal_application_instance.resource.platform_api->create_window((title != NULL) ? title : internal_application_instance.info.name, size[0], size[1]);

    if (!internal_application_instance.resource.platform_api->create_gl_context(&internal_application_instance.resource.window)) {
        lotus_log_fatal("Failed to create GL Context!");
        return LOTUS_FALSE;
    }
    
    if (!lotus_load_fptrs()) {
        lotus_log_fatal("Failed to load GL function pointers!");
        return LOTUS_FALSE;
    }

    return LOTUS_TRUE;
}

void _application_destroy_window_impl(void) {
    if (!internal_application_instance.state.running) return;
    internal_application_instance.resource.platform_api->destroy_window(&internal_application_instance.resource.window);
    internal_application_instance.resource.platform_api->destroy_gl_context(&internal_application_instance.resource.window);
}

ubyte _application_run_impl(void) {
    ubyte result = LOTUS_FALSE;
    if (!internal_application_instance.state.running) return result;
    
    lotus_set_log_level(LOTUS_LOG_FATAL);
    while (internal_application_instance.state.running) {
        if (internal_application_instance.midframe_callback == NULL) {
            lotus_log_fatal("Lotus Application Missing Mid-Frame Callback Pointer!");
            return result;
        }
        
        // preframe logic
        internal_application_instance.resource.platform_api->poll_events();
        if (internal_application_instance.preframe_callback != NULL) {
            result = internal_application_instance.preframe_callback();
        }
        
        if (internal_application_instance.fixedframe_callback != NULL) {
            result = internal_application_instance.fixedframe_callback();
        }

        result = internal_application_instance.midframe_callback();
        lotus_draw_flush();
        
        // postframe logic
        if (internal_application_instance.postframe_callback != NULL) {
            result = internal_application_instance.postframe_callback();
        }
        internal_application_instance.resource.platform_api->swap_buffers(&internal_application_instance.resource.window);
        lotus_update_input(0);

    }; internal_application_api.shutdown();

    return result;
}

sbyte _application_create_scene_impl(const char* scene_name) {
    if (internal_application_instance.state.scene_count + 1 > LOTUS_ENGINE_SCENE_MAX) { return -1; }

    ubyte scene_id = internal_application_instance.state.scene_count++;
    internal_application_instance.resource.scene_map[scene_id] = lotus_init_scene(scene_id, scene_name);

    return scene_id;
}

Lotus_Scene* _application_get_scene_impl(ubyte scene_id) {
    if (scene_id > LOTUS_ENGINE_SCENE_MAX) return NULL;
    Lotus_Scene* scene = internal_application_instance.resource.scene_map[scene_id];
    return scene;
}

void _application_destroy_scene_impl(ubyte scene_id) {
    if (scene_id > LOTUS_ENGINE_SCENE_MAX || internal_application_instance.state.scene_count - 1 < 0) { return; }
    internal_application_instance.state.scene_count--;
    lotus_destroy_scene(internal_application_instance.resource.scene_map[scene_id]);
}

Lotus_Application* _application_initialize_impl(const char* app_name, ubyte4 window_size[2]) {
    lotus_set_log_level(LOTUS_LOG_FATAL);
    if (!app_name) app_name = "Lotus Application";

    internal_application_instance.info.name = app_name;
    internal_application_instance.info.window_size[0] = window_size[0];
    internal_application_instance.info.window_size[1] = window_size[1];
    
    internal_application_instance.state.running = LOTUS_TRUE;
    
    internal_application_instance.resource.allocator = lotus_make_allocator(LOTUS_DEFAULT_APPLICATION_HEAP_SIZE);

    internal_application_instance.resource.platform_api = lotus_init_platform();
    if (!internal_application_instance.resource.platform_api) {
        lotus_log_fatal("Failed to initialize platform api!");
        return NULL;
    }

    // register default event callbacks
    // TODO: resize event
    lotus_register_event(LOTUS_EVENT_APP_QUIT, quit_callback);

    if (!_application_create_window_impl(app_name, window_size)) {
        lotus_log_fatal("Failed to create application window!");
        return NULL;
    }

    lotus_init_renderer();
    
    internal_application_instance.resource.plug_api = lotus_init_plug();
    if (!internal_application_instance.resource.plug_api) {
        lotus_log_fatal("Failed to initialize plug api!");
        return NULL;
    }

    internal_application_instance.resource.plug_manager = internal_application_instance.resource.plug_api->initialize();
    if (!internal_application_instance.resource.plug_manager) {
        lotus_log_fatal("Failed to initialize plug manager!");
        return NULL;
    }

    internal_application_instance.preframe_callback = NULL;
    internal_application_instance.midframe_callback = NULL;
    internal_application_instance.postframe_callback = NULL;
    internal_application_instance.fixedframe_callback = NULL;

    return &internal_application_instance;
}

void _application_shutdown_impl(void) {
    internal_application_instance.state.running = LOTUS_FALSE;
    internal_application_instance.info.name = "NULL";

    internal_application_instance.resource.plug_api->shutdown(internal_application_instance.resource.platform_api);

    for (ubyte i = 0; i < internal_application_instance.state.scene_count; i++) {
        internal_application_api.destroy_scene(i);
    }

    internal_application_api.destroy_window();

    lotus_destroy_renderer();

    internal_application_instance.preframe_callback = NULL;
    internal_application_instance.midframe_callback = NULL;
    internal_application_instance.postframe_callback = NULL;
    internal_application_instance.fixedframe_callback = NULL;

    internal_application_instance.resource.platform_api->shutdown();
    internal_application_instance.resource.platform_api = NULL;
}


Lotus_Application_API* lotus_init_application(void) {
    internal_application_api = (Lotus_Application_API) {
        .initialize = _application_initialize_impl,
        .shutdown = _application_shutdown_impl,
        
        .create_window = _application_create_window_impl,
        .destroy_window = _application_destroy_window_impl,
        
        .create_scene = _application_create_scene_impl,
        .get_scene = _application_get_scene_impl,
        .destroy_scene = _application_destroy_scene_impl,
        
        .set_preframe = _application_set_preframe_impl,
        .set_fixedframe = _application_set_fixedframe_impl,
        .set_midframe = _application_set_midframe_impl,
        .set_postframe = _application_set_postframe_impl,
        .run = _application_run_impl
    };

    return &internal_application_api;
}
