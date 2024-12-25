#ifndef LTLOGGER_H
#define LTLOGGER_H

#include "../defines.h"
#include "soplatform.h"

typedef enum sogl_log_level {
    SOGL_LOG_FATAL,
    SOGL_LOG_ERROR,
    SOGL_LOG_WARNING,
    SOGL_LOG_INFO,
    SOGL_LOG_DEBUG,
    SOGL_LOG_TRACE
} sogl_log_level;

static sogl_log_level currentLogLevel = SOGL_LOG_INFO;

// Log level colors: fatal, error, warning, info, debug, trace
static const ubyte logLevelColors[] = {0, 1, 2, 3, 4, 5};

static sogl_log_level sogl_get_log_level() { return currentLogLevel; }
static void sogl_set_log_level(sogl_log_level level) { currentLogLevel = level; }

static void sogl_log_message(sogl_log_level level, const char *message, ...) {
    if (level > currentLogLevel) return;

    va_list args;
    va_start(args, message);

    char buffer[1024];
    if (vsnprintf(buffer, sizeof(buffer), message, args) >= sizeof(buffer)) {
        printf("Log message truncated!\n");
    }
    va_end(args);

    printf("%s\n", buffer);
}

#define sogl_log_if_enabled(LEVEL, MESSAGE, ...) \
    sogl_log_message(LEVEL, "|" #LEVEL "| :: " MESSAGE, ##__VA_ARGS__)

#define sogl_log_fatal(MESSAGE, ...) \
    sogl_log_if_enabled(SOGL_LOG_FATAL, MESSAGE, ##__VA_ARGS__)

#define sogl_log_error(MESSAGE, ...) \
    sogl_log_if_enabled(SOGL_LOG_ERROR, MESSAGE, ##__VA_ARGS__)

#define sogl_log_warning(MESSAGE, ...) \
    sogl_log_if_enabled(SOGL_LOG_WARNING, MESSAGE, ##__VA_ARGS__)

#define sogl_log_info(MESSAGE, ...) \
    sogl_log_if_enabled(SOGL_LOG_INFO, MESSAGE, ##__VA_ARGS__)

#define sogl_log_debug(MESSAGE, ...) \
    sogl_log_if_enabled(SOGL_LOG_DEBUG, MESSAGE, ##__VA_ARGS__)

#define sogl_log_trace(MESSAGE, ...) \
    sogl_log_if_enabled(SOGL_LOG_TRACE, MESSAGE, ##__VA_ARGS__)


#endif