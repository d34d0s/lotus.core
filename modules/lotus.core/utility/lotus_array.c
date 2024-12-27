#include "lotus_array.h"

void lotus_print_array(char* tag, void* array) {
    ubyte4* header = (ubyte4*)((char*)array - LOTUS_ARRAY_HEADER_SIZE);
    ubyte4 size = header[0];
    ubyte4 stride = header[1];
    ubyte4 length = header[2];
    ubyte4 capacity = header[3];

    printf("|(Array): %s\n", tag);
    printf("|(size): %d bytes (stride): %d bytes (length): %d elems (capacity): %d elems\n", size, stride, length, capacity);
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

void lotus_destroy_array(void* array) {
    ubyte4* header = (ubyte4*)((char*)array - LOTUS_ARRAY_HEADER_SIZE);
    free(header);
}

void* lotus_resize_array(void* array, ubyte4 new_capacity) {
    ubyte4* header = (ubyte4*)((char*)array - LOTUS_ARRAY_HEADER_SIZE);
    ubyte4 stride = LOTUS_ARRAY_STRIDE(array);

    ubyte4 total_size = LOTUS_ARRAY_HEADER_SIZE + (new_capacity * stride);

    void* temp = realloc(header, total_size);
    if (!temp) {
        printf("Failed to re-allocate array!\n");
        return NULL;
    }

    header = (ubyte4*)((char*)temp);
    header[0] = LOTUS_ARRAY_HEADER_SIZE + (new_capacity * stride);
    header[2] = (header[2] > new_capacity) ? new_capacity : header[2];
    header[3] = new_capacity;

    void* content = (void*)((char*)temp + LOTUS_ARRAY_HEADER_SIZE);
    return content;
}

void lotus_pop_array(void* array, void* out_value) {
    ubyte4* header = (ubyte4*)((char*)array - LOTUS_ARRAY_HEADER_SIZE);
    ubyte4 stride = LOTUS_ARRAY_STRIDE(array);
    ubyte4 length = LOTUS_ARRAY_LENGTH(array);

    if (length == 0) {
        printf("Array is empty!\n");
        return;
    }

    void* slot = (char*)array + ((length - 1) * stride);
    memcpy(out_value, slot, stride);
    header[2]--;
}

void lotus_push_array(void* array, void* in_value) {
    ubyte4* header = (ubyte4*)((char*)array - LOTUS_ARRAY_HEADER_SIZE);
    ubyte4 stride = LOTUS_ARRAY_STRIDE(array);
    ubyte4 length = LOTUS_ARRAY_LENGTH(array);
    ubyte4 capacity = LOTUS_ARRAY_CAPACITY(array);

    if (length >= capacity) {
        array = lotus_resize_array(array, capacity * 2);
        header = (ubyte4*)((char*)array - LOTUS_ARRAY_HEADER_SIZE);
    }

    void* slot = (char*)array + (length * stride);
    memcpy(slot, in_value, stride);
    header[2]++;
}

void lotus_insert_array(void* array, ubyte4 index, void* in_value) {
    ubyte4* header = (ubyte4*)((char*)array - LOTUS_ARRAY_HEADER_SIZE);
    ubyte4 stride = LOTUS_ARRAY_STRIDE(array);
    ubyte4 length = LOTUS_ARRAY_LENGTH(array);
    ubyte4 capacity = LOTUS_ARRAY_CAPACITY(array);

    if (index > capacity) {
        printf("Index out of bounds!\n");
        return;
    }

    if (index > length | length >= capacity) {
        array = lotus_resize_array(array, capacity*2);
        header = (ubyte4*)((char*)array - LOTUS_ARRAY_HEADER_SIZE);
    }

    void* slot = (char*)array + (index * stride);
    if (index < length) memmove((char*)slot + stride, slot, (length - index) * stride);

    memcpy((char*)slot, in_value, stride);
    header[2]++;
}
