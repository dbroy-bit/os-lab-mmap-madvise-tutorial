Memory Management Tutorial: mmap() and madvise()
================================================

FILES
-----
main.tex         Complete IIIT-Delhi Beamer source.
mmap_file.c     File-backed MAP_SHARED demonstration.
madvise_demo.c  Private anonymous mapping + MADV_DONTNEED demonstration.

OVERLEAF SETUP
--------------
1. Upload main.tex, mmap_file.c, and madvise_demo.c to your Overleaf project.
2. Keep your existing beamerthemeIIITD.sty and style1colorlarge.png in the
   same project folder (the theme may refer to its logo/image by filename).
3. Set Menu -> Compiler to XeLaTeX.
4. Recompile.

The source uses Times New Roman when the font is available and falls back to
TeXGyreTermes (a Times-compatible font). Code examples use a monospaced font.

LINUX SERVER SETUP
------------------
No sudo access is needed for the mmap() and madvise() demonstrations.
You need a C compiler (gcc or clang). strace and pmap are optional tools.

Compile and run:
  gcc -std=c11 -Wall -Wextra -O0 mmap_file.c -o mmap_file
  ./mmap_file
  head -c 19 mapped_demo.txt
  printf '\n'

  gcc -std=c11 -Wall -Wextra -O0 madvise_demo.c -o madvise_demo
  ./madvise_demo

Inspect mappings:
  getconf PAGESIZE
  cat /proc/$$/maps | less
  pmap -x $$                 # optional; requires procps

Trace calls:
  strace -e trace=mmap,madvise,munmap ./madvise_demo

NOTES
-----
- The C programs obtain the page size at runtime; they do not assume 4096 bytes.
- The MADV_DONTNEED output of zeroes is specific to the private anonymous
  mapping shown in madvise_demo.c on Linux after a successful madvise call.
- MADV_DONTNEED has different follow-up semantics for other mapping types.
- strace of a child process normally does not require sudo, but the server's
  ptrace/security policy may restrict it. Do not attempt to bypass that policy.
- The LaTeX source expects your original IIITD theme file and logo asset.
