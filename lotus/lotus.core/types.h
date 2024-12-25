#pragma once

#include "defines.h"

typedef enum sogl_error_type {
    SOGL_ERR_NONE=0,
    SOGL_ERR_INIT,
    SOGL_ERR_TYPE,
    SOGL_ERR_FUNC,
    SOGL_ERR_MALLOC,
    SOGL_ERR_TYPES,
} sogl_error_type;

typedef ubyte4(*sogl_preframe_callback)(void);
typedef ubyte4(*sogl_fixedframe_callback)(void);
typedef ubyte4(*sogl_midframe_callback)(void);
typedef ubyte4(*sogl_postframe_callback)(void);

typedef struct sogl_application sogl_application;
