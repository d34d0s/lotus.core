#include "soevent.h"

ubyte sogl_event_init(sogl_event_state* state) {
    state->init = SOGL_TRUE;
    return SOGL_TRUE;
}

void sogl_event_exit(sogl_event_state* state) {
    for (ubyte2 i = 0; i < SOGL_EVENT_CODE_MAX; ++i) {
        sogl_destroy_array(&state->alloc, state->registered[i].events);
    }
}

ubyte sogl_push_event(sogl_event_state* state, sogl_event data, ubyte2 eventCode) {
    if (!state->init) return SOGL_FALSE;
    if (state->registered[eventCode].events.header == NULL) return SOGL_FALSE;

    ubyte8 count = sogl_get_array_attr(state->registered[eventCode].events, SOGL_ARRAY_SIZE);
    for (ubyte8 i = 0; i < count; ++i) {
        sogl_event event = ((sogl_event*)state->registered[eventCode].events.content)[i];
        if(event.callback(data, eventCode)) {
            // event handled
            return SOGL_TRUE;
        }
    }

    // not found.
    return SOGL_FALSE;
}

ubyte sogl_register_event(sogl_event_state* state, ubyte2 eventCode, sogl_callback onEvent) {
    if (!state->init) return SOGL_FALSE;

    if (state->registered[eventCode].events.content == NULL) {
        state->registered[eventCode].events = sogl_make_array(&state->alloc, SOGL_EVENT_CODE_MAX, sizeof(sogl_event));
    }

    ubyte8 count = sogl_get_array_attr(state->registered[eventCode].events, SOGL_ARRAY_SIZE);

    // no duplicate event found, continue with registration
    ubyte8 index = (count > 0) ? count : 0;
    sogl_event event = ((sogl_event*)state->registered[eventCode].events.content)[index];
    event.callback = onEvent;
    sogl_push_array(&state->alloc, &state->registered[eventCode].events, SOGL_MEMORY_REF(event));

    return SOGL_TRUE;
}

ubyte sogl_unregister_event(sogl_event_state* state, ubyte2 eventCode, sogl_callback onEvent) {
    if (!state->init) return SOGL_FALSE;

    if (state->registered[eventCode].events.content == NULL) {
        // TODO: warn
        return SOGL_FALSE;
    }

    ubyte8 count = sogl_get_array_attr(state->registered[eventCode].events, SOGL_ARRAY_SIZE);
    for (ubyte8 i = 0; i < count; ++i) {
        sogl_event event = ((sogl_event*)state->registered[eventCode].events.content)[i];
        sogl_free(&state->alloc, SOGL_MEMORY(event));
        return SOGL_TRUE;
    }

    // not found.
    return SOGL_FALSE;
}
