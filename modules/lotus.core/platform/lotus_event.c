#include "lotus_event.h"

ubyte lotus_event_init(lotus_event_state* state) {
    state->init = LOTUS_TRUE;
    return LOTUS_TRUE;
}

void lotus_event_exit(lotus_event_state* state) {
    for (ubyte2 i = 0; i < LOTUS_EVENT_CODE_MAX; ++i) {
        lotus_destroy_array(&state->alloc, state->registered[i].events);
    }
}

ubyte lotus_push_event(lotus_event_state* state, lotus_event data, ubyte2 eventCode) {
    if (!state->init) return LOTUS_FALSE;
    if (state->registered[eventCode].events.header == NULL) return LOTUS_FALSE;

    ubyte8 count = lotus_get_array_attr(state->registered[eventCode].events, LOTUS_ARRAY_SIZE);
    for (ubyte8 i = 0; i < count; ++i) {
        lotus_event event = ((lotus_event*)state->registered[eventCode].events.content)[i];
        if(event.callback(data, eventCode)) {
            // event handled
            return LOTUS_TRUE;
        }
    }

    // not found.
    return LOTUS_FALSE;
}

ubyte lotus_register_event(lotus_event_state* state, ubyte2 eventCode, lotus_callback onEvent) {
    if (!state->init) return LOTUS_FALSE;

    if (state->registered[eventCode].events.content == NULL) {
        state->registered[eventCode].events = lotus_make_array(&state->alloc, LOTUS_EVENT_CODE_MAX, sizeof(lotus_event));
    }

    ubyte8 count = lotus_get_array_attr(state->registered[eventCode].events, LOTUS_ARRAY_SIZE);

    // no duplicate event found, continue with registration
    ubyte8 index = (count > 0) ? count : 0;
    lotus_event event = ((lotus_event*)state->registered[eventCode].events.content)[index];
    event.callback = onEvent;
    lotus_push_array(&state->alloc, &state->registered[eventCode].events, LOTUS_MEMORY_REF(event));

    return LOTUS_TRUE;
}

ubyte lotus_unregister_event(lotus_event_state* state, ubyte2 eventCode, lotus_callback onEvent) {
    if (!state->init) return LOTUS_FALSE;

    if (state->registered[eventCode].events.content == NULL) {
        // TODO: warn
        return LOTUS_FALSE;
    }

    ubyte8 count = lotus_get_array_attr(state->registered[eventCode].events, LOTUS_ARRAY_SIZE);
    for (ubyte8 i = 0; i < count; ++i) {
        lotus_event event = ((lotus_event*)state->registered[eventCode].events.content)[i];
        lotus_free(&state->alloc, LOTUS_MEMORY(event));
        return LOTUS_TRUE;
    }

    // not found.
    return LOTUS_FALSE;
}
