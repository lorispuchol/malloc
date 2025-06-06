# malloc

## Usage

bash
```
make
LD_PRELOAD=./libft_malloc.so ls
```

## Notes
n-N = 64
m-M = 512

`alignement` = 8 or 16 (16 is better)
`alignement` means the minimum allocated size for a malloc call: If a program claim 1 bytes (ex: one char), malloc will give `alignement` bytes because it must aligns memory block. (doing this will aligns blocks beetwen `char` and `int` for exemple)

- Do not allocate a memory block accross two zones or two pages if they have been claim with two call of mmap (do not merge also). Indeed mmap give adjacents pages if they have been claimed in one time 
- Make a page header
- Make a block header: size, next block, previous block (in used) (just size for blocks in use)

> Just one list ? list of freed blocks

- The tree sizes of allocation are managed separatly, These are zones. Tiny and small zones must be ables to accept minimum 100 allocation of its maximum size