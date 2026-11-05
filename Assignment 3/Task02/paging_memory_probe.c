#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define ALLOC_BYTES (64U * 1024U * 1024U)

int main(void)
{
    long page_size = sysconf(_SC_PAGESIZE);
    if (page_size <= 0) {
        fprintf(stderr, "Could not determine system page size.\n");
        return EXIT_FAILURE;
    }

    unsigned char *memory = malloc(ALLOC_BYTES);
    if (memory == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    /* Touch one byte per native page to make the pages resident. */
    uint64_t checksum = 0;
    for (size_t offset = 0; offset < ALLOC_BYTES; offset += (size_t)page_size) {
        memory[offset] = (unsigned char)((offset / (size_t)page_size) & 0xffU);
        checksum += memory[offset];
    }

    printf("Allocated and touched %u bytes using native page size %ld bytes.\n",
           ALLOC_BYTES, page_size);
    printf("Checksum: %llu\n", (unsigned long long)checksum);

    free(memory);
    return 0;
}
