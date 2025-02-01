#!/bin/bash

lines=()

while IFS= read -r line; do
    lines+=("$line")
done

echo ""

for line in "${lines[@]}"; do
  ./build/tools/dtk.exe demangle "$line"  < /dev/null
done
