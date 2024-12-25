#pragma once

#if defined(LOTUS_PLATFORM_WINDOWS)
    #include <GL/gl.h>
#elif defined(LOTUS_PLATFORM_MACOS)
    #include <OpenGL/OpenGL.h>
    #include <OpenGL/gl.h>
    #include <dlfcn.h>
#elif defined(LOTUS_PLATFORM_LINUX)
    #include <GL/gl.h>
    #include <dlfcn.h>
#endif

#include <stddef.h>
#include <stdbool.h>

#include "../types.h"
#include "../defines.h"

// GL equivalent defines
#define LOTUS_STATIC_DRAW                0x88E4

#define LOTUS_ARRAY_BUFFER               0x8892
#define LOTUS_ELEMENT_ARRAY_BUFFER       0x8893

#define LOTUS_VERTEX_SHADER              0x8B31
#define LOTUS_FRAGMENT_SHADER            0x8B30

#define LOTUS_LINK_STATUS                0x8B82
#define LOTUS_COMPILE_STATUS             0x8B81
#define LOTUS_ACTIVE_ATTRIBUTES          0x8B89

#define LOTUS_LINE                       0x1B01
#define LOTUS_FRONT_AND_BACK             0x0408

#define LOTUS_SHADING_LANGUAGE_VERSION   0x8B8C

#define LOTUS_TEXTURE0                   0x84C0
#define LOTUS_TEXTURE1                   0x84C1
#define LOTUS_TEXTURE2                   0x84C2
#define LOTUS_TEXTURE3                   0x84C3
#define LOTUS_ACTIVE_TEXTURE             0x84E0
#define LOTUS_MAX_TEXTURE_UNITS          0x84E2

#ifdef LOTUS_IMPLEMENTATION
#define LOTUS_FUNC(type, name) type name;
#else
#define LOTUS_FUNC(type, name) extern type name;
#endif

ubyte lotus_load_fptrs(void);
void *lotus_get_fptr(const char *name);

// BUFFER FUNCTIONS

extern void (*lotus_gen_buffers)(sbyte4 count, ubyte4 *buffer);
extern void (*lotus_bind_buffer)(ubyte4 target, ubyte4 buffer);
extern void (*lotus_buffer_data)(ubyte4 target, sbyte4 size, const void *data, ubyte4 usage);

extern void (*lotus_delete_buffers)(sbyte4 count, const ubyte4 *buffer);
extern void (*lotus_map_buffer)(ubyte4 target, ubyte4 access);
extern void (*lotus_unmap_buffer)(ubyte4 target);
extern void (*lotus_buffer_sub_data)(ubyte4 target, ubyte4* offset, sbyte4 size, const void *data);
extern void (*lotus_get_buffer_parameteriv)(ubyte4 target, ubyte4 pname, ubyte4 *params);

// VERTEX ATTRIBUTE FUNCTIONS

extern void (*lotus_enable_vertex_attrib_array)(ubyte4 index);
extern void (*lotus_disable_vertex_attrib_array)(ubyte4 index);
extern void (*lotus_vertex_attrib_pointer)(ubyte4 index, ubyte4 size, ubyte4 type, ubyte normalized, sbyte4 stride, const void *pointer);
extern void (*lotus_bind_vertex_array)(ubyte4 array);
extern void (*lotus_gen_vertex_arrays)(sbyte4 count, ubyte4 *arrays);
extern void (*lotus_delete_vertex_arrays)(sbyte4 count, const ubyte4 *arrays);

// SHADER FUNCTIONS

extern ubyte4 (*lotus_create_program)(void);
extern void (*lotus_use_program)(ubyte4 program);
extern void (*lotus_link_program)(ubyte4 program);
extern void (*lotus_delete_program)(ubyte4 program);

extern ubyte4 (*lotus_create_shader)(ubyte4 type);
extern void (*lotus_compile_shader)(ubyte4 shader);
extern void (*lotus_attach_shader)(ubyte4 program, ubyte4 shader);
extern void (*lotus_detach_shader)(ubyte4 program, ubyte4 shader);
extern void (*lotus_delete_shader)(ubyte4 shader);
extern void (*lotus_shader_source)(ubyte4 shader, sbyte4 count, const char **strings, const int *lengths);
extern void (*lotus_get_shaderiv)(ubyte4 shader, ubyte4 pname, ubyte4 *params);
extern void (*lotus_get_shader_info_log)(ubyte4 shader, sbyte4 maxLength, sbyte4 *length, char *infoLog);
extern void (*lotus_get_programiv)(ubyte4 program, ubyte4 pname, ubyte4 *params);
extern void (*lotus_get_program_info_log)(ubyte4 program, sbyte4 maxLength, sbyte4 *length, char *infoLog);
extern ubyte4 (*lotus_get_uniform_location)(ubyte4 program, const char* name);
extern void (*lotus_uniform2fv)(ubyte4 location, ubyte4 count, const f32* value);
extern void (*lotus_uniform3fv)(ubyte4 location, ubyte4 count, const f32* value);
extern void (*lotus_uniform4fv)(ubyte4 location, ubyte4 count, const f32* value);
extern void (*lotus_uniform_matrix4fv)(ubyte4 location, ubyte4 count, ubyte4 transpose, const f32* value);

// TEXTURE FUNCTIONS

extern void (*lotus_gen_textures)(sbyte4 count, ubyte4 *textures);
extern void (*lotus_bind_texture)(ubyte4 target, ubyte4 texture);
extern void (*lotus_tex_parameteri)(ubyte4 target, ubyte4 pname, ubyte4 param);
extern void (*lotus_tex_image2d)(ubyte4 target, ubyte4 level, ubyte4 internalFormat, sbyte4 width, sbyte4 height, ubyte4 border, ubyte4 format, ubyte4 type, const void *data);
extern void (*lotus_active_texture)(ubyte4 texture);
extern void (*lotus_delete_textures)(sbyte4 count, const ubyte4 *textures);
extern void (*lotus_generate_mipmap)(ubyte4 target);
extern void (*lotus_generate_texture_mipmap)(ubyte4 texture);

// FRAMEBUFFER FUNCTIONS

extern void (*lotus_gen_framebuffers)(sbyte4 count, ubyte4 *framebuffers);
extern void (*lotus_bind_framebuffer)(ubyte4 target, ubyte4 framebuffer);
extern void (*lotus_framebuffer_texture2d)(ubyte4 target, ubyte4 attachment, ubyte4 textarget, ubyte4 texture, ubyte4 level);
extern void (*lotus_delete_framebuffers)(sbyte4 count, const ubyte4 *framebuffers);
extern ubyte4 (*lotus_check_framebuffer_status)(ubyte4 target);

// DRAWING FUNCTIONS

extern void (*lotus_clear)(sbyte4 mask);
extern void (*lotus_draw_arrays)(ubyte4 mode, ubyte4 first, sbyte4 count);
extern void (*lotus_clear_color)(f32 r, f32 g, f32 b, f32 a);
extern void (*lotus_draw_elements)(ubyte4 mode, sbyte4 count, ubyte4 type, const void *indices);
extern void (*lotus_draw_elements_base_vertex)(ubyte4 mode, sbyte4 count, ubyte4 type, const void *indices, ubyte4 baseVertex);

// STATE MANAGEMENT FUNCTIONS

extern void (*lotus_enable)(ubyte4 cap);
extern void (*lotus_disable)(ubyte4 cap);
extern void (*lotus_blend_func)(ubyte4 sfactor, ubyte4 dfactor);
extern void (*lotus_cull_face)(ubyte4 mode);
extern void (*lotus_depth_func)(ubyte4 func);
extern void (*lotus_viewport)(ubyte4 x, ubyte4 y, sbyte4 width, sbyte4 height);
extern void (*lotus_polygon_mode)(ubyte4 face, ubyte4 mode);

// UTILITY FUNCTIONS

extern ubyte4 (*lotus_get_error)(void);
extern const ubyte *(*lotus_get_string)(const ubyte4 name);
