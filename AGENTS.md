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

```
config/GMSJ01/
  config.yml          — decomp-toolkit project config
  symbols.txt         — layout of the binary, addresses, sizes, names etc for every symbol present
  splits.txt          — per-TU section address ranges
  build.sha1          — SHA1 hash of the target DOL

src/                  — decompiled C/C++ source files
include/              — headers (class declarations, inline functions)
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
If the worktree already has changes and you want to test for regressions when compared to master, stash the changes, call `ninja baseline`, pop the stash and call `ninja changes_all`.
When asked to work on a task, if a worktree is clean or only contains general scaffolding like classes and function stubs, you **must** use `baseline` before starting work and `changes_all` after finishing work to report on the results to the user.

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

### Decompiling brand-new code without existing scaffolding

When a user requests to decompile a fresh TU for which no stubs or header exists, `docs/PROGRAM_STRUCTURE_REVVING.md` **must** be followed to reconstruct the scaffolding like declarations/definitions of methods/functions, class definitions, global variables, etc.

**Important**: as soon as you notice some method declaration and/or definition missing, thats a sure-sign of someone forgetting to scan the entire `mario.MAP` file properly for all known methods of some class and/or all known symbols in a TU.
Do the rescan yourself for relevant classes and TUs using relevant regular expressions containing file name or the correct mangling suffix that all such class members have.

## Source Organization

Each `.o` file maps 1:1 to a `.cpp` file. The path is listed in `configure.py` under `config.libs`. Each object has a status:

- `Matching` — our code compiles to byte-identical output. Linked into the final DOL.
- `NonMatching` — work in progress. Not linked.
- `Equivalent` — functionally equivalent but not byte-identical. Only linked with `--non-matching`.

### Directory structure mirrors game libraries

| Directory | Description |
|-----------|-------------|
| `src/JSystem/` | Nintendo's JSystem middleware (J3D, JParticle, JDrama, JAudio, etc.) |
| `src/dolphin/` | Dolphin SDK (OS, GX, DVD, PAD, etc.) |
| `src/PowerPC_EABI_Support/` | Metrowerks runtime & MSL |
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
- `// TODO:` — known issues, suspected inaccuracies, or incomplete understanding.
- `// Tiny size mismatch` / `// TODO: incorrect size` — for functions which were completely inlined and not emitted to the original binary, but are present in the symbol map as UNUSED symbols and their size is available. That size can be compared against the size in our code using tools/decomp-diff.py and if it's different, then the guess for the function's contents is not correct yet.
- `// correct but X is incorrect` — the function itself matches, but a called function does not.
- `// rogue includes needed for matching sinit & bss` — includes added purely for BSS/static-init ordering.
- `#pragma dont_inline on/off` — forces the compiler to not inline a function (required for matching in specific cases).

Don't be afraid to leave notes that would be useful to the next person trying to match the code, figure it out, or in the far future, write mods for the game.

### Style

- Formatting follows `.clang-format` (WebKit-based, 80-column, tabs for indent).
- C++98 style: no auto, no range-for, no lambdas, no `nullptr` keyword (it's a macro).
- `#include` paths use angle brackets with project-relative paths: `#include <Enemy/FireWanwan.hpp>`.
- Header guards use `#ifndef FOLDER_FILENAME_HPP` / `#define` / `#endif` pattern.
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
- **Order matters**. Functions and global variables must appear in the same order as in the original binary. The order is determined by the address in `symbols.txt`. **Important**: when `-inline deferred` is used, which it is for most game code, this order is **reversed**. If in doubt, verify. If you move anything around, verify.
- **Include order matters**. It affects BSS layout and static initialization order.
- **Template instantiation** is explicit in the binary — each instantiation is a separate symbol.
- **The compiler is MWCC, not GCC/Clang**. It has unique optimization behavior, instruction selection, and register allocation. Code that looks equivalent in C++ may compile differently. The only ground truth is `decomp-diff.py` comparison.
- **`#pragma dont_inline on/off`** is sometimes needed to prevent the compiler from inlining a function when matching requires a standalone copy to exist. This is a temporary fakematch which can hopefully be figured out later.

### Matching workflow for agents

- **One change at a time**. Make a single source-level change, rebuild with `ninja`, and check the diff immediately. Even small changes can have non-local effects on register allocation and instruction scheduling.
- **Prioritize structural differences**.
   - First of all, control flow and function call order must be matched. Sometimes, the control flow might not look stuctured and it might seem like a `goto` was used. In almost 100% of the cases, this is actually an inlined call to a different function with early returns inside of it. Introducing a TU-local (`static`) fabricated helper is a good first step, which can later be promoted to a method or one of the UNUSED functions in the TU. Function call ordering is trivial to match, unless inline functions are involved. When experiencing inlining issues, pragmas or additional wrappers can be temporarily employed to force a match. Remember to always mark them as fabricated/fake/needs further work.
   - Secnd, order of loads/stores to memory must be matched. Any function call in MWCC invalidates all cached loads, most stores to memory do too, and this can be used to figure out where exactly loads and re-loads happen in the original code.
   - Lastly, in the diff output, `|` (opcode replacement), `<` (deleted), and `>` (inserted) are the real issues. `~` (operand-only) mismatches are usually just register renumbering or stack offset differences that resolve once structural issues are fixed.
- **Don't try to process diff tool output with external utilities**. It is designed to be read by agents, and filtering out certain lines via regex will miss crucial context. The diff shows the disassembly of the original binary and current code simultaneously and should be read directly and sequentially in an attempt to understand what the current code does differently from the original.
- **Read the full diff before acting**. Use `--no-collapse` to see every instruction. Identify all problem clusters before making changes, then work on the largest cluster first.
- **Focus on one part of the function at a time**. Identify what exact lines in the source code a non-matching part of the disassembly corresponds. Use `--range` argument of the diff tool to only see the asm for the part being worked on.
- **Use temporary marker calls to map source to asm when anchors are missing**. If there are no obvious anchors (for example, no calls to known functions nearby), temporarily add a fake external marker like `extern void marker__();` and call it at a candidate point in the function. The call will show up clearly in diff output and helps bracket surrounding instructions, and you can repeat this process to narrow correspondence precisely.
- **Always remove marker calls after mapping**. Any extra call can change register allocation/scheduling and inhibit matching, so markers are strictly temporary debugging aids.
- **Read [docs/AGENT_MATCHING_TIPS.md](docs/AGENT_MATCHING_TIPS.md)** for detailed MWCC codegen patterns that come up repeatedly.

### Pre-PR checklist

Before submitting a PR with matching work, make sure to check the following:
- Whether all code entities available from mario.MAP are declared and defined
  * Every type related to the TU seen from the symbols (including ones with all methods marked as UNUSED)
  * For each class, every method available from the entire map (including ones that are emitted as weak symbols into other TUs)
  * For each TU, every function available in the map, including UNUSED ones
- Whether functions inside of each TU are defined in the same order as they are listed in the mario.MAP file (or reverse order if the unit is -inline deferred)
- If some UNUSED symbols were filled out with guessed code from inline sites -- check that the function size matches the size listed in mario.MAP
- Check that UNUSED symbols are defined inside of the **cpp file** and not the header -- UNUSED symbols are never weak so cannot be inline
- Check that all weak symbols are actually marked as inline in case of free functions or define in-line inside of the class in case of methods. Note that this does not include UNUSED symbols, as they are **never** weak
- Whether the code uses properly uses available inlines of other classes (e.g. checkLiveFlag instead of direct access)
- Re-validate GX code using decomp-diff.py to see whether correct constants were used everywhere (e.g. GX_COLOR0 vs GX_COLOR0A0)
- Validate const-ness of global variables -- they get emitted to different sections depending on if const is present or not
- Validate that all weak symbols are actually marked as inline
  * In case of methods, they should be defined in-line in the header rather than in the cpp file and the inline keyword should not be used
- Validate that all virtual functions in each class are properly marked as virtual (even if they are overriden) and are sorted in the order they appear inside of the virtual table
