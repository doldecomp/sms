#!/bin/bash
# usage: chk.sh file...
cd "$(dirname "$0")/../../../.."
for f in "$@"; do echo "== $f"; g++ -x c++ -std=gnu++03 -fsyntax-only -w -fpermissive -I include -I build/GMSE01/include -DBUILD_VERSION=2 -DVERSION_GMSE01 -DNDEBUG=1 -DGEKKO -fno-gnu-keywords -include docs/progress/port-scope/scripts/compat.h -m64 $f 2>&1 | grep -E "error" | head -8; done
