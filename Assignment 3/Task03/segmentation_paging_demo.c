#include <stdio.h>

#define PAGE_SIZE 4096
#define PTE_COUNT 8
#define SEGMENTS 2

typedef struct {
    const char *name;
    unsigned base;       /* segment base in the linear address space */
    unsigned limit;      /* inclusive segment limit */
} SegmentDescriptor;

typedef struct {
    unsigned linear_page;
    int frame;
} PageEntry;

/* Simplified illustrative page map for the linear addresses used below.
   A real x86 MMU uses hardware-defined directory/table structures. */
static const SegmentDescriptor segments[SEGMENTS] = {
    {"CODE", 0x00400000, 0x00002FFF},
    {"DATA", 0x00600000, 0x00001FFF}
};

static const PageEntry page_table[PTE_COUNT] = {
    {0x00400, 10},
    {0x00401, 11},
    {0x00402,  4},
    {0x00600, 20},
    {0x00601, 21},
    {0x00602, -1},
    {0x00603,  7},
    {0x00604,  8}
};

static int find_frame(unsigned linear_page)
{
    for (int i = 0; i < PTE_COUNT; ++i) {
        if (page_table[i].linear_page == linear_page)
            return page_table[i].frame;
    }
    return -1;
}

static void translate(int seg, unsigned offset)
{
    if (seg < 0 || seg >= SEGMENTS) {
        puts("invalid segment selector");
        return;
    }

    const SegmentDescriptor *d = &segments[seg];

    if (offset > d->limit) {
        printf("%s:%04X -> segmentation limit fault\n",
               d->name, offset);
        return;
    }

    unsigned linear = d->base + offset;
    unsigned vpn = linear / PAGE_SIZE;
    unsigned page_offset = linear % PAGE_SIZE;
    int frame = find_frame(vpn);

    if (frame < 0) {
        printf("%s:%04X -> linear 0x%08X -> page not present\n",
               d->name, offset, linear);
        return;
    }

    unsigned physical = (unsigned)frame * PAGE_SIZE + page_offset;

    printf("%s:%04X -> segment base + offset = linear 0x%08X; "
           "VPN=0x%X offset=0x%X frame=%d -> physical 0x%08X\n",
           d->name, offset, linear, vpn, page_offset,
           frame, physical);
}

int main(void)
{
    puts("Illustrative x86-style two-stage translation:");
    puts("logical selector:offset -> segmentation -> linear address");
    puts("linear address -> paging structures -> physical frame:offset\n");

    translate(0, 0x0123);
    translate(0, 0x1ABC);
    translate(1, 0x0040);
    translate(1, 0x2040);
    return 0;
}
