#!/bin/bash
# Run all bottom-up tests for Cygwin-native JDK
# Usage: ./run_all.sh [level]
#   level: 1-7 for specific level, or omit for all

set -e

cd "$(dirname "$0")"

if [ -n "$1" ]; then
    # Run specific level
    make test-level0$1
else
    # Run all native tests
    make test
fi
