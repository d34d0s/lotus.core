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

Lotus_Application_API* app_api;
Lotus_Application* app;
ubyte scene_id;

Lotus_Entity e0;
Lotus_Component e0_mesh;
Lotus_Component e0_transform;

ubyte midframe_callback(Lotus_Event data, ubyte2 event_code) {
    if (event_code == LOTUS_APPLICATION_MIDFRAME_EVENT) {
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
        app_api->set_component(scene_id, (Lotus_Component){
            .type = LOTUS_TRANSFORM_COMPONENT,
            .data.transform.model = m_translation,
            .data.transform.location = new_location,
        }, e0); e0_transform = app_api->get_component(scene_id, LOTUS_TRANSFORM_COMPONENT, e0);

        lotus_send_shader_uniform(my_shader, LOTUS_UNIFORM_MAT4, "u_view");

        lotus_draw_submit(
            e0_mesh.data.mesh.vbo,
            e0_mesh.data.mesh.ebo,
            e0_mesh.data.mesh.vao,
            &e0_transform.data.transform.model,
            e0_mesh.data.mesh.n_indices,
            e0_mesh.data.mesh.n_vertices
        ); return LOTUS_TRUE;
    }; return LOTUS_FALSE;
}

int main() {
    app_api = lotus_init_application();
    app = app_api->initialize("My Application", LOTUS_VEC2(ubyte4, 1280, 720));

    sbyte scene_id = app_api->create_scene("My Scene");

    e0 = app_api->make_entity(scene_id);

    app_api->add_component(scene_id, LOTUS_MESH_COMPONENT, e0);
    app_api->add_component(scene_id, LOTUS_TRANSFORM_COMPONENT, e0);

    app_api->set_component(scene_id, 
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

    e0_mesh = app_api->get_component(scene_id, LOTUS_MESH_COMPONENT, e0);
    e0_transform = app_api->get_component(scene_id, LOTUS_TRANSFORM_COMPONENT, e0);

    my_shader = lotus_make_shader(vShader, fShader);
    lotus_set_renderer_shader(&my_shader);
    
    // set model matrix
    lotus_set_shader_uniform(my_shader, "u_model", &e0_transform.data.transform.model);

    // set view matrix
    m_view = lotus_look_at(eye, center, up);
    lotus_set_shader_uniform(my_shader, "u_view", &m_view);
    
    lotus_draw_begin(LOTUS_TRIANGLE_MODE, 133, 161, 172, 255, lotus_perspective(lotus_to_radians(45.0), 1280/720, 0.1, 1000.0));
    // lotus_draw_begin(LOTUS_TRIANGLE_MODE, 133, 161, 172, 255, lotus_ortho(0, 1, 0, 1, 0.1, 10));
    app_api->set_callback(LOTUS_APPLICATION_MIDFRAME_EVENT, midframe_callback);
    app_api->run();
    
    return 0;
}
