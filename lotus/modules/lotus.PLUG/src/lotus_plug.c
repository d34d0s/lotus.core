#include "../include/lotus_plug.h"

#include "../../lotus.CORE/include/platform/lotus_logger.h"

static Lotus_Plug_API internal_plug_api = {0};
static Lotus_Plug_Manager internal_plug_manager = {0};

// plug API implementation

Lotus_Plug_Manager* _plug_initialize_impl(void) {
    lotus_set_log_level(LOTUS_LOG_ERROR);
    internal_plug_manager.plug_count = 0;
    
    return &internal_plug_manager;
}

void _plug_shutdown_impl(Lotus_Platform_API* platform_api) {
    for (ubyte i = 0; i < internal_plug_manager.plug_count; i++) {
        Lotus_Plug* plug = &internal_plug_manager.plugs[i];
        plug->shutdown();

        plug->name = NULL;
        plug->shutdown = NULL;
        plug->initialize = NULL;
        
        platform_api->unload_library(&plug->library);
    }; internal_plug_manager.plug_count = 0;
}

sbyte _plug_load_impl(Lotus_Platform_API* platform_api, const char* path, const char* name) {
    lotus_set_log_level(LOTUS_LOG_ERROR);

    if (internal_plug_manager.plug_count >= LOTUS_ENGINE_PLUG_MAX) {
        lotus_log_error("Maximum loaded plugs reached");
        return -1;
    }

    ubyte plug_id = internal_plug_manager.plug_count;
    Lotus_Plug* plug = &internal_plug_manager.plugs[plug_id];
    
    plug->library = platform_api->load_library(path, name);
    if (!plug->library.handle) {
        lotus_log_error("Failed to load plug library: %s\n", name);
        return -1;
    }

    plug->name = name;
    
    plug->initialize;
    plug->shutdown;
    
    plug->function_count = 0;
    plug->functions;

    if (!plug->initialize()) {
        lotus_log_error("Failed to initialize plug: %s\n", name);
        platform_api->unload_library(&plug->library);
        return -1;
    }

    internal_plug_manager.plug_count++;

    lotus_set_log_level(LOTUS_LOG_INFO);
    lotus_log_info("Mod loaded: %s\n", name);

    return plug_id;
}

Lotus_Plug* _plug_get_impl(const char* name) {

}

ubyte _plug_register_function_impl(Lotus_Plug* plug, const char* name) {

}

Lotus_Function_Pointer _plug_get_function_impl(Lotus_Plug* plug, const char* name) {

}

ubyte _plug_unload_impl(Lotus_Platform_API* platform_api, const char* name) {
    lotus_set_log_level(LOTUS_LOG_WARNING);

    for (ubyte i = 0; i < internal_plug_manager.plug_count; i++) {
        Lotus_Plug* plug = &internal_plug_manager.plugs[i];
        if (strcmp(plug->name, name) == 0) {
            plug->shutdown();
            platform_api->unload_library(&plug->library);

            plug->name = NULL;
            plug->shutdown = NULL;
            plug->initialize = NULL;
            
            internal_plug_manager.plug_count--;
            
            lotus_set_log_level(LOTUS_LOG_INFO);
            lotus_log_info("Mod unloaded: %s\n", name);
            return LOTUS_TRUE;
        }
    }
    
    lotus_log_warning("Mod not found for unloading: %s\n", name);
    return LOTUS_FALSE;
}


Lotus_Plug_API* lotus_init_plug(void) {
    internal_plug_api = (Lotus_Plug_API) {
        .initialize = _plug_initialize_impl,
        .shutdown = _plug_shutdown_impl,
        .load_plug = _plug_load_impl,
        .get_plug = _plug_get_impl,
        .register_function = _plug_register_function_impl,
        .get_function = _plug_get_function_impl,
        .unload_plug = _plug_unload_impl
    };

    return &internal_plug_api;
}
