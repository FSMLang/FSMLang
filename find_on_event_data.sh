#!/usr/bin/env bash

##
## look in full_tests for machines with events having data.
##

set -euo pipefail

usage() {
	echo "Usage: $0 <non-negative integer>" >&2
	exit 2
}

# must be exactly no arguments
[[ $# -eq 0 ]] || usage

# skip directories having intentionally defective FSMLang files
find test/full_test* \
  \( -path 'test/full_test43' -o \
     -path 'test/full_test12' -o \
     -path 'test/full_test44' -o \
     -path 'test/full_test42' -o \
     -path 'test/full_test40' -o \
     -path 'test/full_test39'    \
   \) -prune -o \
  -type f -name '*.fsm' -exec $FSM -s -M --find-on-event-data '{}' \;

