#include "sohashmap.h"

#include "../platform/sologger.h"

_SOGL_PRIVATE int sogl_string_hash(const char* buffer) {
    int res = 0;
    size_t size = strlen(buffer);
    for(int i = 0; i < size; i++) {
        res+=(int)buffer[i]*31;
    }; return res;
}

_SOGL_PRIVATE bool sogl_probe_hashmap_f(sogl_hashmap* m, int* kHash, const char* key) {
    bool match = 0;
    sogl_key_value* kvp = m->map[*kHash];
    
    for (int i = *kHash+1; i < m->max; i++) {
        kvp = m->map[i];
        if (key == NULL) {
            if (!kvp) {
                match = 1;
                *kHash = i;
                break;
            } else continue;
        } else {
            if (kvp && !strcmp(key, kvp->k)) {
                *kHash = i;
                match = 1;
                break;
            } else continue;
        }
    } return match;
}

_SOGL_PRIVATE bool sogl_probe_hashmap_r(sogl_hashmap* m, int* kHash, const char* key) {
    bool match = 0;
    sogl_key_value* kvp = m->map[*kHash];
    
    for (int i = *kHash-1; i > 0; i--) {
        kvp = m->map[i];
        if (key == NULL) {
            if (!kvp) {
                match = 1;
                *kHash = i;
                break;
            } else continue;
        } else {
            if (kvp && !strcmp(key, kvp->k)) {
                match = 1;
                *kHash = i;
                break;
            } else continue;
        }
    } return match;
}

sogl_hashmap* sogl_make_hashmap(int max) {
    sogl_hashmap* m = (sogl_hashmap*)malloc(sizeof(sogl_hashmap));
    if (!m) {
        sogl_set_log_level(SOGL_LOG_ERROR);
        sogl_log_error("failed to allocate hashmap");
        return NULL;
    }

    m->max = max;
    m->count = 0;

    m->map = (sogl_key_value**)calloc(max, sizeof(sogl_key_value*));
    if (!m->map) {
        free(m);
        sogl_set_log_level(SOGL_LOG_ERROR);
        sogl_log_error("failed to allocate hashmap array");
        return NULL;
    }

    return m;
}

void sogl_destroy_hashmap(sogl_hashmap* m) {
    for (int i = 0; i < m->max; i++) {
        if (m->map[i]) {
            free(m->map[i]->v);
            free(m->map[i]);
        }
    }
    free(m->map);
    free(m);
}

void* sogl_get_hashmap(sogl_hashmap* m, const char* key) {
    if (!key) { return NULL; }

    int kHash = sogl_string_hash(key) % m->max;
    sogl_key_value* kvp = m->map[kHash];

    if (kvp && strcmp(key, kvp->k)) {
        bool match = 0;

        // forward probing
        match = sogl_probe_hashmap_f(m, &kHash, key);

        // reverse probing
        if (!match) {
            match = sogl_probe_hashmap_r(m, &kHash, key);
        }
        
        if (!match) {
            sogl_set_log_level(SOGL_LOG_ERROR);
            sogl_log_error("probing error | key [%s] is not set", key);
            return NULL;
        }
        
        kvp = m->map[kHash];
    }; return kvp->v;
}

sogl_error_type sogl_set_hashmap(sogl_hashmap* m, const char* key, void* value) {
    if (!key || !value || m->count+1 > m->max) { return SOGL_ERR_TYPE; }

    int kHash = sogl_string_hash(key) % m->max;
    sogl_key_value* kvp = m->map[kHash];

    // resolve collisions with open addressing + linear probing
    if (kvp) {
        if (!strcmp(key, kvp->k)) {
            m->map[kHash]->v = value;
            return SOGL_ERR_NONE;        
        }

        bool set = 0;
        
        // forward probing
        set = sogl_probe_hashmap_f(m, &kHash, NULL);

        // reverse probing
        if (!set) set = sogl_probe_hashmap_r(m, &kHash, NULL);

        if (!set) {
            sogl_set_log_level(SOGL_LOG_ERROR);
            sogl_log_error("probing error | key[%s]", key);
            return SOGL_ERR_FUNC;
        }
    }

    m->map[kHash] = (sogl_key_value*)malloc(sizeof(sogl_key_value));
    m->map[kHash]->k = strdup(key);
    m->map[kHash]->v = value;
    m->count++;
    return SOGL_ERR_NONE;
}

sogl_error_type sogl_rem_hashmap(sogl_hashmap* m, const char* key) {
    if (!key) { return SOGL_ERR_TYPE; }

    int kHash = sogl_string_hash(key) % m->max;
    sogl_key_value* kvp = m->map[kHash];

    if (kvp && strcmp(key, kvp->k)) {
        bool match = 0;

        // forward probing
        match = sogl_probe_hashmap_f(m, &kHash, key);

        // reverse probing
        if (!match) {
            match = sogl_probe_hashmap_r(m, &kHash, key);
        }
        
        if (!match) {
            sogl_set_log_level(SOGL_LOG_ERROR);
            sogl_log_error("probing error | key [%s] is not set", key);
            return SOGL_ERR_FUNC;
        }
        
        kvp = m->map[kHash];
    };
    
    free(m->map[kHash]->v);
    free(m->map[kHash]);
    m->map[kHash] = NULL;
    return SOGL_ERR_NONE;
}
