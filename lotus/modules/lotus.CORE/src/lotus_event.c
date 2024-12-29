#include "../include/platform/lotus_event.h"

static Lotus_Event_State internal_event_state = {0};

Lotus_Event_State* lotus_init_event() {
    internal_event_state.init = LOTUS_TRUE;
    return &internal_event_state;
}

void lotus_exit_event() {
    for (ubyte2 i = 0; i < LOTUS_EVENT_CODE_MAX; ++i) {
        lotus_destroy_array(internal_event_state.registered[i].events);
    }
}

ubyte lotus_push_event(Lotus_Event data, ubyte2 eventCode) {
    if (!internal_event_state.init) return LOTUS_FALSE;
    if (internal_event_state.registered[eventCode].events == NULL) return LOTUS_FALSE;

    ubyte8 count = LOTUS_ARRAY_GET_HEADER_FIELD(internal_event_state.registered[eventCode].events, LOTUS_ARRAY_LENGTH_FIELD);
    for (ubyte8 i = 0; i < count; ++i) {
        Lotus_Event event = internal_event_state.registered[eventCode].events[i];
        if(event.callback(data, eventCode)) {
            // event handled
            return LOTUS_TRUE;
        }
    }

    // not found.
    return LOTUS_FALSE;
}

ubyte lotus_register_event(ubyte2 eventCode, Lotus_Callback onEvent) {
    if (!internal_event_state.init) return LOTUS_FALSE;

    if (internal_event_state.registered[eventCode].events == NULL) {
        internal_event_state.registered[eventCode].events = lotus_make_array(sizeof(Lotus_Event), LOTUS_EVENT_CODE_MAX);
    }

    ubyte8 count = LOTUS_ARRAY_GET_HEADER_FIELD(internal_event_state.registered[eventCode].events, LOTUS_ARRAY_LENGTH_FIELD);

    // no duplicate event found, continue with registration
    ubyte8 index = (count > 0) ? count : 0;
    Lotus_Event event = internal_event_state.registered[eventCode].events[index];
    event.callback = onEvent;
    lotus_push_array(internal_event_state.registered[eventCode].events, &event);

    return LOTUS_TRUE;
}

ubyte lotus_unregister_event(ubyte2 eventCode, Lotus_Callback onEvent) {
    if (!internal_event_state.init) return LOTUS_FALSE;

    if (internal_event_state.registered[eventCode].events == NULL) {
        // TODO: warn
        return LOTUS_FALSE;
    }

    ubyte8 count = LOTUS_ARRAY_GET_HEADER_FIELD(internal_event_state.registered[eventCode].events, LOTUS_ARRAY_LENGTH_FIELD);
    for (ubyte8 i = 0; i < count; ++i) {
        internal_event_state.registered[eventCode].events[i] = (Lotus_Event){NULL};
        return LOTUS_TRUE;
    }

    // not found.
    return LOTUS_FALSE;
}
