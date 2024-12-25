#define SOGL_MATH
#define SOGL_APPLICATION
#include "../../include/sogl.h"

ubyte4 midframe(void) {
    printf("SoGL midframe callback (main-loop)!\n");
    return SOGL_TRUE;
}

ubyte4 postframe(void) {
    printf("SoGL postframe callback!\n");
    return SOGL_TRUE;
}

int main() {
    sogl_application app;
    sogl_init_app(&app, "SoGameLibrary");
    sogl_make_window(&app, "My Window", SOGL_VEC2(ubyte4, 100, 100), SOGL_VEC2(ubyte4, 1280, 720));
    
    sogl_set_midframe(&app, midframe);
    sogl_set_postframe(&app, postframe);
    
    sogl_run_application(&app);
    
    sogl_exit_app(&app);
    
    return 0;
}
