#!/bin/bash
# Level 10: GC Tests
# Run with: ./run_tests.sh [path-to-java]

JAVA="${1:-java}"

echo "=== Level 10: Garbage Collection ==="

# Check if java is available
if ! command -v "$JAVA" &> /dev/null; then
    echo "[SKIP] java not found: $JAVA"
    exit 0
fi

cd "$(dirname "$0")"

# Compile all tests
echo "Compiling..."
for f in Test_*.java; do
    "$JAVA"c "$f" 2>&1
    if [ $? -ne 0 ]; then
        echo "[FAIL] Compilation failed: $f"
        exit 1
    fi
done

# Run all tests with limited heap for OOM test (skip inner classes with $)
FAILED=0
for f in Test_*.class; do
    [[ "$f" == *'$'* ]] && continue
    name="${f%.class}"
    if [ "$name" = "Test_OOMHandling" ]; then
        "$JAVA" -ea -Xmx32m "$name" 2>&1
    else
        "$JAVA" -ea -Xmx64m "$name" 2>&1
    fi
    if [ $? -ne 0 ]; then
        echo "[FAIL] $name"
        FAILED=1
    fi
done

if [ $FAILED -eq 0 ]; then
    echo "=== Level 10: All tests PASSED ==="
else
    echo "=== Level 10: Some tests FAILED ==="
    exit 1
fi
