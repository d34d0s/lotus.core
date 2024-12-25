#pragma once

#include "../types.h"
#include "../defines.h"

typedef struct sogl_key_value {
    const char* k;
    void* v;
} sogl_key_value;

typedef struct sogl_hashmap {
    int max;
    int count;
    sogl_key_value** map;
} sogl_hashmap;

/**
 * @brief Creates a new hashmap with a specified maximum number of entries.
 * 
 * @param max The maximum number of entries the hashmap can hold.
 * @return A pointer to the newly created hashmap, or NULL on failure.
 */
SOGL_API_ENTRY sogl_hashmap* sogl_make_hashmap(int max);

/**
 * @brief Destroys a hashmap and frees all associated memory.
 * 
 * @param m A pointer to the hashmap to destroy.
 */
SOGL_API_ENTRY void sogl_destroy_hashmap(sogl_hashmap* m);

/**
 * @brief Retrieves a value from the hashmap using the specified key.
 * 
 * @param m A pointer to the hashmap.
 * @param key The key to look up.
 * @return A pointer to the value associated with the key, or NULL if the key is not found.
 */
SOGL_API_ENTRY void* sogl_get_hashmap(sogl_hashmap* m, const char* key);

/**
 * @brief Removes a key-value pair from the hashmap.
 * 
 * @param m A pointer to the hashmap.
 * @param key The key to remove.
 * @return `SOGL_SUCCESS` on success, or an appropriate error code if the operation fails.
 */
SOGL_API_ENTRY sogl_error_type sogl_rem_hashmap(sogl_hashmap* m, const char* key);

/**
 * @brief Sets a key-value pair in the hashmap. If the key already exists, its value is updated.
 * 
 * @param m A pointer to the hashmap.
 * @param key The key to set.
 * @param value A pointer to the value to associate with the key.
 * @return `SOGL_SUCCESS` on success, or an appropriate error code if the operation fails.
 */
SOGL_API_ENTRY sogl_error_type sogl_set_hashmap(sogl_hashmap* m, const char* key, void* value);
