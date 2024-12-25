# **soarray.h** - Dynamic Array Abstraction

This header provides an abstraction for dynamic arrays in the `sogl` library, offering an efficient way to manage and manipulate arrays in memory. It uses a custom memory allocator (`sogl_allocator`) for dynamic resizing and includes attributes to store metadata like array size, stride, and capacity. The API supports resizing, adding, and removing elements, with macros for easy access and modification.

---

## **Structures**

### `sogl_array`
A structure representing a dynamic array. It includes metadata, a pointer to the allocated memory block, and the array's content.

```c
typedef struct sogl_array {
    sogl_memory mem;    // Allocated memory of the array
    ubyte8* header;     // Metadata for array attributes
    void* content;      // Pointer to the array's content
} sogl_array;
```

- **mem**: The memory block used by the array, managed by the `sogl_allocator`.
- **header**: A pointer to the metadata block, which contains attributes like size, stride, and capacity.
- **content**: A pointer to the array elements.

---

## **Enums**

### `sogl_array_attr`
Defines the attributes stored in the `header` metadata for a `sogl_array`.

```c
typedef enum sogl_array_attr {
    SOGL_ARRAY_SIZE=0,    // Current number of elements in the array
    SOGL_ARRAY_STRIDE,    // Size of each element in bytes
    SOGL_ARRAY_LENGTH,    // Maximum number of elements the array can hold
    SOGL_ARRAY_ATTRS      // Total number of attributes
} sogl_array_attr;
```

- **SOGL_ARRAY_SIZE**: The current number of elements in the array.
- **SOGL_ARRAY_STRIDE**: The size of each element in bytes.
- **SOGL_ARRAY_LENGTH**: The capacity of the array.
- **SOGL_ARRAY_ATTRS**: The total number of attributes (for internal use).

---

## **Functions**

### `sogl_make_array`
Creates a dynamic array with the specified length and element stride.

```c
SOGL_API_ENTRY sogl_array sogl_make_array(sogl_allocator* alloc, ubyte8 length, ubyte8 stride);
```

- **alloc**: A pointer to the `sogl_allocator` for memory management.
- **length**: The initial capacity of the array.
- **stride**: The size of each element in bytes.
- **Returns**: A `sogl_array` struct representing the dynamic array.

---

### `sogl_destroy_array`
Frees the memory associated with a dynamic array.

```c
SOGL_API_ENTRY void sogl_destroy_array(sogl_allocator* alloc, sogl_array array);
```

- **alloc**: A pointer to the `sogl_allocator` managing the array's memory.
- **array**: The `sogl_array` to be destroyed.

---

### `sogl_resize_array`
Resizes a dynamic array to a new capacity.

```c
SOGL_API_ENTRY void sogl_resize_array(sogl_allocator* alloc, sogl_array* array, ubyte8 new_length);
```

- **alloc**: A pointer to the `sogl_allocator`.
- **array**: A pointer to the `sogl_array` to resize.
- **new_length**: The new capacity of the array.

---

### `sogl_push_array`
Adds a new element to the end of the array.

```c
SOGL_API_ENTRY void sogl_push_array(sogl_allocator* alloc, sogl_array* array, const sogl_memory* element);
```

- **alloc**: A pointer to the `sogl_allocator`.
- **array**: A pointer to the `sogl_array` to modify.
- **element**: A `sogl_memory` struct representing the element to add.

---

### `sogl_pop_array`
Removes and returns the last element of the array.

```c
SOGL_API_ENTRY sogl_memory _sogl_pop_array(sogl_array* array);
```

- **array**: A pointer to the `sogl_array` to modify.
- **Returns**: A `sogl_memory` struct containing the removed element.

---

## **Macros**

### `sogl_get_array`
Retrieves an element at a specific index in the array.

```c
#define sogl_get_array(array, index, type) \
    (sogl_index_array(array, index)) ? ((type*)array.content)[index] : -1
```

- **array**: The `sogl_array` to access.
- **index**: The index of the element.
- **type**: The type of the element.
- **Returns**: The element at the specified index or `-1` if the index is invalid.

---

### `sogl_set_array`
Sets an element at a specific index in the array.

```c
#define sogl_set_array(array, index, type, value) \
    array.header[SOGL_ARRAY_SIZE]++;    \
    ((type*)array.content)[index] = (sogl_index_array(array, index)) ? value : -1
```

- **array**: The `sogl_array` to modify.
- **index**: The index of the element.
- **type**: The type of the element.
- **value**: The value to set.
- **Returns**: `-1` if the index is invalid.

---

### `sogl_pop_array`
Convenience macro to pop an element of a specific type.

```c
#define sogl_pop_array(array, type) *(type*)_sogl_pop_array(&array).ptr
```

- **array**: The `sogl_array` to modify.
- **type**: The type of the element.
- **Returns**: The popped element cast to the specified type.

---

## **Example Usage**

```c
#include "soarray.h"

int main() {
    sogl_allocator alloc = sogl_make_allocator(1024);

    // Create a dynamic array of integers
    sogl_array array = sogl_make_array(&alloc, 10, sizeof(int));

    // Set elements
    for (int i = 0; i < 10; i++) {
        sogl_set_array(array, i, int, i * 2);
    }

    // Get and print elements
    for (int i = 0; i < 10; i++) {
        int value = sogl_get_array(array, i, int);
        printf("Array[%d] = %d\n", i, value);
    }

    // Push a new element
    int new_value = 42;
    sogl_push_array(&alloc, &array, SOGL_MEMORY_REF(new_value));

    // Pop the last element
    int popped = sogl_pop_array(array, int);
    printf("Popped value: %d\n", popped);

    // Destroy the array
    sogl_destroy_array(&alloc, array);

    // Destroy the allocator
    sogl_destroy_allocator(&alloc);

    return 0;
}
```

---

## **Notes**
- The array uses metadata (`header`) to track its size, capacity, and element stride.
- Resizing ensures efficient memory use but may involve reallocation and copying.
- All memory management is handled by the `sogl_allocator`, ensuring consistent behavior across `sogl` modules.