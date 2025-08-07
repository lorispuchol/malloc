#!/bin/bash

# Script to run all malloc tests
echo "========================================"
echo "        MALLOC TEST SUITE RUNNER"
echo "========================================"

cd "$(dirname "$0")"

# List of test files
tests=(
    "01_simple_test"
    "02_free_test"
    "03_calloc_test"
    "04_realloc_test"
    "05_show_alloc_test"
    "06_gcc_compat_test"
    "07_coalescing_test"
    "08_stress_test"
)

passed=0
failed=0

for test in "${tests[@]}"; do
    echo
    echo "=== COMPILING $test ==="
    if gcc "test/${test}.c" -L. -lft_malloc_x86_64_Linux -o "test/$test"; then
        echo "✅ Compilation successful"
        
        echo "=== RUNNING $test ==="
        if LD_LIBRARY_PATH=. timeout 60s "./test/$test" > /dev/null 2>&1; then
            echo "✅ $test PASSED"
            ((passed++))
        else
            echo "❌ $test FAILED"
            ((failed++))
        fi
    else
        echo "❌ Compilation failed for $test"
        ((failed++))
    fi
done

echo
echo "========================================"
echo "           FINAL RESULTS"
echo "========================================"
echo "✅ Tests passed: $passed"
echo "❌ Tests failed: $failed"
echo "📊 Total tests: $((passed + failed))"

if [ $failed -eq 0 ]; then
    echo
    echo "🎉 ALL TESTS PASSED! 🎉"
    echo "Your malloc implementation is working perfectly!"
else
    echo
    echo "⚠️  Some tests failed. Check the implementation."
fi

echo "========================================"
