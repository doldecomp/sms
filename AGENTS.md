# Agent Guide for Super Mario Sunshine (SMS) Decompilation

For parallel Claude and Codex orchestrators, follow `docs/CODEX_INTEGRATION.md` and claim work before creating a unit worktree.

This document describes the structure, conventions and workflows of this repository to help AI agents contribute effectively. The project is a matching decompilation of **Super Mario Sunshine** (GameCube, MWCC 1.2.5 C++ compiler, PowerPC Gekko target).

Important preliminary note: always prefer putting sentences in `.md` files on different lines to reduce the diff shown in git when rewording just one sentence — the lines still get collapsed into a single paragraph as per `.md` formatting rules.

## This clone

This is a personal clone, not upstream `doldecomp/sms`, and it is configured differently from what the rest of this guide assumes.
Read this section before running any command below.

| | Upstream default | **This clone** |
| --- | --- | --- |
| Active version | `GMSJ01` (Japanese) | **`GMSE01`** (North American, rev 0) |
| Linker map | `orig/GMSJ01/files/mario.MAP` | **`orig/GMSE01/files/marioUS.MAP`** |
| Platform | Windows (`.exe` suffixes) | **Linux**, no `.exe` suffix |
| Ninja | `ninja` on `PATH` | **`build/venv/bin/ninja`** |
| Python | `python` | **`python3`** for repo tools; **`build/venv/bin/python3`** when venv packages are needed |
| `m2c` | ask the user for a path | **`$M2C`** (the user's `m2c.py`; ask if unset), run with `build/venv/bin/python3` |

The `GMSJ01` and `GMSP01` configs still exist in the tree but have no input image here, so they cannot be built or regression-tested.

**Neither `python` nor `ninja` is on `PATH`.**
Commands copied verbatim from upstream documentation will fail with "command not found"; use the invocations in the table above.

`tools/validate-symbol-order.py` also defaults to the Japanese map and a Windows `nm`; its section below gives the overrides.
Wherever this guide says `ninja` or `mario.MAP`, read `build/venv/bin/ninja` and `marioUS.MAP`.

The US map is a real linker map shipped on the disc, so it is a priority source of truth exactly as `mario.MAP` is upstream.
Region-specific layout differences between the Japanese and US binaries are real and recurring — see `docs/catalog/region-us.md`.
Guard them with `#if defined(VERSION_GMSE01)` rather than silently changing shared offsets, since the other regions cannot be rebuilt here to catch a regression.

`STRATEGY.md` is the work order and `PROGRESS.md` holds the current numbers and open risks; `DECOMPILATION_PLAN.md` indexes the rest.
What this clone has learned about matching is in `docs/catalog/`: read its short `README.md` index, then only the topic file that fits the problem.
Old batch history is archived under `docs/progress/` and does not need reading.


## Read next

- Agents (unit, closure, research, structural): `docs/agent-brief.md` (the short working rules every agent follows), `docs/AGENT_GUIDE.md`, then your brief (`docs/BATCH_BRIEF.md` or `docs/CLOSURE_BRIEF.md`), then `docs/catalog/LEVERS.md` (grep `RULES.md` only for lookup).
- The orchestrating session: `docs/HANDOFF.md` first (the previous orchestrator's judgement and open leads), then `docs/ORCHESTRATION.md`, `STRATEGY.md`, `PROGRESS.md`, `docs/TIMELINE.md`.

This file is loaded into every session and every agent, so it holds only the rules that are not negotiable.

## Autonomy in this clone

Upstream forbids autonomous work on the MSL runtime, MetroTRK, THPPlayer, the Dolphin SDK and JSystem middleware.
**That prohibition is lifted here.**
All libraries are in scope: decompiling them, linking already-matching objects from source, and fixing their build configuration.
The 308 library objects that already matched byte-for-byte have all been source-linked.
When counting linked objects, filter on membership in `config/GMSE01/objects.json`: a zero-code unit reports `complete_code_percent` of 100 vacuously.

One rule survives the lift, and it is not negotiable:

> **Work only from the binary, the disc's linker map, and this repository.**
> Leaked Nintendo SDK, JSystem and MSL sources are in circulation.
> Never seek them out, never copy from them, and never paste code of unclear provenance into this tree.
> If a chunk of proposed code cannot be justified from the disassembly or the map, it does not go in.

Publicly documented headers, SDK documentation, other clean-room decomp projects and compiler behaviour are all fine evidence.


## Non-negotiable matching rules

- No fakematches: no `volatile char trash[]`/`u32 pad[]` stack padding, no `#pragma dont_inline`, no reinterpret casts, no nonsensical temporaries or control flow to force bytes. Temporary padding to confirm a body is otherwise exact is fine if removed. Leave a `// TODO:` naming the remaining difference instead.
- One change at a time; verify with `tools/decomp-diff.py`, `build/venv/bin/ninja changes_all` and `tools/validate-symbol-order.py`; never commit a regression.
- Use tools for all arithmetic (sizes, offsets, hex). Definition order follows the map (reversed under `-inline deferred`); UNUSED bodies live in the `.cpp` and should compile to their map sizes.
- Worktree agents edit only their own units' `.cpp` and headers; a shared-header need is parked as a TU-local `static inline` in the `.cpp` and reported, never made.

### Committing

Commit each batch yourself once it passes verification — no need to ask.
A batch is committable when `ninja changes_all` shows no function regressions **and** the rebuilt `mario.dol` still matches `a6782903ef79d4196c8489ecb1b57decb5b3728f` byte-for-byte.
If either check fails, fix or revert; never commit a red batch.

Do **not** push.
There is no fork configured, and every batch exists only on this disk — raise that with the user rather than inventing a remote.

Keep commit messages in the existing style: imperative mood, one line naming what was restored or corrected, and the `Claude-Session:` trailer.
