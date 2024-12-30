// lotus.PLUG/include/lotus_plug.h

/*
    LOTUS ENGINE PLUGIN/MODDING API
*/ 

#pragma once

#include "../../lotus.CORE/include/defines.h"
#include "../../lotus.CORE/include/platform/lotus_platform.h"

#define LOTUS_ENGINE_PLUG_MAX    ((1U << 8) - 1)

typedef void (*Lotus_Function_Pointer)(void);
typedef struct Lotus_Plug_Function {
    const char* name;
    Lotus_Function_Pointer ptr;
} Lotus_Plug_Function;

typedef ubyte (*Lotus_Plug_Initialize)(void);
typedef void (*Lotus_Plug_Shutdown)(void);
typedef struct Lotus_Plug {
    const char* name;
    Lotus_DyLib library;
    Lotus_Plug_Shutdown shutdown;
    Lotus_Plug_Initialize initialize;

    ubyte function_count;
    Lotus_Function_Pointer* functions;
} Lotus_Plug;

typedef struct Lotus_Plug_Manager {
    ubyte plug_count;
    Lotus_Plug plugs[LOTUS_ENGINE_PLUG_MAX];
} Lotus_Plug_Manager;

typedef struct Lotus_Plug_API {
    Lotus_Plug_Manager* (*initialize)(void);
    void (*shutdown)(Lotus_Platform_API* platform_api);

    sbyte (*load_plug)(Lotus_Platform_API* platform_api, const char* path, const char* name);
    Lotus_Plug* (*get_plug)(const char* name);
    ubyte (*register_function)(Lotus_Plug* plug, const char* name);
    Lotus_Function_Pointer (*get_function)(Lotus_Plug* plug, const char* name);
    ubyte (*unload_plug)(Lotus_Platform_API* platform_api, const char* name);
} Lotus_Plug_API;

Lotus_Plug_API* lotus_init_plug(void);
