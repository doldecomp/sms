# North American revision 0

This configuration targets the user's `GMSE01` revision 0 disc.
It is a matching decompilation in progress, with an independently verified regional executable rebuild.
Source objects are enabled individually through `objects.json` after verification.
All other objects remain extracted binary objects in the final link.

GMSE01 links through an archive of the ordered object list, built by the pinned MWLD.
The original map records unreferenced duplicate sun-path globals in several game objects.
Archive resolution handles these duplicates while preserving the original section layout; linking those objects loose produces duplicate-definition errors.
This changes input packaging, not source-coverage accounting.
The archive build and final executable byte comparison are verified in [batch 37](../../docs/progress/GMSE01-closure-audit-batch37.md).

## Build

From the repository root, after installing the dependencies in the main README:

```sh
python3 configure.py --version GMSE01
build/tools/dtk disc extract -q '/path/to/GMSE01.iso' orig/GMSE01
ninja
ninja baseline
```

On this machine, Ninja is installed at `build/venv/bin/ninja`.
If `build/tools/dtk` has not yet been downloaded, run:

```sh
mkdir -p build/tools
python3 tools/download_tool.py dtk build/tools/dtk --tag v1.3.0
```

The configuration requires this extracted executable SHA-1:

```text
a6782903ef79d4196c8489ecb1b57decb5b3728f
```

The supplied ISO SHA-256 is:

```text
67cec1634e641227a4cd51e6a0b277730cb9a1adaa867530c9e66de45373e51d
```

The input remains at the repository root as `Super Mario Sunshine (2002)(Nintendo)(US).iso`.
Extracted assets are under `orig/GMSE01` and excluded from Git.

## Metadata provenance

The US disc has no linker map in its filesystem.
The initial symbols and splits were generated using `dtk map config` from the community-hosted [marioUS.map](https://github.com/BitPatty/Super-Mario-Sunshine-C-Kit/blob/6c778bb096c9df688ed7f9d7e077a57843f6c398/maps/marioUS.map).
The downloaded map SHA-256 is `6bdf9784761ff94a554df57dda21bb947e2457e277bfe334bd116e1eb60eb999`.
Its local copy is `orig/GMSE01/files/marioUS.MAP`.
The map is an external reference, not a file extracted from this disc.

The map's section addresses were checked against the executable.
Unit paths were mapped to this repository's existing source organization by unique basename, with explicit handling of `main.o` and `Enemy.a/enemyinterp.cpp`.
Further executable analysis was performed by the pinned decomp-toolkit v1.3.0.

### Corrections required for a byte-identical relink

- Section alignment comes from the US layout; Japanese addresses were not reused.
- The `.rodata` split stops at the executable's meaningful section extent, `0x803AB658`.
- The imported map overstates two English disc-error string sizes in Application: the local strings are 121 and 106 bytes, rather than 124 and 109.
  Subsequent Application strings are four bytes earlier; downstream `.rodata` starts eight bytes earlier after section alignment.
  Batch 35 corrects the affected symbol addresses and all downstream split boundaries together, restoring the final constant's full 64-byte extent at `0x803AB618`.
  See [the regional layout audit](../../docs/progress/GMSE01-closure-audit-batch35.md) for the verified ranges and string counts.
- First symbols in seven common BSS groups needed their pre-link sizes restored to account for CodeWarrior's common BSS inflation behavior.
  These were cross-checked against existing declarations and validated by relinking against the US executable.
- `fill_gaps` preserves gaps from the original executable.
  The initial configuration's 35 `retail_padding_*` labels were fragments caused by the incorrect downstream addresses.
  Batch 35 removes them after verifying that all 116 bytes belong to corrected objects; full executable comparison still passes.
- The MetroTRK exception assembly unit uses its existing repository path and `comment:0` metadata.

An all-extracted-object relink passed a full byte comparison and the expected SHA-1 after these corrections.
This is a prerequisite for source matching and does not mean that the decompilation is complete.

## Verification workflow

```sh
ninja baseline
# Make a bounded source change, rebuild, and inspect its instruction diff.
ninja
python3 tools/decomp-diff.py -u mario/System/ProcessMeter
NM=build/binutils/powerpc-eabi-nm python3 tools/validate-symbol-order.py \
  -u mario/System/ProcessMeter --map orig/GMSE01/files/marioUS.MAP
ninja changes_all
cmp orig/GMSE01/sys/main.dol build/GMSE01/mario.dol
```

The map path must be passed explicitly because the upstream validator defaults to the Japanese map.
The explicit `NM` path is required on Linux because the upstream validator defaults to an `.exe` name.
Keep the verified baseline reports and per-batch results locally.
Do not promote an object solely because an approximate instruction score rounds to 100%.
Data, relocations, weak-symbol selection, layout, and the complete executable hash also matter.
