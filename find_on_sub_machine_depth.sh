#!/usr/bin/env bash

##
## look in full_tests for machines with the desired sub-machine depth.
##

set -euo pipefail

usage() {
	echo "Usage: $0 <non-negative integer>" >&2
	exit 2
}

# must be exactly one argument
[[ $# -eq 1 ]] || usage

arg="$1"

# must be all digits
[[ $arg =~ ^[0-9]+$ ]] || usage

# parse to integer
#n=$((10#$arg))

# skip directories having intentionally defective FSMLang files
find test/full_test* \
  \( -path 'test/full_test43' -o \
     -path 'test/full_test12' -o \
     -path 'test/full_test44' -o \
     -path 'test/full_test42' -o \
     -path 'test/full_test40' -o \
     -path 'test/full_test39'    \
   \) -prune -o \
  -type f -name '*.fsm' -exec $FSM -s --find-on-sub-machine-depth=$arg '{}' \;

