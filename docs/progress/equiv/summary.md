# Semantic-equivalence certification (`tools/equiv-check.py`)

Generated from `build/GMSE01/report.json` and the retail/our objects of the current build.
Byte counts are retail function sizes.

| verdict | functions | bytes |
| --- | ---: | ---: |
| exact (report.json 100%) | 11874 | 2542144 |
| EQUIVALENT (L1 435, L2 70) | 505 | 383236 |
| DIFFERENT | 169 | 192936 |
| UNSUPPORTED | 356 | 485432 |
| **all code** | 12904 | 3603748 |

**exact + certified: 2925380 / 3603748 bytes = 81.18% of all code** (exact alone 70.54%; certification adds 10.63 points).

Per-function verdicts: `equiv.tsv`.  How the checker was validated: `validation.md`.
Regenerate with `python3 tools/equiv-check.py --write` after a build (about 20 s; the callee
summaries are cached in `build/GMSE01/equiv-callees.json`, `--rebuild-callees` refreshes them).

Most common non-certified reasons (numbers and addresses elided):

| verdict | reason | functions |
| --- | --- | ---: |
| UNSUPPORTED | stack: address-taken object layout differs | 96 |
| UNSUPPORTED | stack: address-taken object extent differs | 96 |
| DIFFERENT | block # effect # address differs: # vs # | 46 |
| UNSUPPORTED | cfg: # vs # blocks | 35 |
| UNSUPPORTED | stack: access inside address-taken object differs | 24 |
| DIFFERENT | block # branch: # vs # | 24 |
| UNSUPPORTED | stack: inconsistent slot mapping | 23 |
| DIFFERENT | block # effect #: # vs # | 22 |
| UNSUPPORTED | ours | 21 |
| UNSUPPORTED | stack: unpaired address-taken slot | 19 |
| UNSUPPORTED | stack: store-only slot at a different SP offset | 19 |
| UNSUPPORTED | cfg: block # (bc->[#, #] vs bc->[#, #]) | 13 |
| DIFFERENT | block # effect # icall args differ: r#: # vs # | 10 |
| DIFFERENT | block # effect # call args differ: r#: # vs # | 10 |
| DIFFERENT | block # effect # stored value differs: # vs # | 9 |
| DIFFERENT | block # return value f# differs (#) | 7 |
| DIFFERENT | block # return value r# differs (#) | 7 |
| DIFFERENT | block # effect # call args differ: f#/f#: # vs # | 6 |
| DIFFERENT | block # effect # icall args differ: f#/f#: # vs # | 6 |
| DIFFERENT | block # effect # call args differ: r#/r#: # vs # | 4 |
| DIFFERENT | block # effect # call args differ: f#: # vs # | 4 |
| DIFFERENT | block # effect # icall args differ: f#: # vs # | 4 |
| UNSUPPORTED | stack: slot layout differs | 3 |
| UNSUPPORTED | cfg: block # (b->[#] vs b->[#]) | 3 |
| DIFFERENT | block # effect #: # vs - | 3 |

## What EQUIVALENT means

The retail function (P) and ours (Q) have the same CFG, and for every basic
block the ordered side effects (non-stack loads, stores, calls with the
argument registers the callee may read, special-purpose instructions), the
branch conditions and the return registers some caller reads are the same
expressions over the function's inputs.  Values are hash-consed expression
trees; block-entry values are equivalence classes of registers/stack slots
computed from every predecessor (optimistic start, refined to a fixpoint on
loops), so register allocation may differ per block.
Level 1: the opcode sequences are identical (only registers, stack slots and
the frame differ).  Level 2: instructions are reordered inside blocks
(scheduling), or moves are added/removed.

## Assumptions (soundness)

- Compositional: a function is certified assuming every function it calls
  behaves like its retail version, and whole-program facts are taken from the
  retail objects (`build/GMSE01/obj`).  Per retail function the tool computes,
  by fixpoints over the call graph: the volatile registers it may read (argument
  registers compared at direct call sites), the volatile registers it may pass
  through unchanged, how many bytes it may access through each pointer argument
  (and which argument pointer it may return), how many bytes of its caller's
  outgoing-argument area it may read, and which of r3/r4/f1 any caller reads
  after calling it (all three for address-taken functions: vtables, callbacks).
- Indirect calls may read r3-r10, f1-f8 and cr1 (EABI) and access anything a
  pointer argument reaches.
- Return registers that no retail caller reads are not compared (a mutated
  constructor return value `this+1` is correctly reported EQUIVALENT when every
  caller ignores it).
- Compiler-anonymous data (`@NNNN`, `...data.0`, `@stringBase0` strings, switch
  tables) is compared by bytes and relocations and assumed to be accessed within
  its own extent; writable anonymous objects and `name$N` statics (local static
  guards) are additionally matched by first-reference order within the
  function.  Loads from `.rodata`/`.sdata2` are pure (may be reordered).
- Stack: the extent of an address-taken slot comes from the pointer-extent
  analysis; every access inside it must move with it (one translation per
  object) and its accesses are ordered memory effects.  Other regions may be
  permuted freely (consistent bijection) and are compared by dataflow.  A slot
  that is stored but never loaded keeps its SP offset if a callee may read it as
  a stack argument, and is dropped as dead otherwise.
- Equivalence is for executions that do not read uninitialised stack memory.
  NaN payload propagation of commutative FP operations (fadd/fmul operand order)
  and the XER summary-overflow bit are not modelled.  The saved-register sets
  may differ: each side is checked to save and restore every callee-saved
  register it writes.

## Known limitations (why an equivalent function can be DIFFERENT/UNSUPPORTED)

- A different CFG (inlined vs called helper, different block layout, unrolling)
  is UNSUPPORTED, as are indirect tail jumps and unmatched symbols.
- Non-stack loads, stores and calls must stay in the same order.
- Algebraic identities are limited to copies, commutativity, constant folding of
  add-immediate chains and `cmpwi 0` == record form.
- An address-taken stack object passed to an indirect (virtual) call, stored in
  memory, or passed to a callee that does either has unbounded extent, so a
  shifted frame layout around it is UNSUPPORTED (`stack: address-taken ...`).
- Stale values in argument registers at indirect calls, and in r3/r4/f1 at the
  return of address-taken (virtual) `void` functions, are compared and can make
  an equivalent function DIFFERENT (`icall args differ`, `return value differs`).
