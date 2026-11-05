#include <stdint.h>

/* Replacement implementation with the same exported ABI.
   This is a demonstration transform, NOT real cryptography. */
uint64_t bank_transform(uint64_t value)
{
    return value ^ UINT64_C(0x5A5A5A5A5A5A5A5A);
}

const char *bankcrypto_version(void)
{
    return "bankcrypto-library-v2";
}
