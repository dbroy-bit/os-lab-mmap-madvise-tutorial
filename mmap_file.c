#define _DEFAULT_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    const char *file_name = "mapped_demo.txt";
    const char *message = "Hello from mmap()!\n";

    long page_result = sysconf(_SC_PAGESIZE);
    if (page_result <= 0) {
        perror("sysconf(_SC_PAGESIZE)");
        return EXIT_FAILURE;
    }
    const size_t map_length = (size_t) page_result;

    int fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    if (ftruncate(fd, (off_t) map_length) == -1) {
        perror("ftruncate");
        close(fd);
        return EXIT_FAILURE;
    }

    char *region = mmap(NULL, map_length, PROT_READ | PROT_WRITE,
                        MAP_SHARED, fd, 0);
    if (region == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return EXIT_FAILURE;
    }

    int written = snprintf(region, map_length, "%s", message);
    if (written < 0 || (size_t) written >= map_length) {
        fprintf(stderr, "Message did not fit in the mapped region.\n");
        munmap(region, map_length);
        close(fd);
        return EXIT_FAILURE;
    }

    if (msync(region, map_length, MS_SYNC) == -1) {
        perror("msync");
        munmap(region, map_length);
        close(fd);
        return EXIT_FAILURE;
    }

    printf("Mapped memory says: %s", region);

    if (munmap(region, map_length) == -1) {
        perror("munmap");
        close(fd);
        return EXIT_FAILURE;
    }

    if (close(fd) == -1) {
        perror("close");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
