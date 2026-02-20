#!/bin/bash
# Level 9: JIT Tests
# Run with: ./run_tests.sh [path-to-java]

JAVA="${1:-java}"

echo "=== Level 9: JIT Compiler ==="

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

# Run all tests (skip inner classes with $)
FAILED=0
for f in Test_*.class; do
    [[ "$f" == *'$'* ]] && continue
    name="${f%.class}"
    "$JAVA" -ea "$name" 2>&1
    if [ $? -ne 0 ]; then
        echo "[FAIL] $name"
        FAILED=1
    fi
done

if [ $FAILED -eq 0 ]; then
    echo "=== Level 9: All tests PASSED ==="
else
    echo "=== Level 9: Some tests FAILED ==="
    exit 1
fi
