#!/usr/bin/env bash
# Manage build-capable git worktrees for parallel agents.
#
#   tools/worktree.sh add <name> [base-ref]   create ../sms-wt/<name> on branch wt/<name>,
#                                             wire it to the shared toolchain, build it and
#                                             take a baseline
#   tools/worktree.sh merge <name>            rebase wt/<name> onto the current branch and
#                                             fast-forward it in; verification is the caller's job
#   tools/worktree.sh remove <name>           remove the worktree and its branch
#   tools/worktree.sh list
#
# Everything under build/ and orig/ is untracked, so a bare `git worktree add`
# cannot build. Each worktree gets symlinks to the shared, read-only pieces
# (venv, compilers, tools, binutils, the disc files and map) and its own
# build/GMSE01 for objects, reports and baseline. configure.py bakes absolute
# paths into build.ninja, so it is re-run inside the worktree.

set -euo pipefail

ROOT=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
WT_ROOT=${SMS_WORKTREES:-$ROOT/../sms-wt}
VERSION=GMSE01

usage() { sed -n '2,12p' "$0"; exit 2; }

cmd=${1:-}; name=${2:-}
[ -n "$cmd" ] || usage

case "$cmd" in
add)
	[ -n "$name" ] || usage
	base=${3:-HEAD}
	path=$WT_ROOT/$name
	[ ! -e "$path" ] || { echo "$path already exists" >&2; exit 1; }
	mkdir -p "$WT_ROOT"
	git -C "$ROOT" worktree add -b "wt/$name" "$path" "$base"

	mkdir -p "$path/build" "$path/orig/$VERSION"
	for d in venv compilers tools binutils; do
		ln -s "$ROOT/build/$d" "$path/build/$d"
	done
	for d in files sys; do
		ln -s "$ROOT/orig/$VERSION/$d" "$path/orig/$VERSION/$d"
	done

	cd "$path"
	# Configure with the main tree's python by its real path: configure.py
	# bakes sys.executable into every command line, and ninja rebuilds any
	# output whose recorded command differs, so a worktree-relative python
	# path would make it re-download the shared tools through the symlinks.
	"$ROOT/build/venv/bin/python3" configure.py --version "$VERSION"
	# For the same reason the checkout's download_tool.py must not look newer
	# than the tools, and the worktree needs the main tree's build log.
	touch -r "$ROOT/tools/download_tool.py" "$path/tools/download_tool.py"
	[ -f "$ROOT/.ninja_log" ] && cp "$ROOT/.ninja_log" "$path/.ninja_log"
	if build/venv/bin/ninja -n build/tools/dtk build/compilers 2>&1 | grep -q TOOL; then
		echo "refusing to build: ninja wants to re-download the shared tools" >&2
		exit 1
	fi
	echo "== building $path (first build compiles every unit)"
	build/venv/bin/ninja
	build/venv/bin/ninja baseline
	echo
	echo "worktree ready: $path (branch wt/$name)"
	echo "agents run ninja / decomp-diff / validate-symbol-order from inside it as usual"
	;;
merge)
	[ -n "$name" ] || usage
	path=$WT_ROOT/$name
	target=$(git -C "$ROOT" rev-parse --abbrev-ref HEAD)
	if [ -n "$(git -C "$path" status --porcelain)" ]; then
		echo "wt/$name has uncommitted changes; commit them in $path first" >&2
		exit 1
	fi
	git -C "$path" rebase "$target"
	git -C "$ROOT" merge --ff-only "wt/$name"
	echo "merged wt/$name into $target; now run ninja changes_all and the DOL SHA-1 check here"
	;;
remove)
	[ -n "$name" ] || usage
	git -C "$ROOT" worktree remove --force "$WT_ROOT/$name"
	git -C "$ROOT" branch -D "wt/$name" || true
	;;
list)
	git -C "$ROOT" worktree list
	;;
*)
	usage
	;;
esac
