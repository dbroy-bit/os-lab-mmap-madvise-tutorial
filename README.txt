Memory Management Tutorial: mmap() and madvise()
================================================

FILES
-----
mmap_file.c     File-backed MAP_SHARED demonstration.
madvise_demo.c  Private anonymous mapping + MADV_DONTNEED demonstration.

LINUX SERVER SETUP
------------------
No sudo access is needed for the mmap() and madvise() demonstrations.
You need a C compiler (gcc or clang). strace and pmap are optional tools.

Compile and run:
```bash
gcc -std=c11 -Wall -Wextra -O0 mmap_file.c -o mmap_file
./mmap_file
head -c 19 mapped_demo.txt
printf '\n'

gcc -std=c11 -Wall -Wextra -O0 madvise_demo.c -o madvise_demo
./madvise_demo
```

Inspect mappings:
```bash
getconf PAGESIZE
cat /proc/$$/maps | less
pmap -x $$  # optional; requires procps
```

Trace calls:
```bash
strace -e trace=mmap,madvise,munmap ./madvise_demo
```

NOTES
-----
- The C programs obtain the page size at runtime; they do not assume 4096 bytes.
- The MADV_DONTNEED output of zeroes is specific to the private anonymous
  mapping shown in madvise_demo.c on Linux after a successful madvise call.
- MADV_DONTNEED has different follow-up semantics for other mapping types.
- strace of a child process normally does not require sudo, but the server's
  ptrace/security policy may restrict it. Do not attempt to bypass that policy.
