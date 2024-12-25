#include "sofileio.h"

#include "../platform/sologger.h"

ubyte sogl_is_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) return SOGL_FALSE;
    fclose(file);
    return SOGL_TRUE;
}

char* sogl_read_file(const char* path) {
    FILE* file = fopen(path, "rb");
    sogl_set_log_level(SOGL_LOG_ERROR);

    if (!file) {
        sogl_log_error("Failed to open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    if (length < 0) {
        sogl_log_error("Failed to determine file size");
        fclose(file);
        return NULL;
    }
    fseek(file, 0, SEEK_SET);

    char* content = (char*)malloc(length + 1);
    if (!content) {
        sogl_log_error("Failed to allocate memory for file content\n");
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(content, sizeof(char), length, file);
    if (bytesRead != (size_t)length) {
        sogl_log_error("Failed to read the entire file\n");
        free(content);
        fclose(file);
        return NULL;
    }

    content[length] = '\0';
    fclose(file);
    return content;
}

ubyte sogl_delete_file(const char* path) {
    if (remove(path) != SOGL_FALSE) {
        sogl_set_log_level(SOGL_LOG_ERROR);
        sogl_log_error("Failed to desogl_e file at path: %s\n", path);
        return SOGL_FALSE;
    }
    return SOGL_TRUE;
}

long sogl_get_file_size(const char* path) {
    FILE* file = fopen(path, "rb");

    if (!file) {
        sogl_set_log_level(SOGL_LOG_ERROR);
        sogl_log_error("failed to open file at path: %s\n", path);
        return SOGL_FALSE;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    return size;
}

ubyte sogl_copy_file(const char* dest, const char* src) {
    FILE* srcFile = fopen(src, "rb");
    if (!src) {
        sogl_set_log_level(SOGL_LOG_ERROR);
        sogl_log_error("Failed to open source file: %s\n", src);
        return SOGL_FALSE;
    }

    FILE* destFile = fopen(dest, "wb");
    if (!dest) {
        sogl_log_error("Failed to open destination file: %s\n", dest);
        fclose(srcFile);
        return SOGL_FALSE;
    }

    size_t bytesRead;
    char buffer[4096];
    do {
        fwrite(buffer, sizeof(char), bytesRead, destFile);
    } while ((bytesRead = fread(buffer, sizeof(char), sizeof(buffer), srcFile)) > 0);

    fclose(srcFile);
    fclose(destFile);
    return SOGL_TRUE;
}

ubyte sogl_append_file(const char* data, const char* path, ubyte newline) {
    if (newline) sogl_write_file("\n", path, 1);
    return sogl_write_file(data, path, 1);
}

ubyte sogl_write_file(const char* data, const char* path, ubyte preserve) {
    FILE* file = fopen(path, preserve ? "ab" : "wb");
    sogl_set_log_level(SOGL_LOG_ERROR);
    if (!file) {
        sogl_log_error("Failed to open file");
        return SOGL_FALSE;
    }

    size_t length = strlen(data);
    if (length <= 0) {
        sogl_log_error("Faield to compute length of write data\n");
        fclose(file);
        return SOGL_FALSE;
    }
    
    size_t bytesWritten = fwrite(data, sizeof(char), length, file);
    if (bytesWritten != length) {
        sogl_log_error("Failed to fully write data to file at path: %s\n", path);
        fclose(file);
        return SOGL_FALSE;
    }

    fclose(file);
    return SOGL_TRUE;
}

ubyte sogl_process_file(void (*processLine)(const char* line), const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        sogl_set_log_level(SOGL_LOG_ERROR);
        sogl_log_error("Failed to open file at path: %s\n", path);
        return SOGL_FALSE;
    }

    char line[1024];
    do {
        processLine(line);
    } while(fgets(line, sizeof(line), file));

    fclose(file);
    return SOGL_TRUE;
}
