#include "../lotus_platform.h"

#if LOTUS_PLATFORM_WINDOWS == LOTUS_TRUE

#include <Windows.h>
#include <windowsx.h>   // parameter input extraction

typedef struct _lotus_window_win32 {
    HWND handle;
    HGLRC gl_context;
    HDC device_context;
    HINSTANCE instance;
} _lotus_window_win32;

ubyte lotus_platform_init(lotus_platform_state* state) { return LOTUS_TRUE; }

void lotus_platform_exit(lotus_platform_state* state) { return; }

// on windows, the window processes system events as messages that need to be read from w/l params and handled accordingly
// TODO: modify the event system to allow platform agnostic "hooks" or simply have each platform setup listeners/callbacks for events via a generic API
LRESULT CALLBACK _lotus_window_process_win32(HWND handle, ubyte4 msg, WPARAM w, LPARAM l) {
    return DefWindowProcA(handle, msg, w, l);
}

lotus_window lotus_platform_make_window(char* title, sbyte4 x, sbyte4 y, sbyte4 w, sbyte4 h) {
    lotus_window window;
    window.state = (_lotus_window_win32*)malloc(sizeof(_lotus_window_win32));
    if (!window.state) {
        MessageBoxA(0, "Failed to allocate window state", "Error", MB_ICONEXCLAMATION | MB_OK);
        return (lotus_window){0};
    }

    _lotus_window_win32* window_state = (_lotus_window_win32*)window.state;
    window_state->instance = GetModuleHandleA(0);  // get handle to current application

    // populate and register window
    HICON icon = LoadIcon(window_state->instance, IDI_APPLICATION);
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = _lotus_window_process_win32;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = window_state->instance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "SoGL Window";

    if(!RegisterClassA(&wc)) {
        MessageBoxA(0, "Window Registration Failed", "Error", MB_ICONEXCLAMATION | MB_OK);
        return (lotus_window){0};
    }

    ubyte4 windowStyle = WS_TILED | WS_SYSMENU | WS_CAPTION;
    ubyte4 windowExStyle = WS_EX_APPWINDOW;

    windowStyle |= WS_MAXIMIZEBOX;
    windowStyle |= WS_MINIMIZEBOX;
    windowStyle |= WS_THICKFRAME;

    RECT windowBorder = {0, 0, 0, 0};
    AdjustWindowRectEx(&windowBorder, windowStyle, 0, windowExStyle);

    x += windowBorder.left;
    y += windowBorder.top;

    w += windowBorder.right - windowBorder.left;
    h += windowBorder.bottom - windowBorder.top;

    HWND handle = CreateWindowExA(
        windowExStyle, "SoGL Window", title,
        windowStyle, x, y, w, h, 0, 0, window_state->instance, 0
    );

    if (handle == 0) {
        MessageBoxA(0, "Window Creation Failed", "Error", MB_ICONEXCLAMATION | MB_OK);
        return (lotus_window){0};
    } else { 
        window_state->handle = handle;
        sprintf(window.title, "%s", title);
        window.size[0] = w;
        window.size[1] = h;
        window.location[0] = x;
        window.location[1] = y;
    }

    // get current device context and define a pixel format
    window_state->device_context = GetDC(window_state->handle);
    PIXELFORMATDESCRIPTOR pfd = { 
        sizeof(PIXELFORMATDESCRIPTOR),  // Size of this structure
        1,                              // Version
        PFD_DRAW_TO_WINDOW |            // Support window rendering
        PFD_SUPPORT_OPENGL |            // Support OpenGL rendering
        PFD_DOUBLEBUFFER,               // Support double buffering
        PFD_TYPE_RGBA,                  // RGBA pixel format
        32,                             // 32-bit color depth
        0, 0, 0, 0, 0, 0,               // Color bits ignored
        0,                              // No alpha buffer
        0,                              // Shift bit ignored
        0,                              // No accumulation buffer
        0, 0, 0, 0,                     // Accumulation bits ignored
        24,                             // 24-bit z-buffer
        8,                              // 8-bit stencil buffer
        0,                              // No auxiliary buffer
        PFD_MAIN_PLANE,                 // Main drawing layer
        0,                              // Reserved
        0, 0, 0                         // Layer masks ignored
    };
    int pixelFormat = ChoosePixelFormat(window_state->device_context, &pfd);
    if (!SetPixelFormat(window_state->device_context, pixelFormat, &pfd)) {
        MessageBoxA(0, "Failed to set pixel format.", "Error", MB_ICONEXCLAMATION | MB_OK);
        return (lotus_window){0};
    }

    // show the window
    ShowWindow(window_state->handle, SW_SHOW);

    return window;
}

ubyte lotus_platform_make_glcontext(lotus_window* window) {
    _lotus_window_win32* window_state = (_lotus_window_win32*)window->state;

    window_state->gl_context = wglCreateContext(window_state->device_context);
    if (!wglMakeCurrent(window_state->device_context, window_state->gl_context)) {
        MessageBoxA(0, "Failed to activate OpenGL context.", "Error", MB_ICONEXCLAMATION | MB_OK);
        return LOTUS_FALSE;
    }

    return LOTUS_TRUE;
}

#endif // LOTUS_PLATFORM_WINDOWS == LOTUS_TRUE