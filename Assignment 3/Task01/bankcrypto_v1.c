#include <stdint.h>

/* Teaching-only deterministic transform. This is not encryption. */
uint64_t bank_transform(uint64_t value)
{
    return value ^ UINT64_C(0xA5A5A5A5A5A5A5A5);
}

const char *bankcrypto_version(void)
{
    return "bankcrypto-library-v1";
}
