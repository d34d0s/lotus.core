#define LOTUS_APPLICATION
#include "../../lotus/include/lotus.h"

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
Lotus_Shader my_shader;

// camera-data handled manually until implentation :)
Lotus_Mat4 m_view;
Lotus_Vec3 up = {0.0f, 1.0f, 0.0f};
Lotus_Vec3 eye = {0.0f, 0.0f, 1.0f};
Lotus_Vec3 center = {0.0f, 0.0f, 0.0f};

Lotus_Application app;
Lotus_Scene* scene;

Lotus_Entity e0;
Lotus_Component e0_mesh;
Lotus_Component e0_transform;

ubyte4 midframe(void) {
    f32 speed = 0.01;
    Lotus_Vec3 new_location = lotus_new_vec3(
        e0_transform.data.transform.location.x,
        e0_transform.data.transform.location.y,
        e0_transform.data.transform.location.z
    );

    if (lotus_key_is_down(LOTUS_KEY_A)) new_location.x -= speed;
    if (lotus_key_is_down(LOTUS_KEY_D)) new_location.x += speed;
    
    if (lotus_key_is_down(LOTUS_KEY_W)) new_location.y += speed;
    if (lotus_key_is_down(LOTUS_KEY_S)) new_location.y -= speed;

    // apply transforms before sending data to GPU
    Lotus_Mat4 m_translation = lotus_mul_mat4(lotus_identity(), lotus_trans_mat4(
        new_location.x,
        new_location.y,
        new_location.z
    ));

    // update component data internally and then externally
    lotus_set_component(&scene->component_manager, (Lotus_Component){
        .type = LOTUS_TRANSFORM_COMPONENT,
        .data.transform.model = m_translation,
        .data.transform.location = new_location,
    }, e0); e0_transform = lotus_get_component(&scene->component_manager, LOTUS_TRANSFORM_COMPONENT, e0);

    lotus_send_shader_uniform(my_shader, LOTUS_UNIFORM_MAT4, "u_view");

    lotus_draw_submit(
        e0_mesh.data.mesh.vbo,
        e0_mesh.data.mesh.ebo,
        e0_mesh.data.mesh.vao,
        &e0_transform.data.transform.model,
        e0_mesh.data.mesh.n_indices,
        e0_mesh.data.mesh.n_vertices
    );

    return LOTUS_TRUE;
}

void ecs_test() {
    e0 = lotus_make_entity(&scene->entity_namager);

    lotus_add_component(&scene->component_manager, LOTUS_MESH_COMPONENT, e0);
    lotus_add_component(&scene->component_manager, LOTUS_TRANSFORM_COMPONENT, e0);

    lotus_set_component(&scene->component_manager, 
        (Lotus_Component){ 
            .type = LOTUS_MESH_COMPONENT, 
            .data.mesh.attrs = LOTUS_LOCATION_ATTR | LOTUS_COLOR_ATTR,
            .data.mesh.n_vertices = 3,
            .data.mesh.vertices = (f32[]){
                -0.5, -0.5, 0.5, 1.0, 0.0, 0.0,
                 0.5, -0.5, 0.5, 0.0, 1.0, 0.0,
                 0.0,  0.5, 0.5, 0.0, 0.0, 1.0
            },
        }, e0
    );

    e0_mesh = lotus_get_component(&scene->component_manager, LOTUS_MESH_COMPONENT, e0);
    e0_transform = lotus_get_component(&scene->component_manager, LOTUS_TRANSFORM_COMPONENT, e0);
}

int main() {
    lotus_init_application(&app, "My Application", LOTUS_VEC2(ubyte4, 1280, 720));

    sbyte scene_id = lotus_create_application_scene(&app, "My Scene");
    printf("scene id created: %d\n", scene_id);

    if (scene_id >= 0) {
        scene = lotus_get_application_scene(&app, scene_id);
    }

    lotus_draw_begin(LOTUS_TRIANGLE_MODE, 133, 161, 172, 255, lotus_perspective(lotus_to_radians(45.0), 1280/720, 0.1, 1000.0));
    // lotus_draw_begin(LOTUS_TRIANGLE_MODE, 133, 161, 172, 255, lotus_ortho(0, 1, 0, 1, 0.1, 10));

    ecs_test();

    lotus_set_application_midframe(&app, midframe);

    my_shader = lotus_make_shader(vShader, fShader);
    lotus_set_renderer_shader(&my_shader);
    
    // set model matrix
    lotus_set_shader_uniform(my_shader, "u_model", &e0_transform.data.transform.model);

    // set view matrix
    m_view = lotus_look_at(eye, center, up);
    lotus_set_shader_uniform(my_shader, "u_view", &m_view);
    
    lotus_run_application(&app);

    lotus_rem_component(&scene->component_manager, LOTUS_MESH_COMPONENT, e0);
    
    return 0;
}
