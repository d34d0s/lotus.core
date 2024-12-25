#pragma once

#include "../defines.h"
#include "../memory/somemory.h"
#include "../utility/soarray.h"

#define SOGL_EVENT_CODE_MAX 16384

typedef struct sogl_event sogl_event;
typedef ubyte (*sogl_callback)(sogl_event data, ubyte2 eventCode);

// internal event codes. user application should use codes > 255.
typedef enum sogl_event_code {
    // shuts down the application on the next frame.
    SOGL_EVENT_APP_QUIT = 0x01,

    // keyboard key pressed.
    /* Event Data Usage:
        * ubyte2 keyCode = data.data.ubyte2[0];
    */
    SOGL_EVENT_KEY_PRESSED = 0x02,
    

    // keyboard key released.
    /* Event Data Usage:
        * ubyte2 keyCode = data.data.ubyte2[0];
    */
    SOGL_EVENT_KEY_RELEASED = 0x03,

    // mouse button pressed.
    /* Event Data Usage:
        * ubyte2 button = data.data.ubyte2[0];
    */
    SOGL_EVENT_BUTTON_PRESSED = 0x04,

    // mouse button released.
    /* Event Data Usage:
        * ubyte2 button = data.data.ubyte2[0];
    */
    SOGL_EVENT_BUTTON_RELEASED = 0x05,

    // mouse movement.
    /* Event Data Usage:
        * ubyte2 x = data.data.ubyte2[0];
        * ubyte2 y = data.data.ubyte2[1];
    */
    SOGL_EVENT_MOUSE_MOVE = 0x06,

    // mouse movement.
    /* Event Data Usage:
        * ubyte zDelta = data.data.ubyte[0];
    */
    SOGL_EVENT_MOUSE_WHEEL = 0x07,

    /* OS resize/resolution change. (deprecated)
    * Event Data Usage:
        * ubyte2 width = data.data.ubyte2[0];
        * ubyte2 height = data.data.ubyte2[1];
    */
    SOGL_EVENT_RESIZE = 0x08,

    SOGL_EVENT_MAX = 0xFF
} sogl_event_code;

struct sogl_event {
    sogl_callback callback;
    // 128 byte maximum
    union data {
        sbyte8 sbyte8[2];
        ubyte8 ubyte8[2];
        f64 f64[2];

        sbyte4 sbyte4[4];
        ubyte4 ubyte4[4];
        f32 f32[4];

        sbyte2 sbyte2[8];
        ubyte2 ubyte2[8];
        
        sbyte sbyte[16];
        ubyte ubyte[16];
        char c[16];
    } data;    
};

typedef struct sogl_event_register {
    sogl_array events;
} sogl_event_register;

typedef struct sogl_event_state {
    ubyte init;
    sogl_allocator alloc;
    // event code lookup-table.
    sogl_event_register registered[SOGL_EVENT_CODE_MAX];
} sogl_event_state;

ubyte sogl_event_init(sogl_event_state* state);
void sogl_event_exit(sogl_event_state* state);

/**
 * Push an event to listeners of the specified event code. If an event handler returns
 * `SOGL_TRUE`, the event is considered handled and is passed no further.
 * @param data The event data.
 * @param eventCode The event code to push.
 * @param sender A pointer to the sender can be 0/NULL.
 * @returns `SOGL_TRUE` if handles; otherwise `SOGL_FALSE`
 */
SOGL_API_ENTRY ubyte sogl_push_event(sogl_event_state* state, sogl_event data, ubyte2 eventCode);

/**
 * Register to listen for when events are sent with the specified event code. Events with duplicate
 * listener/callback combinations will not be re-registered and will cause this function to return `SOGL_FALSE`.
 * @param eventCode The event code to listen for.
 * @param listener A pointer to a listener instance. Can be 0/NULL.
 * @param onEvent The callback function pointer to be invoked when the event code is pushed.
 * @returns `SOGL_TRUE` if the event is successfully registered; otherwise `SOGL_FALSE`.
 */
SOGL_API_ENTRY ubyte sogl_register_event(sogl_event_state* state, ubyte2 eventCode, sogl_callback onEvent);

/**
 * Unregister from listening for when events are sent with the specified event code. If no matching
 * registration is found, this function returns `SOGL_FALSE`.
 * @param eventCode The event code to stop listening for.
 * @param listener A pointer to a listener instance. Can be 0/NULL.
 * @param onEvent The callback function pointer to be unregistered.
 * @returns `SOGL_TRUE` if the event is successfully unregistered; otherwise `SOGL_FALSE`.
 */
SOGL_API_ENTRY ubyte sogl_unregister_event(sogl_event_state* state, ubyte2 eventCode, sogl_callback onEvent);

