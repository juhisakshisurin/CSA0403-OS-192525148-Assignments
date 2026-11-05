#define _POSIX_C_SOURCE 200809L
#include <dlfcn.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint64_t (*transform_fn)(uint64_t);
typedef const char *(*version_fn)(void);

static void show_library_maps(void)
{
    FILE *fp = fopen("/proc/self/maps", "r");
    if (fp == NULL) {
        perror("/proc/self/maps");
        return;
    }

    char line[512];
    puts("Library mappings:");
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strstr(line, "libbankcrypto") != NULL) {
            fputs(line, stdout);
        }
    }
    fclose(fp);
}

int main(int argc, char **argv)
{
    if (argc < 2 || strcmp(argv[1], "--encrypt") != 0) {
        puts("Encryption library not loaded.");
        puts("Run with: ./bank_app_dlopen --encrypt [library-path]");
        show_library_maps();
        return EXIT_SUCCESS;
    }

    const char *path = (argc >= 3) ? argv[2] : "./lib/libbankcrypto.so.1";

    /* Alternative design: load the library only when encryption is needed. */
    void *handle = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    if (handle == NULL) {
        fprintf(stderr, "Could not load optional library '%s': %s\n",
                path, dlerror());
        fprintf(stderr, "Continuing without the optional encryption feature.\n");
        return EXIT_SUCCESS;
    }

    (void)dlerror();
    void *transform_symbol = dlsym(handle, "bank_transform");
    const char *error = dlerror();

    if (error != NULL || transform_symbol == NULL) {
        fprintf(stderr, "Could not find bank_transform: %s\n",
                error ? error : "symbol not found");
        dlclose(handle);
        return EXIT_FAILURE;
    }

    transform_fn transform = NULL;
    memcpy(&transform, &transform_symbol, sizeof(transform));

    (void)dlerror();
    void *version_symbol = dlsym(handle, "bankcrypto_version");
    error = dlerror();

    if (error != NULL || version_symbol == NULL) {
        fprintf(stderr, "Could not find bankcrypto_version: %s\n",
                error ? error : "symbol not found");
        dlclose(handle);
        return EXIT_FAILURE;
    }

    version_fn version = NULL;
    memcpy(&version, &version_symbol, sizeof(version));

    printf("Loaded on demand: %s\n", path);
    printf("Library version: %s\n", version());
    printf("Demo transformation (not real cryptography): 0x%016" PRIx64 "\n",
           transform(UINT64_C(0x1122334455667788)));

    show_library_maps();

    if (dlclose(handle) != 0) {
        fprintf(stderr, "dlclose failed: %s\n", dlerror());
        return EXIT_FAILURE;
    }

    puts("Library handle closed.");
    return EXIT_SUCCESS;
}
