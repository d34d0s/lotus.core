#include "lotus_input.h"

#include "lotus_event.h"
#include "../memory/lotus_memory.h"

static lotus_input_state internal_input_state = {0};

lotus_input_state* lotus_input_init(void) {
    internal_input_state.init = LOTUS_TRUE;
    return &internal_input_state;
}

void lotus_input_exit(void) {
    internal_input_state.init = LOTUS_FALSE;
}

void lotus_input_update(f64 delta_time) {
    if (!internal_input_state.init) return;
    memcpy(&internal_input_state.previous, &internal_input_state.current, sizeof(_lotus_input_state));
}

ubyte lotus_key_is_up(lotus_keyboard_key key) {
    if (!internal_input_state.init) return LOTUS_FALSE;
    return internal_input_state.current.keys[key] == LOTUS_FALSE;
}

ubyte lotus_key_was_up(lotus_keyboard_key key) {
    if (!internal_input_state.init) return LOTUS_FALSE;
    return internal_input_state.previous.keys[key] == LOTUS_FALSE;
}

ubyte lotus_key_is_down(lotus_keyboard_key key) {
    if (!internal_input_state.init) return LOTUS_FALSE;
    return internal_input_state.current.keys[key] == LOTUS_TRUE;
}

ubyte lotus_key_was_down(lotus_keyboard_key key) {
    if (!internal_input_state.init) return LOTUS_FALSE;
    return internal_input_state.previous.keys[key] == LOTUS_TRUE;
}

ubyte lotus_button_is_up(lotus_mouse_button button) {
    if (!internal_input_state.init) return LOTUS_FALSE;
    return internal_input_state.current.buttons[button] == LOTUS_FALSE;
}

ubyte lotus_button_was_up(lotus_mouse_button button) {
    if (!internal_input_state.init) return LOTUS_FALSE;
    return internal_input_state.previous.buttons[button] == LOTUS_FALSE;
}

ubyte lotus_button_is_down(lotus_mouse_button button) {
    if (!internal_input_state.init) return LOTUS_FALSE;
    return internal_input_state.current.buttons[button] == LOTUS_TRUE;
}

ubyte lotus_button_was_down(lotus_mouse_button button) {
    if (!internal_input_state.init) return LOTUS_FALSE;
    return internal_input_state.previous.buttons[button] == LOTUS_TRUE;
}

void lotus_mouse_get_position(sbyte4* x, sbyte4* y) {
    if (!internal_input_state.init) {
        *x = 0; *y = 0;
        return;
    }; *x = internal_input_state.current.mouse[0]; *y = internal_input_state.current.mouse[1];
}

void lotus_mouse_get_last_position(sbyte4* x, sbyte4* y) {
    if (!internal_input_state.init) {
        *x = 0; *y = 0;
        return;
    }; *x = internal_input_state.previous.mouse[0]; *y = internal_input_state.previous.mouse[1];
}

void lotus_process_mouse_wheel(sbyte z_delotus_a) {
    lotus_event event;
    event.data.sbyte[0] = z_delotus_a;
    lotus_push_event(event, LOTUS_EVENT_MOUSE_WHEEL);
}

void lotus_process_mouse_move(sbyte2 x, sbyte2 y) {
    if (internal_input_state.current.mouse[0] != x || internal_input_state.current.mouse[1] != y) {
        internal_input_state.current.mouse[0] = x;
        internal_input_state.current.mouse[1] = y;

        lotus_event event;
        event.data.ubyte2[0] = x;
        event.data.ubyte2[1] = y;
        lotus_push_event(event, LOTUS_EVENT_MOUSE_MOVE);
    }
}

void lotus_process_key(lotus_keyboard_key key, ubyte pressed) {
    if (internal_input_state.current.keys[key] != pressed) {
        internal_input_state.current.keys[key] = pressed;

        lotus_event event;
        event.data.ubyte2[0] = key;
        lotus_push_event(event, pressed ? LOTUS_EVENT_KEY_PRESSED : LOTUS_EVENT_KEY_RELEASED);
    }
}

void lotus_process_button(lotus_mouse_button button, ubyte pressed) {
    if (internal_input_state.current.buttons[button] != pressed) {
        internal_input_state.current.buttons[button] = pressed;

        lotus_event event;
        event.data.ubyte2[0] = button;
        lotus_push_event(event, pressed ? LOTUS_EVENT_BUTTON_PRESSED : LOTUS_EVENT_BUTTON_RELEASED);
    }
}
