#pragma once

#include "../defines.h"

typedef enum sogl_mouse_button {
    SOGL_MBUTTON_LEFT,
    SOGL_MBUTTON_RIGHT,
    SOGL_MBUTTON_MIDDLE,
    SOGL_MBUTTON_MAX_BUTTONS
} sogl_mouse_button;

#define SOGL_DEFINE_KEY(name, code) \
    SOGL_KEY_##name = code

typedef enum sogl_keyboard_key {
    SOGL_DEFINE_KEY(BACKSPACE, 0x08),
    SOGL_DEFINE_KEY(ENTER, 0x0D),
    SOGL_DEFINE_KEY(TAB, 0x09),
    SOGL_DEFINE_KEY(SHIFT, 0x10),
    SOGL_DEFINE_KEY(CONTROL, 0x11),
    SOGL_DEFINE_KEY(PAUSE, 0x13),
    SOGL_DEFINE_KEY(CAPITAL, 0x14),
    SOGL_DEFINE_KEY(ESCAPE, 0x1B),
    SOGL_DEFINE_KEY(CONVERT, 0x1C),
    SOGL_DEFINE_KEY(NONCONVERT, 0x1D),
    SOGL_DEFINE_KEY(ACCEPT, 0x1E),
    SOGL_DEFINE_KEY(MODECHANGE, 0x1F),
    SOGL_DEFINE_KEY(SPACE, 0x20),
    SOGL_DEFINE_KEY(PRIOR, 0x21),
    SOGL_DEFINE_KEY(NEXT, 0x22),
    SOGL_DEFINE_KEY(END, 0x23),
    SOGL_DEFINE_KEY(HOME, 0x24),
    SOGL_DEFINE_KEY(LEFT, 0x25),
    SOGL_DEFINE_KEY(UP, 0x26),
    SOGL_DEFINE_KEY(RIGHT, 0x27),
    SOGL_DEFINE_KEY(DOWN, 0x28),
    SOGL_DEFINE_KEY(SELECT, 0x29),
    SOGL_DEFINE_KEY(PRINT, 0x2A),
    SOGL_DEFINE_KEY(EXECUTE, 0x2B),
    SOGL_DEFINE_KEY(SNAPSHOT, 0x2C),
    SOGL_DEFINE_KEY(INSERT, 0x2D),
    SOGL_DEFINE_KEY(DELETE, 0x2E),
    SOGL_DEFINE_KEY(HELP, 0x2F),
    SOGL_DEFINE_KEY(A, 0x41),
    SOGL_DEFINE_KEY(B, 0x42),
    SOGL_DEFINE_KEY(C, 0x43),
    SOGL_DEFINE_KEY(D, 0x44),
    SOGL_DEFINE_KEY(E, 0x45),
    SOGL_DEFINE_KEY(F, 0x46),
    SOGL_DEFINE_KEY(G, 0x47),
    SOGL_DEFINE_KEY(H, 0x48),
    SOGL_DEFINE_KEY(I, 0x49),
    SOGL_DEFINE_KEY(J, 0x4A),
    SOGL_DEFINE_KEY(K, 0x4B),
    SOGL_DEFINE_KEY(L, 0x4C),
    SOGL_DEFINE_KEY(M, 0x4D),
    SOGL_DEFINE_KEY(N, 0x4E),
    SOGL_DEFINE_KEY(O, 0x4F),
    SOGL_DEFINE_KEY(P, 0x50),
    SOGL_DEFINE_KEY(Q, 0x51),
    SOGL_DEFINE_KEY(R, 0x52),
    SOGL_DEFINE_KEY(S, 0x53),
    SOGL_DEFINE_KEY(T, 0x54),
    SOGL_DEFINE_KEY(U, 0x55),
    SOGL_DEFINE_KEY(V, 0x56),
    SOGL_DEFINE_KEY(W, 0x57),
    SOGL_DEFINE_KEY(X, 0x58),
    SOGL_DEFINE_KEY(Y, 0x59),
    SOGL_DEFINE_KEY(Z, 0x5A),
    SOGL_DEFINE_KEY(LWIN, 0x5B),
    SOGL_DEFINE_KEY(RWIN, 0x5C),
    SOGL_DEFINE_KEY(APPS, 0x5D),
    SOGL_DEFINE_KEY(SLEEP, 0x5F),
    SOGL_DEFINE_KEY(NUMPAD0, 0x60),    // wrong on win32????
    SOGL_DEFINE_KEY(NUMPAD1, 0x61),    // wrong on win32????
    SOGL_DEFINE_KEY(NUMPAD2, 0x62),    // wrong on win32????
    SOGL_DEFINE_KEY(NUMPAD3, 0x63),    // wrong on win32????
    SOGL_DEFINE_KEY(NUMPAD4, 0x64),    // wrong on win32????
    SOGL_DEFINE_KEY(NUMPAD5, 0x65),    // wrong on win32????
    SOGL_DEFINE_KEY(NUMPAD6, 0x66),    // wrong on win32????
    SOGL_DEFINE_KEY(NUMPAD7, 0x67),    // wrong on win32????
    SOGL_DEFINE_KEY(NUMPAD8, 0x68),    // wrong on win32????
    SOGL_DEFINE_KEY(NUMPAD9, 0x69),    // wrong on win32????
    SOGL_DEFINE_KEY(MULTIPLY, 0x6A),
    SOGL_DEFINE_KEY(ADD, 0x6B),
    SOGL_DEFINE_KEY(SEPARATOR, 0x6C),
    SOGL_DEFINE_KEY(SUBTRACT, 0x6D),
    SOGL_DEFINE_KEY(DECIMAL, 0x6E),
    SOGL_DEFINE_KEY(DIVIDE, 0x6F),
    SOGL_DEFINE_KEY(F1, 0x70),
    SOGL_DEFINE_KEY(F2, 0x71),
    SOGL_DEFINE_KEY(F3, 0x72),
    SOGL_DEFINE_KEY(F4, 0x73),
    SOGL_DEFINE_KEY(F5, 0x74),
    SOGL_DEFINE_KEY(F6, 0x75),
    SOGL_DEFINE_KEY(F7, 0x76),
    SOGL_DEFINE_KEY(F8, 0x77),
    SOGL_DEFINE_KEY(F9, 0x78),
    SOGL_DEFINE_KEY(F10, 0x79),
    SOGL_DEFINE_KEY(F11, 0x7A),
    SOGL_DEFINE_KEY(F12, 0x7B),
    SOGL_DEFINE_KEY(F13, 0x7C),
    SOGL_DEFINE_KEY(F14, 0x7D),
    SOGL_DEFINE_KEY(F15, 0x7E),
    SOGL_DEFINE_KEY(F16, 0x7F),
    SOGL_DEFINE_KEY(F17, 0x80),
    SOGL_DEFINE_KEY(F18, 0x81),
    SOGL_DEFINE_KEY(F19, 0x82),
    SOGL_DEFINE_KEY(F20, 0x83),
    SOGL_DEFINE_KEY(F21, 0x84),
    SOGL_DEFINE_KEY(F22, 0x85),
    SOGL_DEFINE_KEY(F23, 0x86),
    SOGL_DEFINE_KEY(F24, 0x87),
    SOGL_DEFINE_KEY(NUMLOCK, 0x90),
    SOGL_DEFINE_KEY(SCROLL, 0x91),
    SOGL_DEFINE_KEY(NUMPAD_EQUAL, 0x92),
    SOGL_DEFINE_KEY(LSHIFT, 0xA0),
    SOGL_DEFINE_KEY(RSHIFT, 0xA1),
    SOGL_DEFINE_KEY(LCONTROL, 0xA2),
    SOGL_DEFINE_KEY(RCONTROL, 0xA3),
    SOGL_DEFINE_KEY(LMENU, 0xA4),
    SOGL_DEFINE_KEY(RMENU, 0xA5),
    SOGL_DEFINE_KEY(SEMICOLON, 0xBA),
    SOGL_DEFINE_KEY(PLUS, 0xBB),
    SOGL_DEFINE_KEY(COMMA, 0xBC),
    SOGL_DEFINE_KEY(MINUS, 0xBD),
    SOGL_DEFINE_KEY(PERIOD, 0xBE),
    SOGL_DEFINE_KEY(SLASH, 0xBF),
    SOGL_DEFINE_KEY(GRAVE, 0xC0),
    SOGL_MAX_KEYS
} sogl_keyboard_key;

ubyte sogl_input_init();
void sogl_input_exit();

void sogl_process_mouse_wheel(sbyte z_delta);
void sogl_process_mouse_move(sbyte2 x, sbyte2 y);
void sogl_process_key(sogl_keyboard_key key, ubyte pressed);
void sogl_process_button(sogl_mouse_button button, ubyte pressed);

SOGL_API_ENTRY void sogl_input_update(f64 delta_time);

SOGL_API_ENTRY ubyte sogl_is_key_up(sogl_keyboard_key key);
SOGL_API_ENTRY ubyte sogl_was_key_up(sogl_keyboard_key key);
SOGL_API_ENTRY ubyte sogl_is_key_down(sogl_keyboard_key key);
SOGL_API_ENTRY ubyte sogl_was_key_down(sogl_keyboard_key key);

SOGL_API_ENTRY ubyte sogl_is_button_up(sogl_mouse_button button);
SOGL_API_ENTRY ubyte sogl_was_button_up(sogl_mouse_button button);
SOGL_API_ENTRY ubyte sogl_is_button_down(sogl_mouse_button button);
SOGL_API_ENTRY ubyte sogl_was_button_down(sogl_mouse_button button);

SOGL_API_ENTRY void sogl_get_mouse_position(sbyte4* x, sbyte4* y);
SOGL_API_ENTRY void sogl_get_previous_mouse_position(sbyte4* x, sbyte4* y);
