#define LOTUS_APPLICATION
#define LOTUS_ECS
#include "../../include/lotus.h"

char vShader[] = {
    "#version 460 core\n"
    "layout(location = 0) in vec3 a_Location;\n"
    "layout(location = 1) in vec3 a_Color;\n"
    "uniform mat4 u_model;\n"
    "uniform mat4 u_view;\n"
    "uniform mat4 u_projection;\n"
    "out vec3 vertex_color;\n"
    "void main() {\n"
    "   gl_Position = u_projection * u_view * u_model * vec4(a_Location, 1.0f);\n"
    "   vertex_color = a_Color;\n"
    "}\0"
};

char fShader[] = {
    "#version 460 core\n"
    "in vec3 vertex_color;\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "   frag_color = vec4(vertex_color, 1.0f);\n"
    "}\0"
};
lotus_shader my_shader;

// camera-data handled manually until implementation :)
lotus_vec3 eye = {0.0f, 0.0f, 1.0f};
lotus_vec3 center = {0.0f, 0.0f, 0.0f};
lotus_vec3 up = {0.0f, 1.0f, 0.0f};
lotus_mat4 m_model;
lotus_mat4 m_view;

lotus_application app;
lotus_entity e0;
lotus_component e0_mesh;
lotus_entity_manager em;
lotus_component_manager cm;

ubyte my_callback(lotus_event data, ubyte2 event_code) {
    if (event_code == LOTUS_EVENT_KEY_PRESSED) {
        printf("Pressed A Key!\n");
    }
    return LOTUS_TRUE;
}

ubyte4 preframe(void) {
    lotus_platform_pump();
    return LOTUS_TRUE;
}

ubyte drawn;
ubyte4 midframe(void) {
    if (lotus_key_is_down(LOTUS_KEY_F12) || lotus_key_is_down(LOTUS_KEY_ESCAPE)) {
        app.state.running = LOTUS_FALSE;
    }

    lotus_set_uniform(my_shader, LOTUS_UNIFORM_MAT4, "u_model", &m_model);
    lotus_set_uniform(my_shader, LOTUS_UNIFORM_MAT4, "u_view", &m_view);
    
    lotus_renderer_submit(
        e0_mesh.data.mesh_data.vbo,
        e0_mesh.data.mesh_data.ebo,
        e0_mesh.data.mesh_data.vao,
        &m_model,
        e0_mesh.data.mesh_data.n_indices,
        e0_mesh.data.mesh_data.n_vertices
    );

    return LOTUS_TRUE;
}

ubyte4 postframe(void) {
    lotus_renderer_flush();
    lotus_platform_swap_buffers(&app.resource.window);
    lotus_input_update(0);
    return LOTUS_TRUE;
}

void ecs_test() {
    lotus_init_ecs(&em, &cm);

    e0 = lotus_make_entity(&em);
    printf("My First Entity: %d\n", e0);

    lotus_add_component(&cm, LOTUS_MESH_COMPONENT, e0);

    lotus_set_component(&cm, 
        (lotus_component){ 
            .type = LOTUS_MESH_COMPONENT, 
            .data.mesh_data.attrs = LOTUS_LOCATION_ATTR | LOTUS_COLOR_ATTR,
            .data.mesh_data.n_indices = 0,
            .data.mesh_data.indices = NULL,
            .data.mesh_data.n_vertices = 3,
            .data.mesh_data.vertices = (f32[]){
                -0.5, -0.5, 0.5, 1.0, 0.0, 0.0,
                 0.5, -0.5, 0.5, 0.0, 1.0, 0.0,
                 0.0,  0.5, 0.5, 0.0, 0.0, 1.0
            },
        }, e0
    );

    e0_mesh = lotus_get_component(&cm, LOTUS_MESH_COMPONENT, e0);
}

int main() {
    lotus_app_init(&app, "Lotus Application");
    lotus_app_make_window(&app, "Lotus Window", LOTUS_VEC2(ubyte4, 100, 100), LOTUS_VEC2(ubyte4, 1280, 720));
    
    lotus_renderer_init();
    lotus_renderer_begin(LOTUS_TRIANGLE_MODE, 133, 161, 172, 255, lotus_perspective(lotus_to_radians(45.0), 1280/720, 0.1, 1000.0));
    // lotus_renderer_begin(LOTUS_TRIANGLE_MODE, 133, 161, 172, 255, lotus_ortho(0, 1, 0, 1, 0.1, 10));

    ecs_test();

    lotus_app_set_preframe(&app, preframe);
    lotus_app_set_midframe(&app, midframe);
    lotus_app_set_postframe(&app, postframe);

    lotus_register_event(LOTUS_EVENT_KEY_PRESSED, my_callback);

    my_shader = lotus_make_shader(vShader, fShader);

    m_model = lotus_identity();
    m_view = lotus_look_at(eye, center, up);

    lotus_renderer_set_shader(&my_shader);

    lotus_app_run(&app);

    lotus_rem_component(&cm, LOTUS_MESH_COMPONENT, e0);
    lotus_exit_ecs(&em, &cm);
    
    lotus_renderer_destroy();
    
    lotus_app_destroy_window(&app);
    lotus_app_exit(&app);
    return 0;
}
