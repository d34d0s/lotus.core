#include "../../include/lotus.h"

#define LOTUS_ADDR_OF(x) &x

#define LOTUS_ARRAY_HEADER_SIZE (4 * sizeof(ubyte4))
/* Array Header                           Array Content
    (ubyte4[4]) {size, stride, length, capacity} | (void*){...}
*/
void lotus_print_array(char* tag, void* array) {
    ubyte4* header = (ubyte4*)((char*)array - LOTUS_ARRAY_HEADER_SIZE);
    ubyte4 size = header[0];
    ubyte4 stride = header[1];
    ubyte4 length = header[2];
    ubyte4 capacity = header[3];

    printf("|(Array) %s\n", tag);
    printf("|(Header): <(size)%d bytes (stride)%d bytes (length)%d elems (capacity)%d elems>\n", size, stride, length, capacity);
}

void* lotus_make_array(ubyte4 stride, ubyte4 capacity) {
    void* arr = malloc(LOTUS_ARRAY_HEADER_SIZE + (capacity * stride));
    if (!arr) {
        printf("Failed to allocate array!\n");
        return NULL;
    }

    ubyte4* header = (ubyte4*)((char*)arr);
    header[0] = LOTUS_ARRAY_HEADER_SIZE + (capacity * stride);
    header[1] = stride;
    header[2] = 0;
    header[3] = capacity;

    void* content = (void*)((char*)arr + LOTUS_ARRAY_HEADER_SIZE);
    return content;
}

void lotus_resize_array(void* array, ubyte4 new_capacity) {
    ubyte4* header = (ubyte4*)((char*)array - LOTUS_ARRAY_HEADER_SIZE);
    ubyte4 size = header[0];
    ubyte4 stride = header[1];
    ubyte4 length = header[2];
    ubyte4 capacity = header[3];

}

void* lotus_pop_array(void* array) {

}

void lotus_push_array(void* array, void* value) {

}

void lotus_insert_array(void* array, ubyte4 index, void* value) {

}

void lotus_destroy_array(void* array) {

}



int main() {
    f32* vertices = lotus_make_array(sizeof(ubyte4), 3);

    vertices[0] = 666.0;
    vertices[1] = 420.0;
    vertices[2] = 69.0;

    for (int i = 0; i < 3; i++) {
        printf("Vertex: %0.1f\n", vertices[i]);
    }

    lotus_print_array("Vertices", vertices);

    return 0;
}
