#define LOTUS_MATH
#define LOTUS_APPLICATION
#include "../../../include/lotus.h"

ubyte4 midframe(void) {
    printf("Lotus midframe callback (main-loop)!\n");
    return LOTUS_TRUE;
}

ubyte4 postframe(void) {
    printf("Lotus postframe callback!\n");
    return LOTUS_TRUE;
}

int main() {
    lotus_application app;
    lotus_init_app(&app, "SoGameLibrary");
    lotus_make_window(&app, "My Window", LOTUS_VEC2(ubyte4, 100, 100), LOTUS_VEC2(ubyte4, 1280, 720));
    
    lotus_set_midframe(&app, midframe);
    lotus_set_postframe(&app, postframe);
    
    lotus_run_application(&app);
    
    lotus_exit_app(&app);
    
    return 0;
}
