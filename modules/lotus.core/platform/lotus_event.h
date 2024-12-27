#pragma once

#include "../defines.h"
#include "../memory/lotus_memory.h"
#include "../utility/lotus_array.h"

#define LOTUS_EVENT_CODE_MAX 16384

typedef struct lotus_event lotus_event;
typedef ubyte (*lotus_callback)(lotus_event data, ubyte2 eventCode);

// internal event codes. user application should use codes > 255.
typedef enum lotus_event_code {
    // shuts down the application on the next frame.
    LOTUS_EVENT_APP_QUIT = 0x01,

    // keyboard key pressed.
    /* Event Data Usage:
        * ubyte2 keyCode = data.data.ubyte2[0];
    */
    LOTUS_EVENT_KEY_PRESSED = 0x02,
    

    // keyboard key released.
    /* Event Data Usage:
        * ubyte2 keyCode = data.data.ubyte2[0];
    */
    LOTUS_EVENT_KEY_RELEASED = 0x03,

    // mouse button pressed.
    /* Event Data Usage:
        * ubyte2 button = data.data.ubyte2[0];
    */
    LOTUS_EVENT_BUTTON_PRESSED = 0x04,

    // mouse button released.
    /* Event Data Usage:
        * ubyte2 button = data.data.ubyte2[0];
    */
    LOTUS_EVENT_BUTTON_RELEASED = 0x05,

    // mouse movement.
    /* Event Data Usage:
        * ubyte2 x = data.data.ubyte2[0];
        * ubyte2 y = data.data.ubyte2[1];
    */
    LOTUS_EVENT_MOUSE_MOVE = 0x06,

    // mouse movement.
    /* Event Data Usage:
        * ubyte zDelta = data.data.ubyte[0];
    */
    LOTUS_EVENT_MOUSE_WHEEL = 0x07,

    /* OS resize/resolution change. (deprecated)
    * Event Data Usage:
        * ubyte2 width = data.data.ubyte2[0];
        * ubyte2 height = data.data.ubyte2[1];
    */
    LOTUS_EVENT_RESIZE = 0x08,

    LOTUS_EVENT_MAX = 0xFF
} lotus_event_code;

struct lotus_event {
    lotus_callback callback;
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

typedef struct lotus_event_register {
    lotus_event* events;
} lotus_event_register;

typedef struct lotus_event_state {
    ubyte init;
    // event code lookup-table.
    lotus_event_register registered[LOTUS_EVENT_CODE_MAX];
} lotus_event_state;

lotus_event_state* lotus_init_event();
void lotus_exit_event();

/**
 * Push an event to listeners of the specified event code. If an event handler returns
 * `LOTUS_TRUE`, the event is considered handled and is passed no further.
 * @param data The event data.
 * @param eventCode The event code to push.
 * @param sender A pointer to the sender can be 0/NULL.
 * @returns `LOTUS_TRUE` if handles; otherwise `LOTUS_FALSE`
 */
LOTUS_API_ENTRY ubyte lotus_push_event(lotus_event data, ubyte2 eventCode);

/**
 * Register to listen for when events are sent with the specified event code. Events with duplicate
 * listener/callback combinations will not be re-registered and will cause this function to return `LOTUS_FALSE`.
 * @param eventCode The event code to listen for.
 * @param listener A pointer to a listener instance. Can be 0/NULL.
 * @param onEvent The callback function pointer to be invoked when the event code is pushed.
 * @returns `LOTUS_TRUE` if the event is successfully registered; otherwise `LOTUS_FALSE`.
 */
LOTUS_API_ENTRY ubyte lotus_register_event(ubyte2 eventCode, lotus_callback onEvent);

/**
 * Unregister from listening for when events are sent with the specified event code. If no matching
 * registration is found, this function returns `LOTUS_FALSE`.
 * @param eventCode The event code to stop listening for.
 * @param listener A pointer to a listener instance. Can be 0/NULL.
 * @param onEvent The callback function pointer to be unregistered.
 * @returns `LOTUS_TRUE` if the event is successfully unregistered; otherwise `LOTUS_FALSE`.
 */
LOTUS_API_ENTRY ubyte lotus_unregister_event(ubyte2 eventCode, lotus_callback onEvent);

