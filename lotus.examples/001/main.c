#define LOTUS_APPLICATION
#define LOTUS_MATH
#define LOTUS_ECS
#include "../../include/lotus.h"

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
    lotus_init_app(&app, "Lotus Application");
    lotus_make_window(&app, "Lotus Window", LOTUS_VEC2(ubyte4, 100, 100), LOTUS_VEC2(ubyte4, 1280, 720));
    
    lotus_set_midframe(&app, midframe);
    lotus_set_postframe(&app, postframe);
    
    lotus_entity_manager e_manager;
    lotus_component_manager c_manager;
    lotus_init_ecs(&e_manager, &c_manager);
    
    lotus_entity e0 = lotus_make_entity(&e_manager);

    lotus_add_component(&c_manager, LOTUS_MESH_COMPONENT, e0);
    printf("E0 Has Mesh: %d\n", (int)lotus_has_component(&c_manager, LOTUS_MESH_COMPONENT, e0));
    
    lotus_set_component(
        &c_manager,
        (lotus_component){
            .type = LOTUS_MESH_COMPONENT,
            .data.mesh_data.vbo = 0,
            .data.mesh_data.ebo = 0,
            .data.mesh_data.vao = 0,
            .data.mesh_data.vertices = 0,
            .data.mesh_data.raw_vertices = 0,
            },
        e0
    );
    
    lotus_component e0_mesh = lotus_get_component(&c_manager, LOTUS_MESH_COMPONENT, e0);
    if (e0_mesh.type == LOTUS_MESH_COMPONENT) {
        printf("Retrieved Mesh Component For E0\n");
    } else {
        printf("Unable To Retrieve Mesh Component For E0\n");
    }

    lotus_rem_component(&c_manager, LOTUS_MESH_COMPONENT, e0);
    printf("E0 Has Mesh: %d\n", (int)lotus_has_component(&c_manager, LOTUS_MESH_COMPONENT, e0));
    
    // lotus_run_application(&app);

    lotus_exit_app(&app);
    
    return 0;
}
