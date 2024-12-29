#include "../include/platform/lotus_memory.h"

#include "../include/platform/lotus_logger.h"

lotus_allocator lotus_make_allocator(size_t heap_size) {
    lotus_allocator alloc;
    
    alloc.heap_start = malloc(heap_size);
    if (alloc.heap_start == (void*)0) {
        return (lotus_allocator){NULL};  // allocation failed
    }

    alloc.heap_size = heap_size;
    alloc.free_list = (lotus_memory_itf*)alloc.heap_start;

    alloc.free_list->size = heap_size - sizeof(lotus_memory_itf);
    alloc.free_list->is_free = LOTUS_TRUE;
    alloc.free_list->next = NULL;

    return alloc;
}

lotus_memory lotus_alloc(lotus_allocator* alloc, size_t size, size_t alignment) {
    lotus_memory_itf* current = alloc->free_list;
    lotus_memory mem = { .ptr = NULL, .size = 0 };

    size = lotus_align(size, alignment);

    while (current) {
        if (current->is_free && current->size >= size) {
            // split block if theres extra space
            if (current->size > size + sizeof(lotus_memory_itf)) {
                lotus_memory_itf* new = (lotus_memory_itf*)((char*)current + size + sizeof(lotus_memory_itf));
                new->size = current->size - size - sizeof(lotus_memory_itf);
                new->is_free = LOTUS_TRUE;
                new->next = current->next;

                current->size = size;
                current->next = new;
            }
            current->is_free = LOTUS_FALSE;
            mem.ptr = (char*)current + sizeof(lotus_memory_itf);
            mem.size = current->size;
            return mem;
        }
        current = current->next;
    }
    // allocation failed
    return mem;
}

void lotus_free(lotus_allocator* alloc, lotus_memory mem) {
    lotus_memory_itf* block = (lotus_memory_itf*)((char*)mem.ptr - sizeof(lotus_memory_itf));
    block->is_free = LOTUS_TRUE;

    // coalesce adjacent free blocks
    lotus_memory_itf* current = alloc->free_list;

    while (current) {
        if (current->is_free && current->next && current->next->is_free) {
            current->size += current->next->size + sizeof(lotus_memory_itf);
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

void lotus_destroy_allocator(lotus_allocator* alloc) {
    free(alloc->heap_start);
    alloc->heap_start = NULL;
    alloc->heap_size = 0;
    alloc->free_list = NULL;
}
