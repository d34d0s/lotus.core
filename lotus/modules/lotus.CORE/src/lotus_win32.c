#include "../include/platform/lotus_platform.h"

#if defined(LOTUS_PLATFORM_WINDOWS)

#include <Windows.h>
#include <windowsx.h>   // parameter input extraction

typedef struct platform_window_data {
    HWND handle;
    HGLRC gl_context;
    HDC device_context;
    HINSTANCE instance;
} platform_window_data;

static lotus_platform_state internal_platform_state = {0};

lotus_platform_state* platform_init(void) {
    internal_platform_state.platform = LOTUS_WINDOWS_TAG;
    
    internal_platform_state.state = NULL;
    
    internal_platform_state.event_state = lotus_init_event();
    internal_platform_state.input_state = lotus_input_init();

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    internal_platform_state.clock_frequency = 1.0 / (f64)frequency.QuadPart;
    
    return &internal_platform_state;
}

void platform_exit(void) {
    lotus_exit_event();
    lotus_input_exit();
    internal_platform_state.state = NULL;
    return;
}

LRESULT CALLBACK _window_proc(HWND handle, ubyte4 msg, WPARAM w, LPARAM l) {
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

lotus_window platform_create_window(const char* title, int width, int height) {
    lotus_window window;

    window.internal_data = (platform_window_data*)malloc(sizeof(platform_window_data));
    if (!window.internal_data) {
        return (lotus_window){0};
    }
    platform_window_data* data = (platform_window_data*)window.internal_data;

    // Register window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = _window_proc;
    wc.hInstance = data->instance;
    wc.lpszClassName = "Lotus Window Class";

    if (!RegisterClass(&wc)) {
        return (lotus_window){0};
    }

    // Create the window
    HWND hwnd = CreateWindow(
        "Lotus Window Class",
        title,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,  // default x
        CW_USEDEFAULT,  // default y
        width,
        height,
        NULL,
        NULL,
        data->instance,
        NULL);

    if (!hwnd) {
        return (lotus_window){0};
    }

    data->handle = hwnd;
    data->gl_context = NULL;
    data->device_context = GetDC(hwnd);

    // Initialize the generic window structure
    strncpy(window.title, title, sizeof(window.title) - 1);
    window.title[sizeof(window.title) - 1] = '\0';
    window.location[0] = CW_USEDEFAULT;
    window.location[1] = CW_USEDEFAULT;
    window.size[0] = width;
    window.size[1] = height;
    window.aspect_ratio = (float)width / height;
    window.gl_context = NULL;

    return window;
}

void platform_destroy_window(lotus_window* window) {
    if (window && window->internal_data) {
        platform_window_data* data = (platform_window_data*)window->internal_data;
        if (data->handle) {
            DestroyWindow(data->handle);
        }
        free(data);
        window->internal_data = NULL;
    }
}

bool platform_create_gl_context(lotus_window* window) {
    if (!window || !window->internal_data) {
        printf("window_ptr/window->internal_data_ptr invalid!\n");
        return false;
    }

    platform_window_data* data = (platform_window_data*)window->internal_data;

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0, 0,
        0, 0, 0, 0, 0,
        24, 8,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    int format = ChoosePixelFormat(data->device_context, &pfd);
    if (!SetPixelFormat(data->device_context, format, &pfd)) {
        printf("failed to set pixel format\n");
        return false;
    }

    data->gl_context = wglCreateContext(data->device_context);
    if (!data->gl_context) {
        printf("failed to create gl context\n");
        return false;
    }

    if (!wglMakeCurrent(data->device_context, data->gl_context)) {
        printf("failed to bind gl context\n");
        wglDeleteContext(data->gl_context);
        return false;
    }

    window->gl_context = data->gl_context;
    return true;
}

void platform_destroy_gl_context(lotus_window* window) {
    if (!window || !window->internal_data) return;

    platform_window_data* data = (platform_window_data*)window->internal_data;
    if (data->gl_context) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(data->gl_context);
        data->gl_context = NULL;
        window->gl_context = NULL;
    }
}

double platform_get_time() {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / internal_platform_state.clock_frequency;
}

void platform_sleep(f64 seconds) {
    Sleep((DWORD)(seconds * 1000.0));
}

void platform_swap_buffers(lotus_window* window) {
    if (!window || !window->internal_data) return;

    platform_window_data* data = (platform_window_data*)window->internal_data;
    SwapBuffers(data->device_context);
}

ubyte platform_pump(void) {
    MSG message;
    while(PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
    return LOTUS_TRUE;
}

#endif // LOTUS_PLATFORM_WINDOWS == LOTUS_TRUE