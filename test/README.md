# Malloc Test Suite

This directory contains a comprehensive test suite for the custom malloc implementation. All tests have been thoroughly validated and pass successfully.

## Test Files

### 01_simple_test.c

**Basic functionality test**

- Tests basic malloc allocation across all zones (TINY, SMALL, LARGE)
- Validates malloc(0) behavior
- Tests basic free operations
- Verifies different allocation sizes work correctly

### 02_free_test.c

**Comprehensive free function validation**

- Tests basic free operations
- Validates free(NULL) safety
- Tests double free protection
- Verifies cross-zone free operations
- Tests invalid pointer handling

### 03_calloc_test.c

**Zero-initialized allocation testing**

- Tests calloc zero-initialization across all zones
- Validates overflow protection (SIZE_MAX scenarios)
- Tests calloc(0, size) and calloc(size, 0) edge cases
- Verifies proper memory zeroing

### 04_realloc_test.c

**Memory reallocation functionality**

- Tests realloc growth and shrinking
- Validates realloc(NULL, size) behavior (should act like malloc)
- Tests realloc(ptr, 0) behavior (should act like free)
- Verifies cross-zone reallocation (TINY ↔ SMALL ↔ LARGE)
- Tests data preservation during reallocation

### 05_show_alloc_test.c

**Memory visualization and tracking**

- Tests show_alloc_mem() function
- Demonstrates memory layout visualization by zones
- Tracks total allocated bytes
- Shows memory state changes during allocation/deallocation cycles

### 06_gcc_compat_test.c

**GCC and standard library compatibility**

- Tests integration with standard library functions (strcpy, strcmp)
- Validates array allocation and operations
- Tests struct allocation and member access
- Verifies mixed allocation/deallocation patterns
- Tests edge cases and memory alignment

### 07_coalescing_test.c

**Memory coalescing and fragmentation**

- Tests adjacent block coalescing
- Demonstrates fragmentation recovery
- Validates coalescing with different block sizes
- Tests boundary conditions between zones
- Shows memory cleanup effectiveness

### 08_stress_test.c

**Complex stress testing**

- Random allocation/deallocation stress test (1000+ allocations)
- Memory integrity validation under stress
- Realloc stress testing with data preservation
- Extreme fragmentation testing (1000 small blocks)
- Performance validation under high load

## Running Tests

### Compile and run individual tests:

```bash
# From the malloc project root directory
gcc test/01_simple_test.c -L. -lft_malloc_x86_64_Linux -o test/simple_test
LD_LIBRARY_PATH=. ./test/simple_test
```

### Compile and run all tests:

```bash
# From the malloc project root directory
for test in test/0*.c; do
    name=$(basename "$test" .c)
    echo "=== Compiling $name ==="
    gcc "$test" -L. -lft_malloc_x86_64_Linux -o "test/$name" || continue
    echo "=== Running $name ==="
    LD_LIBRARY_PATH=. "./test/$name"
    echo
done
```

## Test Results Summary

All tests have been validated and pass successfully:

- ✅ **01_simple_test**: Basic malloc functionality verified
- ✅ **02_free_test**: Comprehensive free validation with safety checks
- ✅ **03_calloc_test**: Zero-initialization and overflow protection working
- ✅ **04_realloc_test**: Cross-zone reallocation with data preservation
- ✅ **05_show_alloc_test**: Memory visualization and tracking working
- ✅ **06_gcc_compat_test**: Standard library integration perfect
- ✅ **07_coalescing_test**: Block merging and fragmentation recovery working
- ✅ **08_stress_test**: High-load scenarios with perfect integrity

## Implementation Features Validated

- **Memory Zones**: TINY (≤128 bytes), SMALL (≤1024 bytes), LARGE (>1024 bytes)
- **Memory Safety**: Protection against double free, invalid pointers
- **Data Integrity**: All allocated memory maintains correct values
- **Memory Coalescing**: Adjacent free blocks are properly merged
- **Standard Compatibility**: Works seamlessly with standard library functions
- **Performance**: Handles high-load scenarios efficiently
- **Cleanup**: Complete memory recovery after operations

The malloc implementation is production-ready and passes all comprehensive test scenarios.
