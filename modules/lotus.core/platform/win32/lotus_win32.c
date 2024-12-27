#include "../lotus_platform.h"

#if defined(LOTUS_PLATFORM_WINDOWS)

#include <Windows.h>
#include <windowsx.h>   // parameter input extraction

typedef struct _lotus_window_win32 {
    HWND handle;
    HGLRC gl_context;
    HDC device_context;
    HINSTANCE instance;
} _lotus_window_win32;

static lotus_platform_state internal_platform_state = {0};

lotus_platform_state* lotus_platform_init(void) {
    internal_platform_state.platform = LOTUS_WINDOWS_TAG;
    
    internal_platform_state.state = NULL;
    
    internal_platform_state.event_state = lotus_init_event();
    internal_platform_state.input_state = lotus_init_input();

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    internal_platform_state.clock_frequency = 1.0 / (f64)frequency.QuadPart;
    
    return &internal_platform_state;
}

void lotus_platform_exit(void) {
    lotus_exit_event();
    lotus_exit_input();
    internal_platform_state.state = NULL;
    return;
}

LRESULT CALLBACK _lotus_window_process_win32(HWND handle, ubyte4 msg, WPARAM w, LPARAM l) {
    switch(msg) {
        case WM_ERASEBKGND: return 1;
        case WM_QUIT:       // fall through WM_DESTROY
        case WM_CLOSE:      // fall through WM_DESTROY
        case WM_DESTROY:
            lotus_push_event((lotus_event){.data.ubyte[0]=1}, LOTUS_EVENT_APP_QUIT);
            PostQuitMessage(0);
            return 0;
        case WM_SIZE: {
            RECT r;
            GetClientRect(handle, &r);
            ubyte2 w = r.right - r.left;
            ubyte2 h = r.bottom - r.top;
            // TODO: handle resize event with resize callback
            lotus_push_event((lotus_event){.data.ubyte2[0]=w, .data.ubyte2[1]=h}, LOTUS_EVENT_RESIZE);
        } break;
        case WM_KEYUP:          // fall trough WM_SYSKEYUP
        case WM_KEYDOWN:        // fall trough WM_SYSKEYUP
        case WM_SYSKEYDOWN:     // fall trough WM_SYSKEYUP
        case WM_SYSKEYUP: {
            // key pressed/released
            lotus_keyboard_key key = (ubyte2)w;
            ubyte pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
            lotus_process_key(key, pressed);
        } break;
        case WM_MOUSEMOVE: {
            // mouse movement
            sbyte4 x = GET_X_LPARAM(l); 
            sbyte4 y = GET_Y_LPARAM(l);
            lotus_process_mouse_move(x, y);
        } break;
        case WM_MOUSEWHEEL: {
            sbyte4 z = GET_WHEEL_DELTA_WPARAM(w);
            if (z != 0) {
                // clamp input to OS-independent values (-1, 1)
                z = (z < 0) ? -1 : 1;
                lotus_process_mouse_wheel(z);
            }
        } break;
        case WM_LBUTTONDOWN:    // fall trough WM_RBUTTONUP
        case WM_MBUTTONDOWN:    // fall trough WM_RBUTTONUP
        case WM_RBUTTONDOWN:    // fall trough WM_RBUTTONUP
        case WM_LBUTTONUP:      // fall trough WM_RBUTTONUP
        case WM_MBUTTONUP:      // fall trough WM_RBUTTONUP
        case WM_RBUTTONUP: {
            lotus_mouse_button button = LOTUS_MBUTTON_MAX_BUTTONS;
            ubyte pressed = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN;
            switch (msg) {
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                    button = LOTUS_MBUTTON_LEFT;
                    break;
                
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                    button = LOTUS_MBUTTON_MIDDLE;
                    break;
                
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                    button = LOTUS_MBUTTON_RIGHT;
                    break;
            }
            if (button != LOTUS_MBUTTON_MAX_BUTTONS) lotus_process_button(button, pressed);
        } break;
    }
    
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

    window_state->handle = CreateWindowExA(
        windowExStyle, "SoGL Window", title,
        windowStyle, x, y, w, h, 0, 0, window_state->instance, 0
    );

    if (window_state->handle == 0) {
        MessageBoxA(0, "Window Creation Failed", "Error", MB_ICONEXCLAMATION | MB_OK);
        return (lotus_window){0};
    } else {
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

void lotus_platform_destroy_window(lotus_window* window) {
    _lotus_window_win32* window_state = (_lotus_window_win32*)window->state;
    if (window_state) {
        if (window_state->gl_context) {
            wglDeleteContext(window_state->gl_context);
        }
        if (window_state->device_context) {
            ReleaseDC(window_state->handle, window_state->device_context);
        }
        DestroyWindow(window_state->handle);
        free(window_state);
    }
    window->state = NULL;
}

f64 lotus_platform_get_time(void) {
    LARGE_INTEGER nowTime;
    QueryPerformanceCounter(&nowTime);
    return (f64)nowTime.QuadPart * internal_platform_state.clock_frequency;
}

void lotus_platform_sleep(ubyte8 ms) { Sleep(ms); }

ubyte lotus_platform_pump(void) {
    MSG message;
    while(PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
    return LOTUS_TRUE;
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

void lotus_platform_swap_buffers(lotus_window* window) {
    _lotus_window_win32* window_state = (_lotus_window_win32*)window->state;
    SwapBuffers(GetDC(window_state->handle));
}

#endif // LOTUS_PLATFORM_WINDOWS == LOTUS_TRUE