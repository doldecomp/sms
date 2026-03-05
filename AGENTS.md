# Agent Guide for Super Mario Sunshine (SMS) Decompilation

This document describes the structure, conventions and workflows of this repository to help AI agents contribute effectively. The project is a matching decompilation of **Super Mario Sunshine** (GameCube, MWCC 1.2.5 C++ compiler, PowerPC Gekko target).

## Project Goal

Reconstruct the original C++ source code such that compiling it with the original Metrowerks CodeWarrior compiler produces **byte-identical** object files to the original game binary (`mario.dol`). This is called a **matching** decomp.

### Mindset

The work is fundamentally about getting into the heads of the original developers using only the evidence available — the binary, the linker map, debug strings, and the compiler's behavior. Every assumption must be doubted and verified. Be sceptical of everything: naming guesses, code structure, control flow choices, even whether a function "looks right." Only commit to a decision when the evidence is overwhelming.

When progress stalls because a tough choice must be made and strong evidence is lacking, **leave the code nonmatching and move on**. New evidence often emerges later — from matching a neighboring function, finding a pattern in another TU, or discovering a debug string. Premature commitment to a wrong approach creates technical debt that's harder to undo than a TODO comment. The final goal is still always a 100% match, but trying to headbutt a particular function into matching 100% usually results in fakematches and technical debt. Case-by-case judgement should be used.

Truly hard judgment calls — ambiguous code structure, naming disputes, architectural decisions — should be deferred to humans. The ultimate goal of the project is not just a matching binary but **human-readable, modifiable source code**.

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
build/tools           — various tools: dtk, objdiff-cli, sjiswrap

tools/
  configure.py        — (imported by configure.py at root)
  decompctx.py        — generates decomp.me context files
  project.py          — build system library

configure.py          — main build configuration; lists ALL objects and their matching status
objdiff.json          — auto-generated config for objdiff (the diffing tool)
docs/                 — documentation on reverse-engineering methodology
```

## Build & Diff Workflow

Note that on windows, all tool names should be suffixed with `.exe`, while the following documentation uses the UNIX spelling (without `.exe`) for simplicity.

Ninja is used for most workflows. Simply running `ninja` in the root of the repository will build the project and report the overall matching progress.

To check changes against a baseline, `ninja baseline` can be used to generate a baseline report on the current state of the work tree, after which `ninja.exe changes_all` can be used to show how the current changes to work tree influence matching progress, including a per-symbol report.

The underlying utility used for splitting the binary is `dtk` (`build/tools/dtk`).

The main diffing tool used by humans is **objdiff** (`encounter/objdiff`). It compares the compiled `.o` from our source against the original `.o` extracted from the DOL, function by function, showing PPC assembly side-by-side. It automatically rebuilds on file changes.

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

Output shows left (target/original) vs right (our build) PPC assembly with diff markers:
- ` ` (space) — instructions match
- `~` — argument mismatch (operand differs)
- `|` — opcode replacement (different instruction)
- `>` — inserted (exists only on right side)
- `<` — deleted (exists only on left side)

Differing arguments are wrapped in `{braces}` for easy identification.

Matching instruction runs are collapsed by default (shows 3 lines of context). Options:
- `-C 5` — show 5 context lines instead of 3
- `--no-collapse` — show every instruction
- `--range 100-300` — only show instructions at hex offsets 0x100–0x300

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

For detailed codegen patterns and matching tips, see **[docs/AGENT_MATCHING_TIPS.md](docs/AGENT_MATCHING_TIPS.md)**. Expand this document as you notice new patterns, but always ask for human review afterwards — some patterns might be red herrings.

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

- **Use original names wherever possible.** The symbol map (`mario.MAP`, `symbols.txt`) and debug strings embedded in the binary are the primary sources for names. For example, `PARAM_INIT` stringifies its field name argument, so all parameter struct members have their original names recoverable from the binary's `.rodata` section.
- Function arguments: `snake_case` (e.g., `live_manager`, `hit_actor`). Or palceholders where unsure: `param_1`, `param_2`, etc.
- Local variables: `camelCase` (e.g., `distToMario`, `nextNode`). Or placeholders where unsure what name to chose: `fVar1`, `uVar2`, etc.
- Classes are prefixed with `T` (e.g., `TFireWanwan`, `TSmallEnemy`).
- Manager classes end with `Manager` (e.g., `TFireWanwanManager`).
- Japanese strings appear in actor names (e.g., `"ファイヤーワンワン尻尾当たり"`).
- Member variables use camelCase, sometimes prefixed with `m` (e.g., `mHitPoints`, `mPosition`).
- Unknown members retain placeholder names like `unk194`, `unk238`, with hex offsets.
- Method names use camelCase: `moveObject()`, `calcRootMatrix()`, `behaveToWater()`.

### Comments & annotations in the code

- `// fabricated` — the code is not from the original binary; it was invented/guessed to make things compile. May be incorrect.
- `// TODO:` — known issues, suspected inaccuracies, or incomplete understanding.
- `// Tiny size mismatch` / `// TODO: incorrect size` — compiled output is close but not byte-identical to the target.
- `// correct but X is incorrect` — the function itself matches, but a called function does not.
- `// rogue includes needed for matching sinit & bss` — includes added purely for BSS/static-init ordering.
- `#pragma dont_inline on/off` — forces the compiler to not inline a function (required for matching in specific cases).

### Style

- Formatting follows `.clang-format` (WebKit-based, 80-column, tabs for indent).
- C++98 style: no auto, no range-for, no lambdas, no `nullptr` keyword (it's a macro).
- `#include` paths use angle brackets with project-relative paths: `#include <Enemy/FireWanwan.hpp>`.
- Header guards use `#ifndef FOLDER_FILENAME_HPP` / `#define` / `#endif` pattern.

### Fakematches

A **fakematch** is code that compiles to byte-identical output but is clearly not what the original developers wrote. Examples include nonsensical casts, unnecessary temporaries, bizarre control flow, or abusing compiler quirks to force specific code generation. Fakematches should be **avoided where possible** — the goal is to reconstruct code that a real Japanese developer at Nintendo would have plausibly written in ~2002 using C++98. If a match can only be achieved through obviously artificial code, it's better to leave the function as nonmatching with a TODO comment than to commit a fakematch.

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

1. **Identify candidate code**: Look at the existing decompiled code for patterns that look like they could be factored into a function matching the UNUSED symbol's name, signature, and size.
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

- **Never guess assembly**. All decompiled code must be verified against the original binary via objdiff. Suggest code, but understand it must be tested.
- **Always use tools for arithmetic**. When computing sizes, offsets, instruction counts, or any numeric conversions (e.g. hex↔decimal, size/4), use Python or a terminal command instead of mental math. LLMs are unreliable at arithmetic.
- **Order matters**. Functions and global variables must appear in the same order as in the original binary. The order is determined by the address in `symbols.txt`. **Important**: when `-inline deferred` is used, which it is for most game code, this order is **reversed**.
- **Include order matters**. It affects BSS layout and static initialization order.
- **Template instantiation** is explicit in the binary — each instantiation is a separate symbol.
- **The compiler is MWCC, not GCC/Clang**. It has unique optimization behavior, instruction selection, and register allocation. Code that looks equivalent in C++ may compile differently. The only ground truth is objdiff comparison.
- **`#pragma dont_inline on/off`** is sometimes needed to prevent the compiler from inlining a function when matching requires a standalone copy to exist. This is usually a temporary fakematch which can hopefully be figured out later.

### Matching workflow for agents

- **One change at a time**. Make a single source-level change, rebuild with `ninja`, and check the diff immediately. Even small changes can have non-local effects on register allocation and instruction scheduling.
- **Focus on structural mismatches first**. In the diff output, `|` (opcode replacement), `<` (deleted), and `>` (inserted) are the real issues. `~` (operand-only) mismatches are usually just register renumbering or stack offset differences that resolve once structural issues are fixed.
- **Don't try to process diff tool output with external utilities**. It is designed to be read by agents, and filtering out certain lines via regex will miss crucial context. The diff shows the disassembly of the original binary and current code simultaneously and should be read directly and sequentially in an attempt to understand what the current code does differently from the original.
- **Read the full diff before acting**. Use `--no-collapse` to see every instruction. Identify all problem clusters before making changes, then work on the largest cluster first.
- **Focus on one part of the function at a time**. Identify what exact lines in the source code a non-matching part of the disassembly corresponds. Use `--range` argument of the diff tool to only see the asm for the part being worked on.
- **Read [docs/AGENT_MATCHING_TIPS.md](docs/AGENT_MATCHING_TIPS.md)** for detailed MWCC codegen patterns that come up repeatedly.
