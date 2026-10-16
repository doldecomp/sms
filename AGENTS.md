# Agent Guide for Super Mario Sunshine (SMS) Decompilation

This document describes the structure, conventions and workflows of this repository to help AI agents contribute effectively. The project is a matching decompilation of **Super Mario Sunshine** (GameCube, MWCC 1.2.5 C++ compiler, PowerPC Gekko target).

Important preliminary note: always prefer putting sentences in `.md` files on different lines to reduce the diff shown in git when rewording just one sentence — the lines still get collapsed into a single paragraph as per `.md` formatting rules.

## Project Goal

Reconstruct the original C++ source code such that compiling it with the original Metrowerks CodeWarrior compiler produces **byte-identical** object files to the original game binary (`mario.dol`). This is called a **matching** decomp.

### Mindset

The work is fundamentally about getting into the heads of the original developers using only the evidence available — the binary, the linker map, debug strings, and the compiler's behavior. Every assumption must be doubted and verified. Be skeptical of everything: naming guesses, code structure, control flow choices, even whether a function "looks right." Only commit to a decision when the evidence is overwhelming.

When progress stalls because a tough choice must be made and strong evidence is lacking, **leave the code nonmatching and move on**. New evidence often emerges later — from matching a neighboring function, finding a pattern in another TU, or discovering a debug string. Premature commitment to a wrong approach creates technical debt that's harder to undo than a TODO comment. The final goal is still always a 100% match, but trying to headbutt a particular function into matching 100% usually results in fakematches (see below) and technical debt. Case-by-case judgement should be used.

Truly hard judgment calls — ambiguous code structure, naming disputes, architectural decisions — should be deferred to humans.
Do that by leaving TODOs around places that feel especially fake and wrong rather than refusing to tackle complicated tasks.
The ultimate goal of the project is not just a matching binary, but **human-readable, modifiable source code**.

## Repository Layout

Paths below use `GMSJ01`, the default version. Each supported version has the same layout under its own name.

```
config/GMSJ01/
  config.yml          — decomp-toolkit project config
  symbols.txt         — layout of the binary, addresses, sizes, names etc for every symbol present
  splits.txt          — per-TU section address ranges
  build.sha1          — SHA1 hash of the target DOL

src/                  — decompiled game source files
include/              — game headers (class declarations, inline functions)
libs/<name>/src/      — middleware source files (dolphin, JSystem, THPPlayer, PowerPC_EABI_Support, TRK_MINNOW_DOLPHIN, OdemuExi2)
libs/<name>/include/  — middleware headers
orig/GMSJ01/          — original game disc image (not committed)
build/GMSJ01/         — build artifacts, compiled objects
  obj/                — target (original) object files extracted by dtk
  src/                — recompiled object files from our source
build/compilers/      — various retro compilers, downloaded during build
  GC/1.2.5            — the assembler, compiler and linker used for most of SMS
build/binutils/       — UNIX-style binutils that can be used for GC
build/tools           — various tools: dtk, sjiswrap

tools/
  configure.py        — (imported by configure.py at root)
  decompctx.py        — generates decomp.me context files
  project.py          — build system library

configure.py          — main build configuration; lists ALL objects and their matching status
docs/                 — documentation on reverse-engineering methodology
```

**MOST IMPORTANT NOTE OF ALL IMPORTANT NOTES**: autonomous agents are **strictly prohibited** from working on:
- MSL runtime, MetroTRK and THPPlayer,
- dolphin SDK,
- JSystem middleware.
Under the supervision of a human programmer, some degree of work might be acceptable on a narrow subset of these libraries known to be leak-free, but when working without a human programmer's supervision work on these libraries is strictly prohibited and such PRs will not be accepted. Work on game code instead.

## Build & Diff Workflow

Note that on windows, all tool names should be suffixed with `.exe`, while the following documentation uses the UNIX spelling (without `.exe`) for simplicity.

Ninja is used for most workflows. Simply running `ninja` in the root of the repository will build the project and report the overall matching progress.

To check changes against a baseline, `ninja baseline` can be used to generate a baseline report on the current state of the work tree, after which `ninja changes_all` can be used to show how the current changes to work tree influence matching progress, including a per-symbol report.
**Important**: for regression testing, you **must** use `ninja baseline` and `ninja changes_all`.
When asked to work on a task, if a worktree is clean or only contains general scaffolding like classes and function stubs, you **must** use `baseline` before starting work and `changes_all` after finishing work to report on the results to the user.
If the worktree already has changes and you need a baseline of the committed tree, ask the human to stash and restore the changes.

**Never** use `git checkout`, `git restore`, `git stash`, `git reset` or `git checkout-index` to undo an experiment or to get a "before" state.
The worktree usually holds the human's uncommitted work and your earlier changes in the same files, and these commands destroy all of it.
To measure "before" and "after", copy the files to a scratch directory, make the change, measure, and copy the files back.
Or undo the change with an edit.

The project builds five versions of the game (see `README.md`): `GMSJ01`, `GMSP01`, `GMSE01`, `GMSJ01_01` and `GMSP01_NX`.
Before you commit a change, test **all** of them for regressions, not only JP.
Each version keeps its own baseline in `build/<version>/baseline.json`, so you can make all the baselines first and compare all of them after the change:

```sh
# before the change
for v in GMSJ01 GMSP01 GMSE01 GMSJ01_01 GMSP01_NX; do python configure.py -v $v && ninja baseline; done
# after the change
for v in GMSJ01 GMSP01 GMSE01 GMSJ01_01 GMSP01_NX; do python configure.py -v $v && ninja changes_all; done
python configure.py   # go back to the default version (GMSJ01)
```

Each version must also still report its DOL as `OK`.
A version whose disc image is not in `orig/<version>/` cannot be built. Tell the human which versions you could not test.
`objdiff.json` and `decomp-diff.py` follow the last `configure.py -v`, so reconfigure to the version that you want to diff.
Do not build old commits in separate trees.

When a sweep changes many sites (for example an accessor at every read of a field), report the number of functions that got better, the number that got worse, and the number that reached or lost 100%.
Read the full `changes_all` report, not only the function you work on: a frame gain in one function is not a match if another function gets worse.

The underlying utility used for splitting the binary is `dtk` (`build/tools/dtk`).
It is already properly set up and almost never requires any meddling.

The main diffing tool used by humans is **objdiff** (`encounter/objdiff`). It compares the compiled `.o` from our source against the original `.o` extracted from the DOL, function by function, showing PPC assembly side-by-side in a convenient GUI.

**Important**: The human user typically has objdiff open in the background.
It watches source files and **automatically recompiles** whenever a file changes.
This means after editing a source or header file, you do **not** need to manually `touch` files or force rebuilds — just run `ninja` and if it says "no work to do", that's fine; objdiff has already compiled the latest code.
Never waste time trying to force ninja to rebuild.

### Using `decomp-diff.py` (agent-friendly CLI diffing)

Since agents cannot use the objdiff GUI, use the wrapper script `tools/decomp-diff.py` instead. It calls `objdiff-cli` and produces readable text output.

Unit names follow the pattern in `objdiff.json` — typically `mario/<path>` (e.g. `mario/Enemy/fireWanwan`, `mario/System/MarDirector`). You can grep `objdiff.json` for a source filename to find its unit name.

#### Overview mode (list symbols with match status)

```
python tools/decomp-diff.py -u mario/Enemy/fireWanwan
```

Output columns: STATUS (match/nonmatching/missing/extra), MATCH %, SIZE, SECTION, demangled NAME.

Useful filters (can be combined):
- `-s nonmatching` — only nonmatching symbols
- `-s match` — only matching symbols
- `-s missing` — symbols in target but not in our build
- `-s extra` — symbols in our build but not in target
- `-t function` — only functions (skip data/bss)
- `-t object` — only data objects
- `--section .text` — only `.text` section
- `--search "init"` — fuzzy substring search on demangled name

Example — list all nonmatching functions:
```
python tools/decomp-diff.py -u mario/Enemy/fireWanwan -s nonmatching -t function --section .text
```

#### Diff mode (side-by-side instruction comparison)

```
python tools/decomp-diff.py -u mario/Enemy/fireWanwan -d "TFireWanwan::moveObject"
```

The `-d` argument takes a substring match on the demangled or mangled symbol name.

Output shows target (original) and our build PPC assembly **interleaved** in a single column, with diff markers in the leftmost position:
- ` ` (space) — instructions match perfectly
- `~` — same opcode, but one or more operands differ (shown in `{braces}`)
- `|` — different opcode at corresponding position
- `>` — instruction exists only in our build (inserted/extra)
- `<` — instruction exists only in the target (deleted/missing)

Each line shows **one** instruction. When the marker is ` ` or `~`, both target and our build have a corresponding instruction at that position. When the marker is `|`, the target instruction is shown but our build emits a different opcode there. `<` and `>` lines indicate instructions present in only one side — the shown instruction is from whichever side has it.

Differing operands are wrapped in `{braces}` for easy identification.

By default, long runs of fully matching instructions are **collapsed** into a summary line like `... 10 matching instructions ...`.
This is only a display shortcut: those instructions still exist and still match; they are just hidden to keep the diff compact.

With `--no-collapse`, those summary lines disappear and every instruction in matching regions is printed explicitly.
This is useful when mapping source lines to exact instruction neighborhoods or when checking whether a mismatch cluster is truly contiguous.

Quick rule of thumb for agents:
- default (collapsed): better for scanning large functions and finding mismatch hotspots quickly.
- `--no-collapse`: better for deep analysis when you need complete sequential instruction flow.

Options:
- `-C 5` — show 5 context lines instead of 3
- `--no-collapse` — disable match-run collapsing and show every instruction
- `--range 100-300` — only show instructions at hex offsets 0x100–0x300

### Using `validate-symbol-order.py` (symbols vs the map)

`tools/validate-symbol-order.py` checks a single TU's **function symbols**
against the original linker map (`mario.MAP`) — the things `objdiff` can't see,
because they concern deadstripped (UNUSED) symbols, emission order, and linkage
rather than instruction bytes. It reads our compiled object with `nm`, so
**build the TU first**, then compares it to the map's `.text section layout`
and symbol-closure section.

```
python tools/validate-symbol-order.py -u mario/MarioUtil/MathUtil
```

Unit names are the same `mario/<path>` names as `decomp-diff.py`. Both
versions use the `mario/` prefix; the tool compares the object of the version
`objdiff.json` is configured for (the last `configure.py -v`) with the map that
`config/<version>/config.yml` names, and prints which map it used. Give
`--map` only to compare with a different map on purpose.

What it reports:
- **MISSING** (error) — a map symbol (used or UNUSED) that our object doesn't
  define. Just define the function.
- **ORDER** (error) — non-weak symbols present but in the wrong relative order;
  reorder the definitions to match the map. (Emission order is the reverse of
  source order for `-inline deferred` TUs.) A disorder involving **only weak**
  symbols is downgraded to a *warning*, since weak ordering is compiler-driven
  and painful to steer.
- **BINDING** (error) — a *linked* symbol whose weak/local/global linkage
  disagrees with the map's closure section (e.g. an out-of-line global that
  should be a weak header inline, or a dropped `static`).
- **SIZE** (warning) — an UNUSED symbol whose byte size differs from the map.
  Matching an inlined body's exact size is hard, so this never fails.

Exit codes: `0` pass, `1` a real failure, `2` couldn't run (unit / object /
map not found).

**Caveat — UNUSED binding is not checked.** Dead-stripping removes weak, local
and global symbols alike and records no binding in the map, so an UNUSED
symbol's linkage is *unknowable from the map*: an UNUSED symbol **can** be weak.
The map's `UNUSED` marker says nothing about linkage, so the tool only validates
binding for linked symbols.

This runs in CI: `tools/check-changed-symbol-order.py` maps every changed
`.cpp` in a PR/push to its unit and runs the check, gating the "symbol map"
items in the Pre-PR checklist below.

### Always prefer using `m2c` for from-scratch decompilation

Always insist for the user to provide a path to [`m2c`](https://github.com/matt-kempster/m2c) when working on decompiling new functions (as opposed to matching ones that are already very close), it produces rough C-style draft decompilation of a function or an entire translation unit.
Cleaning up that draft to use proper fields and inline helpers available on relevant types usually already gets the function very close to matching.
The tool accepts plaintext assembly, which for this repository are the dtk-generated assembly files under `build/GMSJ01/asm/<path>.s`.

Usually, m2c is available at `../m2c/m2c.py` relative to the project's root.

Tested example:

```bash
python D:/Develop/m2c/m2c.py -t ppc -f __dt__22TNerveFireWanwanEscapeFv --globals=used build/GMSJ01/asm/Enemy/fireWanwan.s
```

That command decompiles the function named by the `.fn` symbol in the asm file and prints a raw draft to stdout.
For larger functions, the same pattern works with the mangled symbol name, for example:

```bash
python D:/Develop/m2c/m2c.py -t ppc -f "execute__22TNerveFireWanwanEscapeCFP24TSpineBase<10TLiveActor>" --globals=used build/GMSJ01/asm/Enemy/fireWanwan.s
```

Practical workflow:
- find the TU assembly file in `build/GMSJ01/asm/...`
- if the task requires decompiling only one function, open it and copy the mangled function name from the `.fn` line
- run `m2c` with `-t ppc` (`ppc` is the right alias for this CodeWarrior PowerPC output)
- use `-f <mangled name>` when decompiling only one function
- use `--globals=used` by default so that values of used globals are shown
- save the output in a temporary folder for later inspection

Important limitations:
- `m2c` output is often rough for this codebase: inferred placeholder structs, bad field names, missing types, and occasional broken expressions are normal.
- `m2c` does **not** support the C++ context flow needed for this repository's real types, so do not try to feed it SMS C++ context files and expect good typed output.
- use `m2c` for orientation only; never trust it over the asm or decomp-diff.

### Read the compiler instead of guessing: the MWCC decompilation

A 100% matching decompilation of `mwcceppc.exe` exists: [`rayanht/mwcc`](https://github.com/rayanht/mwcc).
It covers GC 1.0, 1.1, 1.1p1, 1.2.5, 1.2.5n and 1.3, so it includes the exact compiler that SMS uses (`GC/1.2.5`, and `GC/1.2.5n` for a few TUs).
Usually it is at `../mwcc` relative to the project root.

Some problems do not yield to guesswork, for example:

- a register swap or rotation when every instruction already matches,
- a frame that is a few slots too big or too small,
- an inline that the ROM expands at one site and calls at another,
- a CSE, a copy or a load that MWCC keeps or removes for no visible reason,
- a symbol in the wrong section, or a weak symbol in the wrong TU.

For such problems, reading what the compiler does is much faster than trying source spellings one by one.
When you meet such a problem and the decompilation is not available, **stop and ask the human to clone it**, and tell them the path where you expect it.
Do not continue to guess spellings for hours when you could read the answer.

Where to look:

| Topic | Files |
|---|---|
| Parser, expressions, inlining, templates, mangling | `src/frontend/` (`CInline.c`, `CExpr.c`, `CDecl.c`, `CTemplate*.c`, `CMangler.c`) |
| IR optimizer: copy propagation, CSE, struct scalarization | `src/optimizer/Iro*.c` |
| Code generation, register colouring, scheduling, stack frame, peephole | `src/backend/` (`CodeGen.c`, `Coloring.c`, `InterferenceGraph.c`, `StackFrameEABI.c`, `Peephole.c`) |

Use the decompilation to **explain** a codegen difference, and then find a source spelling that a human would write.
An explanation of the compiler is not permission for a fakematch: the rules in [Fakematches](#fakematches-important) still apply.
Check each conclusion with a real build of SMS. [docs/AGENT_MATCHING_TIPS.md](docs/AGENT_MATCHING_TIPS.md#use-the-compiler-sources-and-the-mwcc-debugger) has the facts that were already read from it, and the `mwcc-izer` debugger that dumps the compiler's internal state for one function.

### Decompiling brand-new code without existing scaffolding

When a user requests to decompile a fresh TU for which no stubs or header exists, `docs/PROGRAM_STRUCTURE_REVVING.md` **must** be followed to reconstruct the scaffolding like declarations/definitions of methods/functions, class definitions, global variables, etc.

**Important**: as soon as you notice some method declaration and/or definition missing, thats a sure-sign of someone forgetting to scan the entire `mario.MAP` file properly for all known methods of some class and/or all known symbols in a TU.
Do the rescan yourself for relevant classes and TUs using relevant regular expressions containing file name or the correct mangling suffix that all such class members have.

## Evidence

Every name, every helper and every body in the source needs evidence.
A guess that compiles to the right bytes is not evidence: many different sources compile to the same bytes.

### What counts as evidence

In order of strength:

1. A name in `mario.MAP`: a function, a data symbol, or a class name inside a mangled signature.
2. A debug string in the binary, for example the field names that `PARAM_INIT` stringifies, or an assert condition.
3. A setter that stores the field, especially one that also configures something else from the same value.
4. An allocation size: `count * sizeof(T)` names the array that it fills.
5. The parameter name of the function that the value goes to.
6. The size of an UNUSED map symbol.

A name in **our own** source is not evidence.
A local variable name, a placeholder or a field name that someone wrote earlier (often an agent in the same session) is a guess.
Do not read it back as proof.

### The maps

- `orig/GMSJ01/files/mario.MAP` is the main source of truth.
- `config/GMSJ01/symbols.txt` lists only the symbols that survived in the DOL.
  A function that is missing there can still be an UNUSED map entry with a size.
  Before you say that a function is inline, stripped or missing, grep `mario.MAP` for its mangled name.
- The symbol-closure part of `mario.MAP` (the tree before `.init section layout`) has a `>>>` line for each other TU that emitted the same weak symbol.
  The closure, not the section layout, shows which TUs emitted an inline.
  A weak symbol that the map lists but our build does not emit shows a site where MWCC did not inline, in the ROM.
- `orig/GMSJ01/files/marioL.MAP` is the map of a **different** link that keeps the `JAW*` debug windows.
  About 287 functions that `mario.MAP` calls UNUSED are linked there.
  In its closure tree, the entries indented one level below a function are the symbols that the function references: its call list.
  Read it before you write any UNUSED body.
- Other versions have their own maps, for example `orig/GMSP01/files/marioEU.MAP` (PAL) and `orig/GMSJ01_01/files/mario.MAP` (JP Rev 1). They can name functions that the JP map does not.
- A function that another version's map names but the JP map does not is usually the same source, dead-stripped from the JP link.
  Write the body without `#ifdef` in the shared `.cpp`. The JP DOL stays identical, because the linker removes it again.
  Confirm this with a build of every version.

### Other games

Other decomp projects share JSystem and some game-layer code.
Each one uses a **different MWCC version**, so take only source-level facts from them: names, field order, control flow, whether a function or an assert existed.
Never use their frame sizes, instruction counts or register assignments as evidence for our build.

| Project | Symbol evidence |
|---|---|
| The Wind Waker (TWW) | A **debug** map with demangled names. It lists every inline that each TU called. Asserts are on, so its string pools keep assert conditions of dead-stripped code. |
| Twilight Princess (TP) | Retail and debug builds. Good for JSystem names and asserts. |
| Mario Kart: Double Dash (MKDD) | A review build that ships a linker map with mangled names only. It is **not** a debug build. |
| Animal Crossing | Shipped linker maps (`static.map`, `foresta.map`) with function and file names. Its decomp has the older C version of the JAudio library (`jaudio_NES`), a close relative of SMS's JASystem. Its function bodies are decompiled, and their names are evidence only where the AC maps confirm them. |
| Pikmin 2 | Some `JAInter` field names. |
| Luigi's Mansion | **No** map. Its names are the decompilers' own. Never cite it. |

A decomp project's `asm` and `symbols.txt` files are outputs of naming decisions, not evidence.
Trace a borrowed name back to a shipped map or to strings in that game's binary.
Check every borrowed name against a use in SMS: TWW is sometimes wrong for the SMS revision.
SMS's own map wins over any other game on names and on inline-or-not.

## Source Organization

Each `.o` file maps 1:1 to a `.cpp` file. The path is listed in `configure.py` under `config.libs`.
For a middleware object, the first path component is the library: the object `JSystem/JKernel/JKRHeap.cpp` has its source in `libs/JSystem/src/JKernel/JKRHeap.cpp`.
Each object has a status:

- `Matching` — our code compiles to byte-identical output. Linked into the final DOL.
- `NonMatching` — work in progress. Not linked.
- `Equivalent` — functionally equivalent but not byte-identical. Only linked with `--non-matching`.

### Directory structure mirrors game libraries

| Directory | Description |
|-----------|-------------|
| `libs/JSystem/` | Nintendo's JSystem middleware (J3D, JParticle, JDrama, JAudio, etc.) |
| `libs/dolphin/` | Dolphin SDK (OS, GX, DVD, PAD, etc.) |
| `libs/PowerPC_EABI_Support/` | Metrowerks runtime & MSL |
| `libs/THPPlayer/` | THP movie player |
| `libs/TRK_MINNOW_DOLPHIN/` | MetroTRK debugger |
| `libs/OdemuExi2/` | Debugger EXI driver |
| `src/System/` | Game system framework (directors, params, events) |
| `src/Strategic/` | Core game object hierarchy (actors, hit detection, spine/nerve AI) |
| `src/Player/` | Mario player code |
| `src/Enemy/` | All enemy implementations |
| `src/Camera/` | Camera system |
| `src/Map/` | Map geometry, collision, pollution |
| `src/MoveBG/` | Interactive map objects |
| `src/NPC/` | NPC behavior |
| `src/MSound/` | Sound system wrappers |
| `src/M3DUtil/` | 3D model utility layer (MActor) |
| `src/MarioUtil/` | Math, drawing, packet, rumble utilities |
| `src/GC2D/` | 2D UI (console, menus, HUD) |
| `src/Animal/` | Animal AI (birds, butterflies, fish) |

## Compiler & Flags

- **Compiler**: Metrowerks CodeWarrior for GameCube, version `GC/1.2.5` (some SDK files use `GC/1.2.5n`)
- **Language standard**: C++98 (Metrowerks dialect). No C++11. `nullptr` is `#define nullptr 0`.
- **Key game code flags**: `-O4,p -fp_contract on -str reuse,readonly -opt all,nostrength -inline deferred`
- **RTTI off**, **C++ exceptions off**

### Compiler behavior to be aware of

- MWCC aggressively inlines small functions, especially those defined in headers or marked inline.
- The compiler preserves symbol order strictly — function/variable definition order in the source must match the order in the binary.
- Dead code elimination is limited — unused `static` variables and functions may still be emitted.
- BSS ordering is affected by include order — "rogue includes" are sometimes needed just to match BSS/SBSS layout. These are marked with comments like `// rogue includes needed for matching sinit & bss`.

Before starting any deocmpilation or matching tasks, you **must** read **[docs/AGENT_MATCHING_TIPS.md](docs/AGENT_MATCHING_TIPS.md)**.
Expand this document as you notice new patterns, but always ask for human review afterwards — some patterns might be red herrings.

## Key Data Files

### `config/GMSJ01/symbols.txt`

Every symbol present in the binary with address, size, type, scope and alignment. Format:
```
mangled_name = .section:0xADDRESS; // type:function size:0xSIZE scope:global align:4
```

Scope is `global`, `local` (static), or `weak` (inline/header-defined). Mangled names use MWCC's mangling scheme (similar to but not identical to Itanium ABI). The `bulk_demangle.sh` script or objdiff can demangle them.

### `config/GMSJ01/splits.txt`

Maps each source file to its section address ranges, telling dtk how to split the DOL into per-TU objects.

### `orig/GMSJ01/files/mario.MAP`

The original linker map file, included on the game disc. This is a **priority source of truth** — it contains every symbol the linker saw, including UNUSED (inlined/dead) functions with their mangled names and compiled sizes. When there is any ambiguity about symbol names, signatures, sizes, or ordering, defer to this file first before other sources. It is very large (~102k lines) so grep it rather than reading it in full.

### `build/GMSJ01/asm/<path>.s`

Disassembled `.s` files generated by dtk for every TU (e.g. `build/GMSJ01/asm/Enemy/fireWanwan.s`). These contain the full assembly including data sections (`.rodata`, `.data`, `.sdata2`, etc.) with literal values — strings, floats, jump tables, and vtables. Useful for recovering string constants, float literals, and other data symbol contents that aren't visible in the symbol map.

## Class Hierarchy & Architecture

The game uses a deep OOP hierarchy typical of GameCube-era Nintendo games:

```
JDrama::TNameRef
  └─ JDrama::TActor             (position, rotation, scaling)
       └─ THitActor             (collision shapes, hit flags)
            └─ TTakeActor       (hold/take mechanics)
                 └─ TLiveActor  (model, spine AI, physics, velocity)
                      └─ TSpineEnemy  (graph pathing, HP, body scale)
                           └─ TSmallEnemy   (water reactions, BCK anims, juice blocks)
                                └─ TFireWanwan, etc.
```

### Spine/Nerve System

Enemy AI uses a **spine/nerve** state machine:
- `TSpineBase<TLiveActor>` manages a stack of nerve states.
- Each nerve is a singleton class inheriting `TNerveBase<TLiveActor>` with an `execute()` method.
- Declared with `DECLARE_NERVE(Name, TLiveActor)` in headers.
- Defined with `DEFINE_NERVE(Name, TLiveActor) { ... }` in source files — this macro expands to both a `theNerve()` singleton accessor and the `execute()` body.

### Parameter System

Game-tunable parameters use `TParamRT<T>` fields initialized via the `PARAM_INIT(member, default)` macro in constructor initializer lists. Parameters are loaded from `.prm` files at runtime. Access a parameter's value with `.get()`.

## Code Conventions

### Naming

- **Use original names wherever possible.** The symbol map (`mario.MAP`, `symbols.txt`) and debug strings embedded in the binary are the primary sources for names. For example, `PARAM_INIT` stringifies its field name argument, so all parameter struct members have their original names recoverable from the binary's `.rodata` section. Another source of debug names can be debug builds of other games that share libraries with this one, but this is left for humans to figure out.
- Try to follow existing conventions of the subsystem whenever possible (e.g. conventions in MSL, dolphin SDK, JSystem and gameplay code are all slightly different).
- Function arguments: `snake_case` (e.g., `live_manager`, `hit_actor`). Or palceholders where unsure: `param_1`, `param_2`, etc.
- Local variables: `camelCase` (e.g., `distToMario`, `nextNode`). Or placeholders where unsure what name to chose: `fVar1`, `uVar2`, etc. If a function is hard to match, it is extremely useful to name variables as `r31`/`f31` etc according to the register it is stored in (or `r31_2` if same register is used for multiple variables), as well as `local_2B4` etc for variables that are stored on the stack. The former should almost always be used for matching code with vector and matrix math, as it is extremely helpful in mapping the C++ code to the assembly in your head without losing track of things. If a vector is split between multiple float registers, names like `f31f30f29` should be used.
- Classes are prefixed with `T` (e.g., `TFireWanwan`, `TSmallEnemy`).
- Member variables use camelCase, most of the time prefixed with `m` (e.g., `mHitPoints`, `mPosition`).
- Unknown members retain placeholder names like `unk194`, `unk238`, with hex offsets.
- Method names use camelCase: `moveObject()`, `calcRootMatrix()`, `behaveToWater()`.

### Comments & annotations in the code

- `// fabricated` — the code is not from the original binary; it was invented/guessed to make things compile. May be incorrect.
- `// TODO:` — known issues, suspected inaccuracies, or incomplete understanding, for example a guessed name.
- `// correct but X is incorrect` — the function itself matches, but a called function does not.
- `// rogue includes needed for matching sinit & bss` — includes added purely for BSS/static-init ordering.
- `#pragma dont_inline on/off` — forces the compiler to not inline a function. This always hides a real cause (see the inlining section of the tips document).
- `/* 0x1EC */` — the offset comment of a field. It is part of the layout record.

Keep comments to these short markers.
Do not write explanatory prose in source files: no paragraphs above a declaration, no notes after a field, no text that summarizes a function.
A header is a layout record: write the offset comment and the declaration.
Put the reasoning and the evidence in the PR description or in the commit message.
Write prose in source only when a human asks for documentation.

Do not write a `// TODO` about a frame deficit or about the size of an UNUSED body.
Such notes go out of date at once, and `decomp-diff.py` shows the difference anyway.
Write "frame-only" in the PR description.

Keep existing `TODO` comments unless your change resolves them, and delete a `TODO` that is no longer true.

### Style

- Formatting follows `.clang-format` (WebKit-based, 80-column, tabs for indent).
- C++98 style: no auto, no range-for, no lambdas, no `nullptr` keyword (it's a macro).
- `#include` paths use angle brackets with project-relative paths: `#include <Enemy/FireWanwan.hpp>`.
- Header guards use `#ifndef FOLDER_FILENAME_HPP` / `#define` / `#endif` pattern.
- When the bodies of a class template must leave the header, put them in `<Header>.tpp` (never `<Header>Impl.hpp`), with a `_TPP` include guard.
  The `.tpp` includes its own `.hpp` first.
  Include the `.tpp` only from the TUs that the map shows as owners of the template's weak symbols.
- Cast between a pointer and an integer with `uintptr_t`, never with `u32`, `s32` or `int`.
  On PowerPC this changes no byte, and it keeps a 64-bit port possible.
  Do not change casts that do not involve pointers.
- Port the asserts that a reference project supports, as `JUT_ASSERT(<cond>);` with no line number.
  A disabled assert still changes codegen.
- Write a release-stripped warning with the existing `JUT_REPORT_MSG(...)` or `JUT_ASSERT(...)` macros, never with an empty `if (cond) { }`.
- When working with dolphin's mtx.h library, always prefer routing calls to PSMTX* or PSVEC* functions through the non-PS-prefixed macros MTX* and VEC*.
  This is so that when a debug build of the game is eventually built, we still can easily switch between paired-singles PowerPC assembly implementation or a regular C implementation (C_MTX* and C_VEC*).
  However, sometimes the game code directly calls C_ prefixed versions of the functions, in which case we should too.
- Always keep track of available enums and inlines:
  * Sound effect IDs should always go through the enum in SoundEffects.hpp
  * Live actor flags should use the LIVE_FLAG_* enum and onLiveFlag/checkLiveFlag/offLiveFlag
  * Hit actor flags should use HIT_FLAG_* and onHitFlag/checkHitFlag/offHitFlag
  * Etc. Many enums in the codebase are already named, look for prior usages of relevant fields and stylize things analogously.
- When encountering a new enum-like thing (e.g. states or flags  of some class), make an enum for it.
  Name the enumerands as `THING_FLAG_UNK10` if it's not immediately clear what the different states/flags represent -- sometimes running the game in an emulator and inspecting the memory state is required to truly understand what the different states or flags represent, but having them named already makes working with the code simpler.

### Fakematches (**IMPORTANT**)

A **fakematch** is code that compiles to byte-identical output but is clearly not what the original developers wrote. Examples include nonsensical casts, unnecessary temporaries, bizarre control flow, or abusing compiler quirks to force specific code generation. Fakematches should be **avoided where possible** — the goal is to reconstruct code that a real Japanese developer at Nintendo would have plausibly written in ~2002 using C++98. If a match can only be achieved through obviously artificial code, it's better to leave the function as nonmatching with a TODO comment than to commit a fakematch.

An agent is prohibited from committing `volatile char trash[0x10];` family of tricks to the repositiory to force-fix function stack frame size issues.
Never the less, their temporary use is encouraged to **validate** that the function matches modulo stack frame issues.
It must however be noted that such tricks don't always resolve stack frame issues -- it is not rare for proper inlines at correct points in the function to be required for a perfect match.

If only stack offsets still differ, stop. Leave the function nonmatching and write "frame-only" in the PR description. Don't commit locals, casts or temporaries whose only job is the frame.

#### Frame deficits are not evidence

A frame deficit can be closed in many ways.
Two `f32` pads, two `s32` pads, one `f64`, four `u16`s or two pointers all give the same 100%.
An invented accessor almost always gives the missing slots too.
So a frame match alone says nothing about what the original wrote.

These are fakematches, and reviewers reject them on sight:

- an unused local that only adds a slot,
- a named or reference local inside a one-line accessor, only to add a temp:
  `u8 getGlobalAlpha() const { const GXColor& c = mGlobalPrmColor; return c.a; }`,
- a getter for a class's **own** field, called inside that class's own methods.
  A method of the class reads its own fields raw.
  An accessor on **another** object is normal.
- an accessor that the frame size alone chose.
  If a deficit could equally come from an accessor for `unk61`, `unk60` or `unk4C`, the frame does not tell you which one.

The frame measurements in [docs/AGENT_MATCHING_TIPS.md](docs/AGENT_MATCHING_TIPS.md#the-stack-frame) are **diagnostics**.
Use them to measure how many slots are missing and where.
Commit a spelling only when something outside the frame also supports it: an instruction difference, a sibling function in the same file that uses the same idiom, a reference project, a debug string, or the map.

Remove every pad and probe from a function before you measure anything else in it.
A pad sets the frame, so a correct candidate competes with it and measures worse than it is.

#### The accessor consistency test

An accessor that has no map symbol is a hypothesis.
Test it at **every** site in the game that touches that member, then read the whole `changes_all` report.

- A real accessor holds everywhere: no function gets worse.
- If any function gets worse, the hypothesis is refuted. Report it as refuted and remove it.
- Do not keep the accessor at the sites where it helps and remove it where it hurts. That is choosing by score, and it hides the evidence.
- An accessor that changes nothing anywhere has no evidence either.

A helper that the ROM **proves** (its out-of-line copy matches byte for byte, or the map names it) is different.
Use it at every site whose code it explains, also where the match percentage drops.
A site that gets worse then shows other damage in that function; do not hide it with a hand-written block that scores better by chance.

A few accessors are known exceptions, where the original used the getter and the raw field without a rule.
They are listed in [docs/AGENT_MATCHING_TIPS.md](docs/AGENT_MATCHING_TIPS.md#known-accessor-verdicts) with the other sweep results.

## UNUSED Functions (Inlined/Dead Code)

This is a critical concept for matching decomps.

### What are UNUSED functions?

The symbol map lists functions that were **compiled** by the original developers but **not emitted** into the final binary. This happens when:

1. **The function was inlined at all call sites** — the compiler replaced every call with the function body, so no standalone copy was emitted.
2. **The function was truly dead code** — defined but never called.

Despite having no assembly, these symbols are known from the linker map with their **mangled names** (giving us the full signature except return type) and **byte sizes**.

### Why do they matter?

UNUSED functions must still be reconstructed in the source because:
- The compiler inlines them into other functions. If the inline body is wrong, the calling function won't match.
- Their presence (or absence) affects code generation of surrounding functions.
- Their byte size (from the symbol map) serves as a verification constraint.

### How to reconstruct UNUSED inlines

1. **Identify candidate code**: Look at the existing decompiled code for patterns that look like they could be factored into a function matching the UNUSED symbol's name.
2. **Cross-reference the signature**: The demangled name gives argument types and the class it belongs to. Match these against code patterns in call sites.
3. **Use the byte size as a constraint**: The symbol map (`mario.MAP`) gives the exact compiled size for every UNUSED function. After writing a candidate body, compile with `ninja` and check the compiled size using `decomp-diff.py`:
   ```
   python tools/decomp-diff.py -u mario/Enemy/fireWanwan -s extra --search "functionName"
   ```
   The SIZE column shows the compiled size of your implementation. Compare it against the expected size from `mario.MAP`. PPC instructions are 4 bytes each, so `size / 4` gives the instruction count (minus alignment padding).

4. **Some UNUSED functions are truly dead**: If no code in the TU resembles the function, it may have been unused or called only from other UNUSED functions.

### Decide fast whether an UNUSED body is recoverable

An UNUSED function defined out of line in a `.cpp` can only be inlined **inside its own TU**: other TUs see only the declaration.
UNUSED also means that no linked function calls it out of line.
So there are two cases, and one command tells them apart:

```
python tools/decomp-diff.py -u <unit> -s nonmatching -t function
```

- A linked function in the TU is missing a block of about the UNUSED size: that is the inline site, and its ROM assembly **is** the body. Recover it.
  The caller's match rises, and the standalone copy hits the map size: two checks.
- Every linked function in the TU already matches: nothing in the shipped game reaches the UNUSED function. Leave the stub empty.

Before you call a body unrecoverable, also check `marioL.MAP` (see [The maps](#the-maps)) and the reference projects.

### Never fabricate an UNUSED body

Write a body for an UNUSED symbol only when you can reconstruct it from evidence: an inline site in the same TU, a function in `marioL.MAP`'s call tree, or a reference project.
With no evidence, leave the stub empty (`void Foo::bar() { }`), and write no `TODO`.
`validate-symbol-order.py` reports the size difference as a warning, and warnings never fail.

An exact size is a **necessary** check on a body that came from evidence.
It is never a **reason** to believe a body that you guessed: many different bodies compile to the same size.
Do not tune a guessed body until its size matches.

Stronger than the size is the **weak helper cluster**.
MWCC emits the weak helpers that a function first needs immediately after that function, so the run of weak symbols after an UNUSED entry in the map is a fingerprint of its body.

- A `__dt__` in the run means a by-value local of that type.
- `__construct_new_array` means `new T[n]`.
- A run of one class's accessors means that the body touched that class.

Check that your body brings back the whole run in the map's order, not only its own size.
Do not declare such a destructor yourself: let the call site generate it.

Before you size a destructor body, subtract the delete tail: an empty destructor is already about 0x70 bytes (see the tips document).

A fabricated helper that has no UNUSED entry must be `inline` (or `static inline`), not plain `static`.
A plain `static` function stays in the object after MWCC expands all its sites, so the map would list it as UNUSED.

## Working with the Codebase

### Key tasks an agent may help with

1. **Finding inlines** — Given a list of UNUSED function symbols (with names, signatures, sizes), identify which pieces of existing decompiled code correspond to inlined versions of those functions. Propose reconstructions.

2. **Understanding & documenting code** — Read decompiled functions and:
   - Suggest meaningful names for `unk*` member variables based on usage patterns.
   - Add comments explaining what complex code blocks do.
   - Identify game mechanics being implemented.

3. **Naming variables** — Many variables retain placeholder names. Study how they're used across functions to propose descriptive names.

### Important things to keep in mind

- **Always verify**. All decompiled code must be verified against the ground truth (original binary) via `decomp-diff.py`.
- **Always use tools for arithmetic**. When computing sizes, offsets, instruction counts, or any numeric conversions (e.g. hex↔decimal, size/4), use Python or a terminal command instead of mental math. LLMs are unreliable at arithmetic.
- **Order matters**. Functions and global variables must appear in the same order as in the original binary. The order is determined by the address in `symbols.txt`. **Important**: when `-inline deferred` is used, which it is for most game code, this order is **reversed**. The JAudio TUs do not use it, so their order follows the map forward. If in doubt, verify. If you move anything around, verify.
- **Include order matters**. It affects BSS layout and static initialization order.
- **Template instantiation** is explicit in the binary — each instantiation is a separate symbol.
- **The compiler is MWCC, not GCC/Clang**. It has unique optimization behavior, instruction selection, and register allocation. Code that looks equivalent in C++ may compile differently. The only ground truth is `decomp-diff.py` comparison.
- **`#pragma dont_inline on/off`** is sometimes needed to prevent the compiler from inlining a function when matching requires a standalone copy to exist. This is a temporary fakematch which can hopefully be figured out later. The real cause is almost always the inline pass ladder: a wrapper inline, a call in a conditional part, or a callee over the statement limit (see the tips document).
- **A high match percentage does not mean correct behavior.** objdiff ignores register names and scores a different offset as a small difference, so a logic bug can hide in the last 1-3%. Real gameplay bugs were found in 97-99.8% functions:
  * a field load at the wrong offset (`lfs 0x50` where the ROM has `lfs 0x58`), or a wrong array index,
  * a wrong bit in a mask (`rlwinm.`/`andi.`), or a wrong constant in a compare,
  * a virtual call through the wrong vtable slot (only the `lwz r12, N(r12)` displacement differs),
  * a `bl` to the wrong function,
  * a virtual call on the wrong receiver, or two values stored into swapped slots,
  * a misplaced closing brace (only one branch target differs),
  * a wrong stack local passed to a call, hidden because every `r1` offset already differs in a frame-short function,
  * `max` written where the ROM has `min`.

  For each `~` line in a diff, check the data flow: which field offset feeds which store, which register is the call receiver, which arm writes the result.
  Do not stop at the percentage.
- **A function in `mario.MAP` but not in `symbols.txt` is not missing.** `symbols.txt` lists only what survived in the DOL. Grep the map for the mangled name.

### Matching workflow for agents

- **One change at a time**. Make a single source-level change, rebuild with `ninja`, and check the diff immediately. Even small changes can have non-local effects on register allocation and instruction scheduling.
- **Prioritize structural differences**.
   - First of all, control flow and function call order must be matched. Sometimes, the control flow might not look stuctured and it might seem like a `goto` was used. In almost 100% of the cases, this is actually an inlined call to a different function with early returns inside of it. Introducing a TU-local (`static inline`) fabricated helper is a good first step, which can later be promoted to a method or one of the UNUSED functions in the TU. Function call ordering is trivial to match, unless inline functions are involved. When experiencing inlining issues, pragmas or additional wrappers can be temporarily employed to force a match. Remember to always mark them as fabricated/fake/needs further work.
   - Secnd, order of loads/stores to memory must be matched. Any function call in MWCC invalidates all cached loads, most stores to memory do too, and this can be used to figure out where exactly loads and re-loads happen in the original code.
   - Lastly, in the diff output, `|` (opcode replacement), `<` (deleted), and `>` (inserted) are the real issues. `~` (operand-only) mismatches are usually just register renumbering or stack offset differences that resolve once structural issues are fixed.
- **Don't try to process diff tool output with external utilities**. It is designed to be read by agents, and filtering out certain lines via regex will miss crucial context. The diff shows the disassembly of the original binary and current code simultaneously and should be read directly and sequentially in an attempt to understand what the current code does differently from the original.
- **Read the full diff before acting**. Use `--no-collapse` to see every instruction. Identify all problem clusters before making changes, then work on the largest cluster first.
- **Focus on one part of the function at a time**. Identify what exact lines in the source code a non-matching part of the disassembly corresponds. Use `--range` argument of the diff tool to only see the asm for the part being worked on.
- **Use temporary marker calls to map source to asm when anchors are missing**. If there are no obvious anchors (for example, no calls to known functions nearby), temporarily add a fake external marker like `extern void marker__();` and call it at a candidate point in the function. The call will show up clearly in diff output and helps bracket surrounding instructions, and you can repeat this process to narrow correspondence precisely.
- **Always remove marker calls after mapping**. Any extra call can change register allocation/scheduling and inhibit matching, so markers are strictly temporary debugging aids.
- **Prove every hunk.** Before opening a PR, revert each change on its own, rebuild, and keep only those that move the object toward the target.
- **Follow the file's own idioms.** Use a spelling only if an already-matching function in the same file or directory writes it the same way. A form the rest of the file never uses is usually a fakematch.
- **Read [docs/AGENT_MATCHING_TIPS.md](docs/AGENT_MATCHING_TIPS.md)** for detailed MWCC codegen patterns that come up repeatedly.
- **When two spellings both match, choose the one that invents nothing.** A plain local beats a new helper, and a helper that the map proves beats one that it does not.
- **Rename in small batches.** For each renamed field, state the evidence. Then run `ninja changes_all` and confirm that the objects did not change.

### Editing files on Windows

These traps have emptied or silently skipped edits before:

- Do not edit source files with a `python -c` one-liner. Write a script file, and pass literal strings through a JSON file.
- `open(p, 'wb').write(open(p, 'rb').read())` truncates the file before it reads it.
- PowerShell 5 removes embedded `"` from the arguments of native programs, so a replacement with quotes silently fails to match.
- Git Bash `sed -i`, and Python text mode with `newline=""`, change CRLF files to LF. `git diff` shows nothing, but `git status` shows the file as modified.
- `Set-Content -Encoding utf8` in PowerShell 5 adds a BOM.

After each scripted edit, check the result with `git diff --stat`.

### Common review feedback

- Call existing inlines instead of writing their bodies out.
- Fill an UNUSED function only with code visible inlined in a linked caller; otherwise leave it empty.
- Use `ARRAY_COUNT` for array loops and named enums for state values; if some values of a field are named, name all of its uses.
- Keep unknown fields as `unkXX`, and don't rename anything in shared headers in a unit PR.
- Don't copy globals or members into locals only to change codegen.
- Keep existing `TODO` comments unless the change resolves them.
- One unit per PR; leave out edits that don't change the object.
- If the bot reports broken matches in units you didn't touch, the branch is behind main. Merge main.

### Pre-PR checklist

Before submitting a PR with matching work, make sure to check the following:
- Run `tools/validate-symbol-order.py -u <unit>` on each changed TU (CI runs it
  automatically on every changed `.cpp`). It automates several of the items
  below: symbol **presence**, non-weak **ordering**, and linked-symbol
  **linkage** against `mario.MAP`
- Whether all code entities available from mario.MAP are declared and defined
  * Every type related to the TU seen from the symbols (including ones with all methods marked as UNUSED)
  * For each class, every method available from the entire map (including ones that are emitted as weak symbols into other TUs)
  * For each TU, every function available in the map, including UNUSED ones
- Whether functions inside of each TU are defined in the same order as they are listed in the mario.MAP file (or reverse order if the unit is -inline deferred)
- If some UNUSED symbols were filled out with guessed code from inline sites -- check that the function size matches the size listed in mario.MAP
- Check that UNUSED symbols are defined inside of the **cpp file** and not the header.
  An `inline` function that MWCC expands at every site is never emitted, so it leaves no map entry at all.
  An UNUSED entry therefore means that the TU emitted an out-of-line copy, which is normally an ordinary non-inline function.
  An UNUSED symbol can be weak only when the code that referenced it was dead-stripped too (the map does not record the binding of UNUSED symbols)
- Check that all weak symbols are actually marked as inline in case of free functions or define in-line inside of the class in case of methods
- Check that no fabricated helper is plain `static`: a helper with no UNUSED map entry must be `inline`
- Whether the code uses properly uses available inlines of other classes (e.g. checkLiveFlag instead of direct access)
- Re-validate GX code using decomp-diff.py to see whether correct constants were used everywhere (e.g. GX_COLOR0 vs GX_COLOR0A0)
- Validate const-ness of global variables -- they get emitted to different sections depending on if const is present or not
- Validate that all weak symbols are actually marked as inline
  * In case of methods, they should be defined in-line in the header rather than in the cpp file and the inline keyword should not be used
- Validate that all virtual functions in each class are properly marked as virtual (even if they are overriden) and are sorted in the order they appear inside of the virtual table
- After a header change, rebuild everything and confirm no other unit lost a match.
- For a unit that is empty on main, the symbol check allows nothing. Every symbol the map lists for it, weak and template copies included, must exist in the object.
- Run `ninja baseline` and `ninja changes_all` for **every** supported version (see [Build & Diff Workflow](#build--diff-workflow)), and confirm that each DOL still reports `OK`.
- CI also checks the other versions; don't change or invent `VERSION_*` code you can't verify against that version.
- Check that no pad, probe, marker call or `volatile` frame trick is left in the source.
- Commit messages are one line, with no `Co-Authored-By` line.
