#pragma once

#include "../modules/lotus.core/types.h"
#include "../modules/lotus.core/defines.h"

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

/* define any of these directives to include lotus utilities from this header */
#if defined(LOTUS_MATH) || defined(LOTUS_UTILS)
    #include "../modules/lotus.core/utility/lotus_math.h"
#endif  // LOTUS_MATH

#if defined(LOTUS_FILEIO) || defined(LOTUS_UTILS)
    #include "../modules/lotus.core/utility/lotus_fileio.h"
#endif  // LOTUS_FILEIO

#if defined(LOTUS_DATASTRUCTS) || defined(LOTUS_UTILS)
    #include "../modules/lotus.core/utility/lotus_array.h"
    #include "../modules/lotus.core/utility/lotus_hashmap.h"
#endif  // LOTUS_DATASTRUCTS

#if defined(LOTUS_CORE)
    #include "../modules/lotus.core/application/lotus_app.h"

    #include "../modules/lotus.core/memory/lotus_memory.h"
    
    #include "../modules/lotus.core/platform/lotus_input.h"
    #include "../modules/lotus.core/platform/lotus_event.h"
    #include "../modules/lotus.core/platform/lotus_platform.h"
    
    #include "../modules/lotus.core/graphics/lotus_gfx.h"
    #include "../modules/lotus.core/graphics/lotus_glapi.h"
    
    #include "../modules/lotus.core/utility/lotus_math.h"
    #include "../modules/lotus.core/utility/lotus_fileio.h"
    #include "../modules/lotus.core/utility/lotus_array.h"
    #include "../modules/lotus.core/utility/lotus_hashmap.h"    
#endif  // LOTUS_CORE
