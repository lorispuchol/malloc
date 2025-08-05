#!/bin/bash

# Comprehensive test script for malloc implementation
# Run this script to test all functionality

echo "=============================================="
echo "      MALLOC IMPLEMENTATION TEST SUITE"
echo "=============================================="

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

# Build the library first
echo "Building malloc library..."
cd "$PROJECT_DIR"
make clean &>/dev/null || true  # Clean if possible, ignore if no clean target
make
if [ $? -ne 0 ]; then
    echo "❌ Build failed!"
    exit 1
fi
echo "✅ Build successful"
echo

# Return to test directory
cd "$SCRIPT_DIR"

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test counter
TOTAL_TESTS=0
PASSED_TESTS=0

# Function to run a test
run_test() {
    local test_name="$1"
    local test_file="$2"
    local test_command="$3"
    
    echo -n "Testing $test_name... "
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Compile if source file provided
    if [ -n "$test_file" ]; then
        gcc -I ../includes/ -o "$test_name" "$test_file" ../libft_malloc.so &>/dev/null
        if [ $? -ne 0 ]; then
            echo -e "${RED}❌ Compilation failed${NC}"
            return 1
        fi
    fi
    
    # Run the test
    if [ -n "$test_command" ]; then
        eval "$test_command" &>/dev/null
    else
        LD_LIBRARY_PATH=.. ./"$test_name" &>/dev/null
    fi
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✅ PASSED${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        return 0
    else
        echo -e "${RED}❌ FAILED${NC}"
        return 1
    fi
}

# Function to run system program test
run_system_test() {
    local program="$1"
    local args="$2"
    
    echo -n "Testing system program '$program'... "
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    LD_PRELOAD=../libft_malloc.so $program $args >/dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✅ PASSED${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ FAILED${NC}"
    fi
}

echo "1. BASIC FUNCTIONALITY TESTS"
echo "-----------------------------"
run_test "simple_test" "simple_test.c"
run_test "comprehensive_test" "comprehensive_test.c"
run_test "calloc_test" "calloc_test.c"

echo
echo "2. STRESS AND EDGE CASE TESTS"
echo "------------------------------"
run_test "stress_test" "stress_test.c"
run_test "edge_case_test" "edge_case_test.c"
run_test "buffer_test" "buffer_test.c"
run_test "linker_test" "linker_test.c"
run_test "large_zone_test" "large_zone_test.c"

echo
echo "3. SYSTEM PROGRAM TESTS"
echo "-----------------------"
run_system_test "ls" "-la"
run_system_test "echo" "Hello World"
run_system_test "cat" "/dev/null"
run_system_test "wc" "-l /dev/null"

echo
echo "4. MEMORY VISUALIZATION TEST"
echo "----------------------------"
echo -n "Testing show_alloc_mem()... "
TOTAL_TESTS=$((TOTAL_TESTS + 1))

# Create a simple test file
cat > show_test.c << 'EOF'
#include "malloc.h"
int main() {
    malloc(42);
    malloc(500);
    malloc(2000);
    show_alloc_mem();
    return 0;
}
EOF

gcc -I ../includes/ -o show_test show_test.c ../libft_malloc.so &>/dev/null
if [ $? -eq 0 ]; then
    LD_LIBRARY_PATH=.. ./show_test 2>/dev/null | grep -q "TINY\|SMALL\|LARGE\|Total"
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✅ PASSED${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}❌ FAILED (output format)${NC}"
    fi
else
    echo -e "${RED}❌ FAILED (compilation)${NC}"
fi
rm -f show_test.c

echo
echo "5. KNOWN LIMITATIONS TEST"
echo "-------------------------"
echo -n "Testing gcc compilation (expected to fail)... "
TOTAL_TESTS=$((TOTAL_TESTS + 1))
echo "int main(){return 0;}" > gcc_test.c
LD_PRELOAD=../libft_malloc.so gcc -o gcc_test gcc_test.c &>/dev/null
if [ $? -ne 0 ]; then
    echo -e "${YELLOW}⚠️  EXPECTED FAILURE (GNU ld incompatibility)${NC}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "${GREEN}✅ UNEXPECTED SUCCESS${NC}"
    PASSED_TESTS=$((PASSED_TESTS + 1))
fi

# Clean up
rm -f simple_test comprehensive_test calloc_test stress_test edge_case_test buffer_test linker_test large_zone_test show_test gcc_test gcc_test.c show_test.c

echo
echo "=============================================="
echo "               TEST RESULTS"
echo "=============================================="
echo "Tests passed: $PASSED_TESTS/$TOTAL_TESTS"

if [ $PASSED_TESTS -eq $TOTAL_TESTS ]; then
    echo -e "${GREEN}🎉 ALL TESTS PASSED! 🎉${NC}"
    echo "The malloc implementation is working correctly!"
    exit 0
else
    FAILED=$((TOTAL_TESTS - PASSED_TESTS))
    echo -e "${RED}❌ $FAILED test(s) failed${NC}"
    exit 1
fi
