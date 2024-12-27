#define LOTUS_APPLICATION
#define LOTUS_DATASTRUCTS
#define LOTUS_ECS
#define LOTUS_MATH
#include "../../include/lotus.h"

lotus_application app;

ubyte4 preframe(void) {
    lotus_platform_pump();
    return LOTUS_TRUE;
}

ubyte4 midframe(void) {
    if (lotus_is_key_down(LOTUS_KEY_F12) || lotus_is_key_down(LOTUS_KEY_ESCAPE)) {
        app.state.running = LOTUS_FALSE;
    }
    return LOTUS_TRUE;
}

ubyte4 postframe(void) {
    lotus_platform_swap_buffers(&app.resource.window);
    lotus_update_input(0);
    return LOTUS_TRUE;
}

int array_test() {
    f32* vertices = lotus_make_array(sizeof(f32), 2);
    lotus_print_array("Vertices", vertices);

    lotus_insert_array(vertices, 0, LOTUS_LITERAL(f32, 666.0));
    lotus_insert_array(vertices, 1, LOTUS_LITERAL(f32, 420.0));
    lotus_insert_array(vertices, 2, LOTUS_LITERAL(f32, 69.0));

    for (int i = 0; i < 3; i++) {
        printf("Vertex: %0.1f\n", vertices[i]);
    }
    
    lotus_print_array("Vertices", vertices);

    lotus_destroy_array(vertices);

    return 0;
}

int main() {
    lotus_init_app(&app, "Lotus Application");
    lotus_make_window(&app, "Lotus Window", LOTUS_VEC2(ubyte4, 100, 100), LOTUS_VEC2(ubyte4, 1280, 720));
    
    lotus_set_preframe(&app, preframe);
    lotus_set_midframe(&app, midframe);
    lotus_set_postframe(&app, postframe);

    lotus_run_application(&app);

    lotus_destroy_window(&app);
    lotus_exit_app(&app);
    return 0;
}
