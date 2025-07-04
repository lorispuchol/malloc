# malloc

## Usage

To ensure that the program call our malloc and not the libc malloc, we need to preload our lib to the program execution.

bash
```
make
LD_PRELOAD=./libft_malloc.so <program>
```

<program> can be `ls` for example


## Test printing memory map

We need a program which calls `show_alloc_mem()`, `show_alloc_mem()` is included in the library.  
So we need to link the library to your program program like this

```bash
make
gcc test/main.c -L . -l ft_malloc
LD_LIBRARY_PATH=. ./a.out
```

## draft: notes
n = 128
m = 1024

`alignement` = 8 or  16 (16 is better)
`alignement` means the minimum allocated size for a malloc call: If a program claim 1 bytes (ex: one char), malloc will give `alignement` bytes because it must aligns memory block. (doing this will aligns blocks beetwen `char` and `int` type for exemple)

- Do not allocate a memory block accross two zones or two pages if they have been claim with two call of mmap (do not merge also). Indeed mmap give adjacents pages if they have been claimed in one time 
- Make a page header
- Make a block header: size, next block, previous block (in used) (just size for blocks in use)

> Just one list ? list of freed blocks

- The tree sizes of allocation are managed separatly, These are zones. Tiny and small zones must be ables to accept minimum 100 allocation of its maximum size