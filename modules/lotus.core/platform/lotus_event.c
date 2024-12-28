#include "lotus_event.h"

static lotus_event_state internal_event_state = {0};

lotus_event_state* lotus_init_event() {
    internal_event_state.init = LOTUS_TRUE;
    return &internal_event_state;
}

void lotus_exit_event() {
    for (ubyte2 i = 0; i < LOTUS_EVENT_CODE_MAX; ++i) {
        lotus_destroy_array(internal_event_state.registered[i].events);
    }
}

ubyte lotus_push_event(lotus_event data, ubyte2 eventCode) {
    if (!internal_event_state.init) return LOTUS_FALSE;
    if (internal_event_state.registered[eventCode].events == NULL) return LOTUS_FALSE;

    ubyte8 count = LOTUS_ARRAY_GET_HEADER_FIELD(internal_event_state.registered[eventCode].events, LOTUS_ARRAY_LENGTH_FIELD);
    for (ubyte8 i = 0; i < count; ++i) {
        lotus_event event = internal_event_state.registered[eventCode].events[i];
        if(event.callback(data, eventCode)) {
            // event handled
            return LOTUS_TRUE;
        }
    }

    // not found.
    return LOTUS_FALSE;
}

ubyte lotus_register_event(ubyte2 eventCode, lotus_callback onEvent) {
    if (!internal_event_state.init) return LOTUS_FALSE;

    if (internal_event_state.registered[eventCode].events == NULL) {
        internal_event_state.registered[eventCode].events = lotus_make_array(sizeof(lotus_event), LOTUS_EVENT_CODE_MAX);
    }

    ubyte8 count = LOTUS_ARRAY_GET_HEADER_FIELD(internal_event_state.registered[eventCode].events, LOTUS_ARRAY_LENGTH_FIELD);

    // no duplicate event found, continue with registration
    ubyte8 index = (count > 0) ? count : 0;
    lotus_event event = internal_event_state.registered[eventCode].events[index];
    event.callback = onEvent;
    lotus_push_array(internal_event_state.registered[eventCode].events, &event);

    return LOTUS_TRUE;
}

ubyte lotus_unregister_event(ubyte2 eventCode, lotus_callback onEvent) {
    if (!internal_event_state.init) return LOTUS_FALSE;

    if (internal_event_state.registered[eventCode].events == NULL) {
        // TODO: warn
        return LOTUS_FALSE;
    }

    ubyte8 count = LOTUS_ARRAY_GET_HEADER_FIELD(internal_event_state.registered[eventCode].events, LOTUS_ARRAY_LENGTH_FIELD);
    for (ubyte8 i = 0; i < count; ++i) {
        internal_event_state.registered[eventCode].events[i] = (lotus_event){NULL};
        return LOTUS_TRUE;
    }

    // not found.
    return LOTUS_FALSE;
}
