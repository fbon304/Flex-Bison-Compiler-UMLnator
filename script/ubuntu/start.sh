#! /bin/bash

set -euo pipefail

BASE_PATH="$(dirname "$0")/../.."
cd "$BASE_PATH"

INPUT="$1"
export TABLE_NAME=$(basename "$INPUT")
shift 1
cat "$INPUT" | build/Compiler "$@"
