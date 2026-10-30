#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void)
{
    long page_result = sysconf(_SC_PAGESIZE);
    if (page_result <= 0) {
        perror("sysconf(_SC_PAGESIZE)");
        return EXIT_FAILURE;
    }

    const size_t page_size = (size_t) page_result;
    const size_t page_count = 4;
    const size_t length = page_count * page_size;

    char *mem = mmap(NULL, length, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < page_count; ++i) {
        mem[i * page_size] = (char) ('A' + i);
    }

    printf("Page size: %zu bytes\n", page_size);
    printf("Before: %c %c %c %c\n",
           mem[0], mem[page_size],
           mem[2 * page_size], mem[3 * page_size]);

    if (madvise(mem, length, MADV_DONTNEED) == -1) {
        perror("madvise(MADV_DONTNEED)");
        munmap(mem, length);
        return EXIT_FAILURE;
    }

    printf("After : %d %d %d %d\n",
           (unsigned char) mem[0],
           (unsigned char) mem[page_size],
           (unsigned char) mem[2 * page_size],
           (unsigned char) mem[3 * page_size]);

    if (munmap(mem, length) == -1) {
        perror("munmap");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
