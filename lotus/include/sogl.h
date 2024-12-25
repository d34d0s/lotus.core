#pragma once

#include "../lotus.core/types.h"
#include "../lotus.core/defines.h"

#ifdef SOGL_APPLICATION
    #include "../lotus.core/application/soapp.h"
#else 
#ifdef SOGL_PLATFORM
    #include "../lotus.core/memory/somemory.h"
    #include "../lotus.core/platform/soinput.h"
    #include "../lotus.core/platform/soevent.h"
    #include "../lotus.core/platform/soplatform.h"
#else
#ifdef SOGL_GRAPHICS
    #include "../lotus.core/graphics/sogfx.h"
    #include "../lotus.core/graphics/soglapi.h"
#endif // SOGL_GRAPHICS
#endif  // SOGL_PLATFORM
#endif  // SOGL_APPLICATION

/* define any of these directives to include sogl utilities from this header */
#if defined(SOGL_MATH) || defined(SOGL_UTILS)
    #include "../lotus.core/utility/somath.h"
#if defined(SOGL_FILEIO) || defined(SOGL_UTILS)
    #include "../lotus.core/utility/sofileio.h"
#if defined(SOGL_DATASTRUCTS) || defined(SOGL_UTILS)
    #include "../lotus.core/utility/soarray.h"
    #include "../lotus.core/utility/sohashmap.h"
#endif  // SOGL_DATASTRUCTS
#endif  // SOGL_FILEIO
#endif  // SOGL_MATH

#ifdef SOGL_CORE
    #include "../lotus.core/application/soapp.h"

    #include "../lotus.core/memory/somemory.h"
    
    #include "../lotus.core/platform/soinput.h"
    #include "../lotus.core/platform/soevent.h"
    #include "../lotus.core/platform/soplatform.h"
    
    #include "../lotus.core/graphics/sogfx.h"
    #include "../lotus.core/graphics/soglapi.h"
    
    #include "../lotus.core/utility/somath.h"
    #include "../lotus.core/utility/sofileio.h"
    #include "../lotus.core/utility/soarray.h"
    #include "../lotus.core/utility/sohashmap.h"    
#endif  // SOGL_CORE
