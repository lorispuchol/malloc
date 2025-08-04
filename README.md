# malloc

## Usage

To ensure that a program call our malloc and not the libc malloc, we need to preload our lib to the program execution.

bash
```
make
LD_PRELOAD=./libft_malloc.so <any program>  
```
`any program` can be `ls` or `gcc` for example


## Test printing memory map

We need a program which calls `show_alloc_mem()`, `show_alloc_mem()` is included in our library.  
So we need to link the library to a testing program like this

```bash
make
gcc test/main.c -L . -l ft_malloc
LD_LIBRARY_PATH=. ./a.out
```

## Notes
128 = Maximum size of a tiny block (subject: `n` bytes)  
1024 = Maximum size of a small block (subject: `m` bytes)

We need to manage 3 zones of memory allocation:
- Tiny zone: for allocations of size > 0 and <= 128 bytes
- Small zone: for allocations of size > 128 bytes and <= 1024 bytes
- Large zone: for allocations of size > 1024 bytes  

Tiny and small zones must be ables to accept minimum 100 blocks of its maximum size without calling `mmap()` again.
We need to preallocate enough pages to the tiny and small zones so that each can contain at least 100 blocks.
Thus, the zone size must be at least:

$$
100 \times \text{BLOCK\_MAX\_SIZE} + 100 \times \text{sizeof(block\_header)} + \text{sizeof(page\_header)}
$$

rounded up to the nearest page size multiple.

This formule gives us the `N` and `M` values mentioned in the subject. Which will be used as `lenght` parameter of `mmap()` calls for tiny and small zones.

> Page header is added to the begin of each set of pages allocated by `mmap()`,  
> `mmap()` return adjacent pages if the `length` parameter is bigger than the page size,
#

`alignement` = 8 or  16 (16 is better)  
`alignement` means the minimum allocated size for a malloc call: If a program claim 1 bytes (ex: one char), malloc will give `alignement` bytes because it must aligns memory block. (doing this will aligns blocks beetwen `char` and `int` type for exemple)

- Do not allocate a memory block accross two zones or two pages if they have been claim with two call of mmap (do not merge also). Indeed mmap give adjacents pages if they have been claimed in one time 
