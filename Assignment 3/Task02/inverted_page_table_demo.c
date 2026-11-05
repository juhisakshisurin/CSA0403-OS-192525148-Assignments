#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define FRAME_COUNT 8
#define HASH_SIZE 17

typedef struct {
    int valid;
    int pid;
    unsigned virtual_page;
    int frame;
} InvertedEntry;

static InvertedEntry table[HASH_SIZE];

static unsigned hash_key(int pid, unsigned vpn)
{
    return ((unsigned)pid * 31U + vpn) % HASH_SIZE;
}

static void insert_mapping(int pid, unsigned vpn, int frame)
{
    unsigned slot = hash_key(pid, vpn);

    for (unsigned probes = 0; probes < HASH_SIZE; ++probes) {
        unsigned i = (slot + probes) % HASH_SIZE;

        if (!table[i].valid ||
            (table[i].pid == pid &&
             table[i].virtual_page == vpn)) {
            table[i].valid = 1;
            table[i].pid = pid;
            table[i].virtual_page = vpn;
            table[i].frame = frame;
            return;
        }
    }

    puts("Inverted table hash is full.");
}

static int lookup_frame(int pid, unsigned vpn)
{
    unsigned slot = hash_key(pid, vpn);

    for (unsigned probes = 0; probes < HASH_SIZE; ++probes) {
        unsigned i = (slot + probes) % HASH_SIZE;

        if (!table[i].valid) return -1;

        if (table[i].pid == pid &&
            table[i].virtual_page == vpn) {
            return table[i].frame;
        }
    }

    return -1;
}

int main(void)
{
    /* Eight physical frames are represented by eight resident mappings.
       In a real inverted page table, one entry exists per physical frame. */
    insert_mapping(10, 0, 0);
    insert_mapping(10, 1, 1);
    insert_mapping(10, 4, 2);
    insert_mapping(20, 0, 3);
    insert_mapping(20, 2, 4);
    insert_mapping(30, 3, 5);
    insert_mapping(30, 4, 6);
    insert_mapping(30, 7, 7);

    const int pid[] = {10, 10, 20, 30, 40};
    const unsigned vpn[] = {0, 3, 2, 7, 0};

    puts("Inverted page-table hash lookup demonstration");
    for (size_t i = 0; i < sizeof(pid) / sizeof(pid[0]); ++i) {
        int frame = lookup_frame(pid[i], vpn[i]);

        if (frame >= 0) {
            printf("PID %d virtual page %u -> physical frame %d\n",
                   pid[i], vpn[i], frame);
        } else {
            printf("PID %d virtual page %u -> not resident / no mapping\n",
                   pid[i], vpn[i]);
        }
    }

    printf("Physical frames represented: %d\n", FRAME_COUNT);
    printf("Average hash lookup: expected O(1); worst case O(FRAME_COUNT).\n");
    return 0;
}
