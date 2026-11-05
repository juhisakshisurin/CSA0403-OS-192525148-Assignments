#include <stdio.h>
#include <string.h>

#define SEGMENTS 3

typedef struct {
    const char *name;
    unsigned base;
    unsigned limit;      /* inclusive maximum valid offset */
    const char *permissions;
} Segment;

static const Segment segment_table[SEGMENTS] = {
    {"CODE",  0x10000, 0x0FFF, "R-X"},
    {"DATA",  0x30000, 0x07FF, "RW-"},
    {"STACK", 0x70000, 0x03FF, "RW-"}
};

static void translate(int segment, unsigned offset)
{
    if (segment < 0 || segment >= SEGMENTS) {
        printf("segment %d offset 0x%04X -> invalid segment\n",
               segment, offset);
        return;
    }

    const Segment *s = &segment_table[segment];

    if (offset > s->limit) {
        printf("%-5s offset 0x%04X -> SEGMENTATION FAULT "
               "(limit 0x%04X)\n",
               s->name, offset, s->limit);
        return;
    }

    unsigned physical = s->base + offset;

    printf("%-5s offset 0x%04X -> base 0x%05X + offset "
           "0x%04X = physical 0x%05X (%u), perms=%s\n",
           s->name, offset, s->base, offset,
           physical, physical, s->permissions);
}

int main(void)
{
    /* Eight test addresses: valid starts/ends and offsets beyond limits. */
    const int seg[] = {0, 0, 0, 0, 1, 1, 2, 2};
    const unsigned off[] = {
        0x0000, 0x0123, 0x0FFF, 0x1000,
        0x0040, 0x0800, 0x03FF, 0x0400
    };

    puts("Segment table:");
    for (int i = 0; i < SEGMENTS; ++i) {
        printf("%d %-5s base=0x%05X limit=0x%04X permissions=%s\n",
               i, segment_table[i].name, segment_table[i].base,
               segment_table[i].limit, segment_table[i].permissions);
    }

    puts("\nEight logical (segment, offset) translations:");
    for (int i = 0; i < 8; ++i) {
        translate(seg[i], off[i]);
    }

    return 0;
}
