#pragma once

#include "../defines.h"

#define LOTUS_LITERAL(type, value) &(type){value}

#define LOTUS_ARRAY_HEADER_SIZE (4 * sizeof(ubyte4))
#define LOTUS_ARRAY_SIZE(array)     ((ubyte4*)((char*)array - LOTUS_ARRAY_HEADER_SIZE))[0]
#define LOTUS_ARRAY_STRIDE(array)   ((ubyte4*)((char*)array - LOTUS_ARRAY_HEADER_SIZE))[1]
#define LOTUS_ARRAY_LENGTH(array)   ((ubyte4*)((char*)array - LOTUS_ARRAY_HEADER_SIZE))[2]
#define LOTUS_ARRAY_CAPACITY(array) ((ubyte4*)((char*)array - LOTUS_ARRAY_HEADER_SIZE))[3]

LOTUS_API_ENTRY void* lotus_make_array(ubyte4 stride, ubyte4 capacity);
LOTUS_API_ENTRY void lotus_destroy_array(void* array);

LOTUS_API_ENTRY void lotus_print_array(char* tag, void* array);

LOTUS_API_ENTRY void lotus_push_array(void* array, void* in_value);
LOTUS_API_ENTRY void lotus_pop_array(void* array, void* out_value);
LOTUS_API_ENTRY void* lotus_resize_array(void* array, ubyte4 new_capacity);
LOTUS_API_ENTRY void lotus_insert_array(void* array, ubyte4 index, void* in_value);
