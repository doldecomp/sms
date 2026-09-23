#!/usr/bin/env bash
# Score hand-written variants of one unit's source file against retail.
#
#   tools/score-variant.sh <Unit> <symbol> <variant>...
#   tools/score-variant.sh Enemy/seal 'TSeal::init(TLiveManager*)' v1.cpp v2.cpp
#
# For each variant: copy it over src/<Unit>.cpp (or .c), compile only that
# object, and print one line with the match, the mismatch markers (~ | < >)
# and the frame. Read the markers, not the rounded percentage: "100.0%" can
# still hide slot differences. The original source is restored and its
# object rebuilt afterwards, even on Ctrl-C.
#
# Run from the worktree root. This is the cheap scorer for hand-written
# variants; tools/lever-search.py is a search tool, not a scorer. Variants
# are compiled one at a time on purpose: the machine has 4 cores shared by
# every agent.

set -uo pipefail

[ $# -ge 3 ] || { sed -n '2,15p' "$0"; exit 2; }
unit=$1; sym=$2; shift 2

src=
for ext in cpp c cp; do
	[ -f "src/$unit.$ext" ] && { src="src/$unit.$ext"; break; }
done
[ -n "$src" ] || { echo "no src/$unit.{cpp,c,cp} here (run from the worktree root)" >&2; exit 2; }
obj="build/GMSE01/src/$unit.o"

lock="build/.score-variant.lock"
exec 9>"$lock"
flock -n 9 || { echo "another score-variant run holds $lock in this worktree" >&2; exit 1; }

backup=$(mktemp)
cp "$src" "$backup"
restore() {
	cp "$backup" "$src"
	rm -f "$backup"
	build/venv/bin/ninja "$obj" >/dev/null 2>&1
}
trap restore EXIT

for v in "$@"; do
	name=$(basename "$v")
	if [ ! -f "$v" ]; then
		echo "$name: missing"
		continue
	fi
	cp "$v" "$src"
	if ! build/venv/bin/ninja "$obj" >/dev/null 2>&1; then
		echo "$name: BUILD FAILED"
		continue
	fi
	out=$(python3 tools/decomp-diff.py -u "mario/$unit" -d "$sym" --clusters 2>&1)
	match=$(printf '%s\n' "$out" | sed -n '1s/.*: \([0-9.]*% match\).*/\1/p')
	markers=$(printf '%s\n' "$out" | sed -n 's/^markers: \(.*\)   frame: .*/\1/p')
	frame=$(printf '%s\n' "$out" | sed -n 's/.*frame: target r1, \(-0x[0-9a-f]*\)(r1) \/ ours r1, \(-0x[0-9a-f]*\)(r1).*/target \1 ours \2/p')
	echo "$name: ${match:-?} | ${markers:-?} | frame ${frame:-?}"
done
