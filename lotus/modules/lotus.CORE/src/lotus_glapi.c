#include "../include/graphics/lotus_glapi.h"

#include "../include/platform/lotus_logger.h"
#include "../include/platform/lotus_platform.h"

// BUFFER FUNCTIONS

void (*lgl_gen_buffers)(sbyte4 count, ubyte4 *buffer);
void (*lgl_bind_buffer)(ubyte4 target, ubyte4 buffer);
void (*lgl_buffer_data)(ubyte4 target, sbyte4 size, const void *data, ubyte4 usage);
void (*lgl_delete_buffers)(sbyte4 count, const ubyte4 *buffer);
void (*lgl_map_buffer)(ubyte4 target, ubyte4 access);
void (*lgl_unmap_buffer)(ubyte4 target);
void (*lgl_buffer_sub_data)(ubyte4 target, ubyte4* offset, sbyte4 size, const void *data);
void (*lgl_get_buffer_parameteriv)(ubyte4 target, ubyte4 pname, ubyte4 *params);

// VERTEX ATTRIBUTE FUNCTIONS

void (*lgl_enable_vertex_attrib_array)(ubyte4 index);
void (*lgl_disable_vertex_attrib_array)(ubyte4 index);
void (*lgl_vertex_attrib_pointer)(ubyte4 index, ubyte4 size, ubyte4 type, ubyte normalized, sbyte4 stride, const void *pointer);
void (*lgl_bind_vertex_array)(ubyte4 array);
void (*lgl_gen_vertex_arrays)(sbyte4 count, ubyte4 *arrays);
void (*lgl_delete_vertex_arrays)(sbyte4 count, const ubyte4 *arrays);

// SHADER FUNCTIONS

ubyte4 (*lgl_create_program)(void);
void (*lgl_use_program)(ubyte4 program);
void (*lgl_link_program)(ubyte4 program);
void (*lgl_delete_program)(ubyte4 program);

ubyte4 (*lgl_create_shader)(ubyte4 type);
void (*lgl_compile_shader)(ubyte4 shader);
void (*lgl_attach_shader)(ubyte4 program, ubyte4 shader);
void (*lgl_detach_shader)(ubyte4 program, ubyte4 shader);
void (*lgl_delete_shader)(ubyte4 shader);
void (*lgl_shader_source)(ubyte4 shader, sbyte4 count, const char **strings, const int *lengths);
void (*lgl_get_shaderiv)(ubyte4 shader, ubyte4 pname, ubyte4 *params);
void (*lgl_get_shader_info_log)(ubyte4 shader, sbyte4 maxLength, sbyte4 *length, char *infoLog);
void (*lgl_get_programiv)(ubyte4 program, ubyte4 pname, ubyte4 *params);
void (*lgl_get_program_info_log)(ubyte4 program, sbyte4 maxLength, sbyte4 *length, char *infoLog);
ubyte4 (*lgl_get_uniform_location)(ubyte4 program, const char* name);
void (*lgl_uniform2fv)(ubyte4 location, ubyte4 count, const f32* value);
void (*lgl_uniform3fv)(ubyte4 location, ubyte4 count, const f32* value);
void (*lgl_uniform4fv)(ubyte4 location, ubyte4 count, const f32* value);
void (*lgl_uniform_matrix4fv)(ubyte4 location, ubyte4 count, ubyte4 transpose, const f32* value);

// TEXTURE FUNCTIONS

void (*lgl_gen_textures)(sbyte4 count, ubyte4 *textures);
void (*lgl_bind_texture)(ubyte4 target, ubyte4 texture);
void (*lgl_tex_parameteri)(ubyte4 target, ubyte4 pname, ubyte4 param);
void (*lgl_tex_image2d)(ubyte4 target, ubyte4 level, ubyte4 internalFormat, sbyte4 width, sbyte4 height, ubyte4 border, ubyte4 format, ubyte4 type, const void *data);
void (*lgl_active_texture)(ubyte4 texture);
void (*lgl_delete_textures)(sbyte4 count, const ubyte4 *textures);
void (*lgl_generate_mipmap)(ubyte4 target);
void (*lgl_generate_texture_mipmap)(ubyte4 texture);

// FRAMEBUFFER FUNCTIONS

void (*lgl_gen_framebuffers)(sbyte4 count, ubyte4 *framebuffers);
void (*lgl_bind_framebuffer)(ubyte4 target, ubyte4 framebuffer);
void (*lgl_framebuffer_texture2d)(ubyte4 target, ubyte4 attachment, ubyte4 textarget, ubyte4 texture, ubyte4 level);
void (*lgl_delete_framebuffers)(sbyte4 count, const ubyte4 *framebuffers);
ubyte4 (*lgl_check_framebuffer_status)(ubyte4 target);

// DRAWING FUNCTIONS

void (*lgl_clear)(sbyte4 mask);
void (*lgl_draw_arrays)(ubyte4 mode, ubyte4 first, sbyte4 count);
void (*lgl_clear_color)(f32 r, f32 g, f32 b, f32 a);
void (*lgl_draw_elements)(ubyte4 mode, sbyte4 count, ubyte4 type, const void *indices);
void (*lgl_draw_elements_base_vertex)(ubyte4 mode, sbyte4 count, ubyte4 type, const void *indices, ubyte4 baseVertex);

// STATE MANAGEMENT FUNCTIONS

void (*lgl_enable)(ubyte4 cap);
void (*lgl_disable)(ubyte4 cap);
void (*lgl_blend_func)(ubyte4 sfactor, ubyte4 dfactor);
void (*lgl_cull_face)(ubyte4 mode);
void (*lgl_depth_func)(ubyte4 func);
void (*lgl_viewport)(ubyte4 x, ubyte4 y, sbyte4 width, sbyte4 height);
void (*lgl_polygon_mode)(ubyte4 face, ubyte4 mode);

// GL UTILITY FUNCTIONS

ubyte4 (*lgl_get_error)(void);
const ubyte *(*lgl_get_string)(const ubyte4 name);

// OpenGL function pointer loading mechanism
#if defined(LOTUS_PLATFORM_WINDOWS)
#include <Windows.h>
#include <windowsx.h>
    void* lotus_get_fptr(const char* name) {
        void* proc = (void*)wglGetProcAddress(name);
        if (proc == NULL || (proc == (void*)0x1) || (proc == (void*)0x2) || (proc == (void*)0x3) || (proc == (void*)-1)) {
            proc = (void*)GetProcAddress(LoadLibrary("opengl32.dll"), name);
        }
        return proc;
    }
#elif defined(LOTUS_PLATFORM_MACOS) || defined(LOTUS_PLATFORM_LINUX)
    void* lotus_get_fptr(const char* name) {
        // OpenGL on macOS and Linux is loaded from the OpenGL library using dlopen/dlsym
        void* libGL = dlopen("libGL.dylib", 0x1);  // macOS uses libGL.dylib, Linux uses libGL.so
        if (!libGL) {
            fprintf(stderr, "Failed to load OpenGL library\n");
            return NULL;
        }
        void* proc = dlsym(libGL, name);
        if (!proc) {
            fprintf(stderr, "Failed to load function: %s\n", name);
        }
        return proc;
    }
#else
    #error "Unsupported platform for OpenGL function loading!"
#endif

ubyte lotus_load_fptrs(void) {
    lotus_set_log_level(LOTUS_LOG_ERROR);
    ubyte result = LOTUS_TRUE;

    struct {
        void **function;
        const char *name;
    } functions[] = {
        // BUFFER FUNCTIONS
        {(void**)&lgl_gen_buffers, "glGenBuffers"},
        {(void**)&lgl_bind_buffer, "glBindBuffer"},
        {(void**)&lgl_buffer_data, "glBufferData"},
        {(void**)&lgl_map_buffer, "glMapBuffer"},
        {(void**)&lgl_unmap_buffer, "glUnmapBuffer"},
        {(void**)&lgl_buffer_sub_data, "glBufferSubData"},
        {(void**)&lgl_get_buffer_parameteriv, "glGetBufferParameteriv"},
        {(void**)&lgl_delete_buffers, "glDeleteBuffers"},

        // VERTEX ATTRIBUTE FUNCTIONS
        {(void**)&lgl_enable_vertex_attrib_array, "glEnableVertexAttribArray"},
        {(void**)&lgl_disable_vertex_attrib_array, "glDisableVertexAttribArray"},
        {(void**)&lgl_vertex_attrib_pointer, "glVertexAttribPointer"},
        {(void**)&lgl_bind_vertex_array, "glBindVertexArray"},
        {(void**)&lgl_gen_vertex_arrays, "glGenVertexArrays"},
        {(void**)&lgl_delete_vertex_arrays, "glDeleteVertexArrays"},

        // SHADER FUNCTIONS
        {(void**)&lgl_create_program, "glCreateProgram"},
        {(void**)&lgl_use_program, "glUseProgram"},
        {(void**)&lgl_link_program, "glLinkProgram"},
        {(void**)&lgl_delete_program, "glDeleteProgram"},
        {(void**)&lgl_create_shader, "glCreateShader"},
        {(void**)&lgl_compile_shader, "glCompileShader"},
        {(void**)&lgl_attach_shader, "glAttachShader"},
        {(void**)&lgl_detach_shader, "glDetachShader"},
        {(void**)&lgl_delete_shader, "glDeleteShader"},
        {(void**)&lgl_shader_source, "glShaderSource"},
        {(void**)&lgl_get_shaderiv, "glGetShaderiv"},
        {(void**)&lgl_get_shader_info_log, "glGetShaderInfoLog"},
        {(void**)&lgl_get_programiv, "glGetProgramiv"},
        {(void**)&lgl_get_program_info_log, "glGetProgramInfoLog"},
        {(void**)&lgl_get_uniform_location, "glGetUniformLocation"},
        {(void**)&lgl_uniform2fv, "glUniform2fv"},
        {(void**)&lgl_uniform3fv, "glUniform3fv"},
        {(void**)&lgl_uniform4fv, "glUniform4fv"},
        {(void**)&lgl_uniform_matrix4fv, "glUniformMatrix4fv"},

        // TEXTURE FUNCTIONS
        {(void**)&lgl_gen_textures, "glGenTextures"},
        {(void**)&lgl_bind_texture, "glBindTexture"},
        {(void**)&lgl_tex_parameteri, "glTexParameteri"},
        {(void**)&lgl_tex_image2d, "glTexImage2D"},
        {(void**)&lgl_active_texture, "glActiveTexture"},
        {(void**)&lgl_delete_textures, "glDeleteTextures"},
        {(void**)&lgl_generate_mipmap, "glGenerateMipmap"},
        {(void**)&lgl_generate_texture_mipmap, "glGenerateTextureMipmap"},

        // FRAMEBUFFER FUNCTIONS
        {(void**)&lgl_gen_framebuffers, "glGenFramebuffers"},
        {(void**)&lgl_bind_framebuffer, "glBindFramebuffer"},
        {(void**)&lgl_framebuffer_texture2d, "glFramebufferTexture2D"},
        {(void**)&lgl_delete_framebuffers, "glDeleteFramebuffers"},
        {(void**)&lgl_check_framebuffer_status, "glCheckFramebufferStatus"},

        // DRAWING FUNCTIONS
        {(void**)&lgl_clear, "glClear"},
        {(void**)&lgl_clear_color, "glClearColor"},
        {(void**)&lgl_draw_arrays, "glDrawArrays"},
        {(void**)&lgl_draw_elements, "glDrawElements"},
        {(void**)&lgl_draw_elements_base_vertex, "glDrawElementsBaseVertex"},

        // STATE MANAGEMENT FUNCTIONS
        {(void**)&lgl_enable, "glEnable"},
        {(void**)&lgl_disable, "glDisable"},
        {(void**)&lgl_blend_func, "glBlendFunc"},
        {(void**)&lgl_cull_face, "glCullFace"},
        {(void**)&lgl_depth_func, "glDepthFunc"},
        {(void**)&lgl_viewport, "glViewport"},
        {(void**)&lgl_polygon_mode, "glPolygonMode"},

        // UTILITY FUNCTIONS
        {(void**)&lgl_get_error, "glGetError"},
        {(void**)&lgl_get_string, "glGetString"}
    };

    // Load all functions
    for (size_t i = 0; i < sizeof(functions) / sizeof(functions[0]); ++i) {
        *functions[i].function = lotus_get_fptr(functions[i].name);
        if (!*functions[i].function) {
            lotus_log_error("Failed to load GL function: %s\n", functions[i].name);
            result = LOTUS_FALSE;
        }
    }

    return result;
}
