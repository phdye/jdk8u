#!/bin/bash
# Level 12: Integration Tests
# Run with: ./run_tests.sh [path-to-java]

JAVA="${1:-java}"

echo "=== Level 12: Full Integration ==="

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

# Run all tests
FAILED=0
for f in Test_*.class; do
    name="${f%.class}"
    # Skip inner classes
    if [[ "$name" == *'$'* ]]; then
        continue
    fi
    "$JAVA" -ea "$name" 2>&1
    if [ $? -ne 0 ]; then
        echo "[FAIL] $name"
        FAILED=1
    fi
done

if [ $FAILED -eq 0 ]; then
    echo "=== Level 12: All tests PASSED ==="
else
    echo "=== Level 12: Some tests FAILED ==="
    exit 1
fi
