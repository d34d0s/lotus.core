#pragma once

#include "../modules/lotus.core/types.h"
#include "../modules/lotus.core/defines.h"

#include "../modules/lotus.core/utility/lotus_math.h"
#include "../modules/lotus.core/utility/lotus_fileio.h"
#include "../modules/lotus.core/utility/lotus_array.h"
#include "../modules/lotus.core/utility/lotus_hashmap.h"    

#if defined(LOTUS_APPLICATION)
    #include "../modules/lotus.core/application/lotus_app.h"
#endif  // LOTUS_APPLICATION

#if defined(LOTUS_PLATFORM)
    #include "../modules/lotus.core/memory/lotus_memory.h"
    #include "../modules/lotus.core/platform/lotus_input.h"
    #include "../modules/lotus.core/platform/lotus_event.h"
    #include "../modules/lotus.core/platform/lotus_platform.h"
#endif  // LOTUS_PLATFORM

#if defined(LOTUS_GRAPHICS)
    #include "../modules/lotus.core/graphics/lotus_gfx.h"
    #include "../modules/lotus.core/graphics/lotus_glapi.h"
#endif // LOTUS_GRAPHICS

#if defined(LOTUS_ECS)
    #include "../modules/lotus.ECS/lotus_ecs.h"
#endif // LOTUS_ECS

#if defined(LOTUS_CORE)
    #include "../modules/lotus.core/application/lotus_app.h"

    #include "../modules/lotus.core/memory/lotus_memory.h"
    
    #include "../modules/lotus.core/platform/lotus_input.h"
    #include "../modules/lotus.core/platform/lotus_event.h"
    #include "../modules/lotus.core/platform/lotus_platform.h"
    
    #include "../modules/lotus.core/graphics/lotus_gfx.h"
    #include "../modules/lotus.core/graphics/lotus_glapi.h"
#endif  // LOTUS_CORE
