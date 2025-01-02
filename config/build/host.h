#ifndef HOST_H
#define HOST_H

/* Check if musl libc is being used */
#define LIBC_CHECK "$(shell ldd --version 2>&1 | grep -i musl)"

/* Default compiler */
#define CC "gcc"

/* Use musl-gcc if musl libc is detected */
#ifdef LIBC_CHECK
#undef CC
#define CC "musl-gcc"
#endif

#endif /* HOST_H */
