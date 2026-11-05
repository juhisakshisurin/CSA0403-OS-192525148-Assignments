#include <stdio.h>

#ifndef PAGE_SIZE
#define PAGE_SIZE 1024
#endif

#define PAGES 8

/* Page table of a process: frame number, or -1 if the page is not in memory. */
int page_table[PAGES] = {5, 2, -1, 7, 0, -1, 3, -1};

static void translate(unsigned logical)
{
    unsigned p = logical / PAGE_SIZE;
    unsigned d = logical % PAGE_SIZE;

    if (p >= PAGES) {
        printf("%5u -> page %u offset %6u : invalid address\n",
               logical, p, d);
    } else if (page_table[p] == -1) {
        printf("%5u -> page %u offset %6u : page fault\n",
               logical, p, d);
    } else {
        unsigned physical =
            (unsigned)page_table[p] * PAGE_SIZE + d;

        printf("%5u -> page %u offset %6u : physical %u\n",
               logical, p, d, physical);
    }
}

int main(void)
{
    unsigned logical[] = {0, 1500, 3100, 6200, 9000, 2049};

    printf("Page size = %d bytes; page-table entries = %d\n",
           PAGE_SIZE, PAGES);
    for (int i = 0; i < 6; i++) {
        translate(logical[i]);
    }

    puts("\nPage-boundary tests:");
    unsigned boundary[] = {
        0,
        PAGE_SIZE - 1,
        PAGE_SIZE,
        PAGES * PAGE_SIZE - 1,
        PAGES * PAGE_SIZE
    };

    for (size_t i = 0; i < sizeof(boundary) / sizeof(boundary[0]); ++i) {
        translate(boundary[i]);
    }

    return 0;
}
