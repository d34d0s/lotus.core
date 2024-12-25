#include "somemory.h"

#include "../platform/sologger.h"

sogl_allocator sogl_make_allocator(size_t heap_size) {
    sogl_allocator alloc;
    
    alloc.heap_start = malloc(heap_size);
    if (alloc.heap_start == (void*)0) {
        return (sogl_allocator){NULL};  // allocation failed
    }

    alloc.heap_size = heap_size;
    alloc.free_list = (sogl_memory_itf*)alloc.heap_start;

    alloc.free_list->size = heap_size - sizeof(sogl_memory_itf);
    alloc.free_list->is_free = SOGL_TRUE;
    alloc.free_list->next = NULL;

    return alloc;
}

sogl_memory sogl_alloc(sogl_allocator* alloc, size_t size, size_t alignment) {
    sogl_memory_itf* current = alloc->free_list;
    sogl_memory mem = { .ptr = NULL, .size = 0 };

    size = sogl_align(size, alignment);

    while (current) {
        if (current->is_free && current->size >= size) {
            // split block if theres extra space
            if (current->size > size + sizeof(sogl_memory_itf)) {
                sogl_memory_itf* new = (sogl_memory_itf*)((char*)current + size + sizeof(sogl_memory_itf));
                new->size = current->size - size - sizeof(sogl_memory_itf);
                new->is_free = SOGL_TRUE;
                new->next = current->next;

                current->size = size;
                current->next = new;
            }
            current->is_free = SOGL_FALSE;
            mem.ptr = (char*)current + sizeof(sogl_memory_itf);
            mem.size = current->size;
            return mem;
        }
        current = current->next;
    }
    // allocation failed
    return mem;
}

void sogl_free(sogl_allocator* alloc, sogl_memory mem) {
    sogl_memory_itf* block = (sogl_memory_itf*)((char*)mem.ptr - sizeof(sogl_memory_itf));
    block->is_free = SOGL_TRUE;

    // coalesce adjacent free blocks
    sogl_memory_itf* current = alloc->free_list;

    while (current) {
        if (current->is_free && current->next && current->next->is_free) {
            current->size += current->next->size + sizeof(sogl_memory_itf);
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

void sogl_destroy_allocator(sogl_allocator* alloc) {
    free(alloc->heap_start);
    alloc->heap_start = NULL;
    alloc->heap_size = 0;
    alloc->free_list = NULL;
}
