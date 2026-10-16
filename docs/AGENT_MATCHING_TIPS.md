# MWCC Matching Tips for Agents

This document collects facts about how Metrowerks CodeWarrior (MWCC `GC/1.2.5`) makes PowerPC code.
Use these facts when you change source code to get a byte-identical match against the original binary.

Read this document before you try to match functions.
The patterns here occur again and again, and they save much trial and error.
Add to the document when you find a new pattern.
Ask for human review when the document does not agree with what you see.

Each fact here was measured on this project with this compiler.
Do not use codegen facts from other decomp projects (TP, TWW, MKDD, AC): each one uses a different MWCC version.
From other projects, take source-level facts only: names, field order, control flow, whether a function or an assert existed.

## Terms

This document uses these terms with one meaning only:

- **ROM** or **target**: the original object code, extracted from the DOL.
- **site**: one place in the source that calls a function or reads a field.
- **inline expansion**: MWCC copies the body of a callee into the caller at a site.
- **inline pass**: one sweep of MWCC's inliner over the statement list (see [Inlining](#inlining)).
- **temp**: an object that the compiler makes and that has no name in the source. MWCC names temps `@NNN`.
- **slot**: one stack location that MWCC reserves for a local or a temp.
- **frame**: the full stack frame of a function, set by `stwu r1, -FRAME(r1)`.
- **frame deficit**: the frame of our build is smaller than the ROM frame. A **frame surplus** is the opposite.
- **frame-only**: every instruction matches, and only the frame size or the `r1` offsets differ.
- **vreg**: a virtual register, before register allocation.
- **node**: a vreg in the interference graph of the register allocator.
- **coalesced node**: a node that MWCC merged with a physical register, usually an argument register.
- **web**: one live range of a variable. A variable that gets a new value starts a new web.
- **UNUSED**: a symbol that `mario.MAP` lists with a size but that the linker removed.
- **fabricated**: a name or a function that has no evidence in the map, in strings or in a reference.

## Where to start

### What NOT to focus on first

Some differences look easy, but they are the most difficult to fix:

- the frame size,
- the prologue and epilogue shape,
- wrong registers.

Fix these last, after everything else in the file matches.
First fix wrong instructions and the order of instructions.

### Priority order

1. Match the control flow and the order of calls.
2. Match the order of loads and stores.
3. Match the instruction selection.
4. Match the registers.
5. Match the frame.

Steps 4 and 5 often fix themselves when steps 1 to 3 are correct.
Step 4 can depend on step 5: the extra temps of a correct inline change the order in which MWCC colours registers.
Thus, do not try to fix a register swap before the inline depth is correct.

### Use the compiler sources and the MWCC debugger

A complete, 100% matching decompilation of `mwcceppc.exe` exists ([`rayanht/mwcc`](https://github.com/rayanht/mwcc); it covers GC 1.0 to 1.3, including 1.2.5).
It is usually at `../mwcc`. If it is not there, ask the human to clone it (see [AGENTS.md](../AGENTS.md#read-the-compiler-instead-of-guessing-the-mwcc-decompilation)).
Read it before you guess about codegen.

| Topic | Files |
|---|---|
| Parser, expressions, inlining, templates | `src/frontend/` (`CExpr.c`, `CInline.c`, `CTemplate*.c`, `CClass.c`, `CDecl.c`, `CParser.c`, `CMangler.c`) |
| IR optimizer: propagation, CSE, struct scalarization | `src/optimizer/Iro*.c` (`IroCSE.c`, `IroPropagate.c`, `IroSubable.c`, `IroVars.c`, `IroLinearForm.c`) |
| Code generation, register colouring, scheduling, stack frame | `src/backend/` (`CodeGen.c`, `Coloring.c`, `InterferenceGraph.c`, `Scheduler.c`, `StackFrameEABI.c`, `Peephole.c`, `StrengthReduction.c`) |

The `mwcc-izer` tool runs the real `mwcceppc.exe` and dumps its internal state after each pass.
It runs `GC/1.2.5` natively, so its dumps are exact for SMS.
If a human gave you the tool, use it like this:

```
python mwcc_debugger.py -C <sms root> -a '<mwcceppc command line> -sym on -c src/...' <mangled name> <outdir>
python mwcc_izer.py stack <unit> <mangled name>        # owner of each frame slot
python inline_trace.py -u <unit> --root <sms root> -C <sms root> <mangled caller>   # inline verdict per callee per pass
```

Get the command line from `ninja -t commands <object>`, and remove the `sjiswrap` and `transform_dep.py` wrappers.
To find a mangled name, give a name that does not exist: the tool prints each function that it skips.

The most useful dumps:

| Dump | Use |
|---|---|
| `variables.txt` | Every local and temp, with its register or its `r1` range. This is the frame layout. |
| `frontend-00-ast-initial-code.txt` | The AST after inlining. Shows which inline expansion made each temp. |
| `backend-00-initial-code.txt` | Read this first. If a difference is already here, the front end made it. Change the AST shape. |
| `regalloc-gpr-pass-1-all.txt` | The full interference graph: each node, its register, `fCoalesced`, and its neighbours. |
| `regalloc-gpr-pass-1-assigned.txt` | Nodes in colouring order, highest priority first. |

`inline_trace.py` shows `[N stmt]`, which is the statement count that the inline pass ladder uses.

The sweep scripts change the real source file between builds, and objdiff watches that file.
Restore the file at the end of each script.

## Control flow

### Ifs

An `if` always compiles to simple code:

- a compare (`cmpwi`, `cmplwi`, or an arithmetic instruction with a dot),
- a conditional branch (`beq`, `bne`, `ble`, ...),
- the true block,
- an unconditional branch `b` to the end of the false block,
- the false block.

MWCC **never** swaps the true block and the false block.
It also changes the control flow very little, so C++ control flow usually agrees with the assembly one to one.
Use this to read the polarity of a condition: `cmplwi r31, 0; beq L_one` with `li r0, 1` last means the source tested `p != nullptr`, and the null case is the second arm.

To find where an `if`/`else` ends, find the unconditional `b .L_X` at the end of the true block.
The code at `.L_X` is the first statement after the `if`.
A misplaced closing brace changes only one branch target, so objdiff shows almost no change.

### Ternaries

A ternary compiles like an `if`, with one exception.
MWCC can load the "else" value into the result register before the compare, and then the false block disappears:

```cpp
int b = thing != nullptr ? thing->field : 0;
```

Use this signature to tell a ternary from an early return:

- A branch target whose only instruction is `fmr` or `mr` into the return register is a ternary.
- A branch target that loads a constant is an early `return`.

```cpp
if (d < 0.0f) return 0.0f;   // the constant is loaded inside the taken branch
f32 x = d < 0.0f ? 0.0f : d; // the constant is loaded before the compare
```

Write a chained clamp as two ternaries with a named intermediate, not as one nested ternary.
The compare keeps the operand order of the source: `a > b` gives `fcmpo cr0, a, b`.

### Sequential integer compares

MWCC can change `if (a == 8 || a == 9 || a == 10)` into `if (a - 8 <= 2)`.
When you see the second shape with an enum, write it back as separate `==` compares.

### `&&`/`||` chains assigned to a bool

When you assign a multi-term `&&`/`||` expression to a `bool`, MWCC makes intermediate flag temps.
The shape is several `li rN, 0` first, `li rN, 1` at the success points, then `clrlwi.`/`beq` merges.
These come from the `CInline_LoadToTemp` pair in `CInline_SerializeELOR`.

```cpp
// Target has flag-register merges. This form makes them:
bool same = (-e <= dx && dx <= e) && (-e <= dy && dy <= e) && (-e <= dz && dz <= e);
if (!same) { ... }

// Nested ifs do NOT make them. They branch to the body directly:
bool same = false;
if (-e <= dx && dx <= e)
    if (-e <= dy && dy <= e)
        if (-e <= dz && dz <= e)
            same = true;
```

Group the terms to agree with the nesting of the flags.
Case: `MtxUtil.cpp` `TRope::constraintTail` went from 89.3% (nested ifs) to 99.8%.

A **ternary chain** assigned to a bool is different.
It writes the destination register directly in each arm, so the whole chain uses **one** register.
The ROM shape is several `li rN, 0` and one `li rN, 1` in the same register, then one `clrlwi. rN, rN, 24`.
An `&&`/`||` chain uses two or more registers, a `cror`/`mfcr`/`extrwi` merge, and `mr` moves.

A comparison written as a ternary arm gives `mfcr`/`extrwi`.
If the ROM branches there, the arm is a ternary itself: `x < y ? false : true`, not `x >= y`.

An `&&` inside a ternary condition is computed as 0 or 1, not as a short circuit.

### Switches

MWCC compiles a switch as a jump table or as branches.
A jump table uses `mtctr` and `bctr`.
A switch compiled as branches has more than one conditional branch after one compare:

```
cmpwi r0, 0x1
beq ...
bge ...
cmpwi r0, 0x0
bge ...
b   ...
... the code block inside the switch ...
```

MWCC **never** reorders the cases.
The order of the `case` labels must agree with the order in the jump table or the branch chain.

A compare constant is not always a `case` value.
`build_case_ranges` sorts the case values and adds default ranges between them.
`emit_case_range_binary_search` can compare against the start of a default range.
Case: in `conductor.cpp` `TConductor::isBossDefeated`, `default` grouped with `case 1`, then `case 3`, gives compares against `2` and `4`.
`2` is the default gap between the explicit cases, and `4` starts the default range above `3`.

### Control flow that looks like a goto

Inline expansion can make control flow that agrees with no `if`, `switch` or loop.
A jump that looks like a `goto` is almost always a `return` inside an inlined function.
The jump target is the end of the inline expansion.
Write a helper with early returns.
A fabricated helper must be `inline` (or `static inline`), never plain `static`.
A plain `static` function stays in the object after MWCC expands all its sites, and then the map must list it as UNUSED.
If the map has no UNUSED entry for it, the helper was not a `static` function.

### Release-only debug code

A release build removes debug warnings and asserts, but the **form** of the removed code changes the registers around it.

```cpp
if (sVir2PhyTable[vir_id] != 0xFFFF) { }       // the compare stays as dead nodes
if (sVir2PhyTable[vir_id] != 0xFFFF) (void)0;  // the dead compare goes away early
```

With the empty compound statement, MWCC computes the compare as a value.
The dead instructions are removed only after register colouring, so they take registers.
With `(void)0`, the condition is still evaluated (the load stays), but the compare chain never reaches the back end.
Case: `JASystem::BankMgr::setVir2PhyTable` went from 77.1% to 100%.

Use the existing macros: `JUT_ASSERT(...)` and `JUT_REPORT_MSG(...)` expand to `(void)0` in release builds.
Do not write a raw `#ifndef NDEBUG` block in braces, and do not leave `if (cond) { }`.

`JGADGET_ASSERT` and `JUT_ASSERT` are not no-ops for matching.
The empty statement still reaches the front end and changes temp numbering, the frame and register colouring.
Removing the body of `JGADGET_ASSERT` dropped three JGadget functions to below 67%.
An assert inside an inlined accessor can add a frame slot by itself.
Port every assert that a reference supports, as `JUT_ASSERT(<cond>);`. Asserts in this project have no line numbers.

A release-stripped `JUT_ASSERT(...)` is a `(void)0;` statement, and `(void)0;` is an `ST_EXPRESSION`.
It counts for the 15-statement gate and for the budgets of the pass ladder.
**One** assert can stop the inlining of a callee that is at the limit (see the table in [Is the callee inlinable at all](#is-the-callee-inlinable-at-all)).

### A dead `subfc`/`subfze` pair

MWCC computes `unsigned >= 0` with carry arithmetic, not with a branch: `li 0`, `li -1`, `subfc`, `subfze`.
When nothing uses the result, the users go away but the arithmetic stays.
In a loop, only the two `li` stay, hoisted into the preheader.

- `(cond) ? (void)0 : (void)0;` also removes the second half of an `&&`.
- `(void)(cond);` keeps it, so the upper-bound load and compare stay.

`x == -1` on an unsigned value gives `addis r0, rX, 1` / `cmplwi r0, 0xffff`. That is a real compare.

## Inlining

### Is the callee inlinable at all

For a body in a `.cpp` file, `CInline_GenFunc` marks a function as inlinable only when two checks pass:

- **`CInline_IsSmallFunction`**: at most **15 statements**, and at most 1024 bytes of locals.
- **`CInline_CanInline`**: the result is one of three values.
  - `CI_CanInline0` (never): varargs, a class return value whose class has a destructor, or a by-value class parameter whose class has a destructor.
  - `CI_CanInline6`: the body is only expression statements and one final `return`.
  - `CI_CanInline3`: the body has control flow.

The mark is on the function, so it is the same at each site.
**A difference between two sites is never caused by the callee.**

The 15-statement gate counts a flat statement list:

| Source | Statements |
|---|---|
| Declaration with an initializer (also `T& r = x;` and `T* p = &x;`) | 1 |
| Declaration without an initializer | 0 |
| `if (c) x;` | 2 (`if` plus the body statement; the label is free) |
| `(void)0;`, and so a release-stripped `JUT_ASSERT(...)` | 1 |
| an empty statement `;` or an empty block `{}` | 0 |
| `break`, `continue` | 1 |
| `while` or `for` | 4, the same for both |
| `return`, also the implicit `return` at the end of a `void` function | 1 |
| an `&&` chain in one `if` | still 1 |

The gate counts every statement except `ST_NOP` and `ST_LABEL` (`check_statement_count_and_locals_size` in `CInline.c` of the decompilation).
Measured with `GC/1.2.5` on a `void` callee: 14 stores inline, 15 stores do not, and 14 stores plus one `(void)0;` or one `JUT_ASSERT(p);` do not.

Read the real count from `frontend-00-ast-initial-code.txt`.
If the target inlines a function that our build calls, the original body was 15 statements or fewer.
Look for a helper that replaces a group of statements with one call, or fold `if (c) continue;` guards into one `&&` condition.
If the target calls a function that our build inlines, the original body was over the gate.
A named local for a value that the code already computes adds exactly one statement.

Case: on `JASystem::Dvd::loadToDramDvdT`, a five-line block was really a call to the UNUSED `Dvd::addTask`.
That took the function from 16 to 12 statements, and its caller from 32% to 100%.

### An in-class body has no gate

A body inside the class body sets `Q_INLINE`.
For such a function, MWCC does not run `CInline_IsSmallFunction` and does not read `#pragma dont_inline`.
The same constructor inlined with 14, 16 and 26 statements.

A member of a **class template** defined outside the class body is never marked inlinable: it is always a `bl`.
Move the body into the class to inline it at every site.
This changes every site, so do it only when every caller in the ROM inlines it.

### The inline pass ladder

`CInline_Expand` sweeps the statement list once per pass.
Each pass permits a smaller callee:

| Pass | Largest callee (`[N stmt]` in `inline_trace.py`) |
|---|---|
| 0: calls that the source writes | no limit |
| 1: calls that a pass-0 expansion uncovered | 10 |
| 2 | 7 |
| 3 | 3 |

**There is no pass 4.**
MWCC never scans a body that a pass-3 expansion inserted, so every call in it stays a `bl`.
This is true also for a one-statement accessor that MWCC inlines everywhere else, and MWCC then emits a weak copy of it in that TU.
The signature: a `bl` to a trivial inline at one place, and the same inline expanded a few instructions before.

How the depth grows:

- An expansion returns its last statement, so statements that a pass makes are first seen in the next pass.
- A `CI_CanInline6` body pasted as an expression is not walked again in the same pass.
- Calls nested in one statement's arguments are all walked in the same pass, so `a()->b()->c()` costs one pass, not three.
- **A call in the argument of an expanded call moves down one pass.** In `stack.push(fetchS32())`, `push` is pass 0, the implicit `TSpcSlice(int)` conversion is pass 1, and `fetchS32` is pass 2.
- A call nested inside an already expanded body costs no extra pass.

To push one callee deeper than its sibling, move the call into the other callee's body.
Do not nest it as an argument.

Case: `std::sqrtf` is 9 statements.
Every ROM `frsqrte` site is at pass 0 or 1, and every `bl sqrtf__3stdFf` site is at pass 2 or deeper.
To keep one call out of line, add one inline level above it.
Never add statements to `sqrtf`.

### Why a pass-0 call can stay a `bl`

A call statement at pass 0 is always expanded, except in these cases:

1. **The call is in a conditional sub-expression**: the right operand of `&&`, `||` or `,`, an arm of `?:`, or a null-check condition.
   There, a `CI_CanInline3` callee stays a call.
   A `CI_CanInline6` callee is pasted in anyway.
   An `if` condition is not a conditional part.
   In `new T(...)`, the constructor call is on the unconditional side, so even a `CI_CanInline3` constructor expands.
2. **The statement is not an expression statement** (the call is the full expression of a `return`, `if`, `switch`), **and the callee has a `return` before its last statement**.
3. The site came out of another expansion (pass 1 or more), and the callee is over that pass's budget.
4. The callee is `CI_CanInline0`, its body is not compiled yet, it is not inlinable, or it is a virtual call through the vtable.

Reason 1 is the clean way to keep one site out of line without a change to the callee.
Case: `JALListD::searchD` is inlined in `searchGroup` but called in `startSoundSetDyna`.
Writing the second site as the right operand of `||` gives both, and MWCC emits the weak copy that the map lists.

Case: `TFireWanwan::behaveToWater` calls `isWandering()`.
Inside it, the first `theNerve()` is unconditional and expands, and the same call behind `||` stays a `bl`.

These things do **not** change an inline decision:

- the shape of the arguments,
- `Class::f(...)` against `this->f(...)`,
- the position of an in-class body in the file (`-inline deferred` generates every function at the end of the TU).

`#pragma dont_inline` is read once, at the end of the function.
It cannot turn off inlining for one statement, and it also stops the function that it wraps from being inlinable.
Treat it as a placeholder that hides a real cause, never as a fix.
Case: in `JAISound.cpp`, both `#pragma dont_inline` blocks hid the `U7` wrapper family (`setSeInterFxmixU7` and others).
The wrapper expands at pass 0, and the real setter (15 statements) is then a pass-1 call that the ladder refuses.

### A zero-argument call of a constructor with default arguments costs one pass

```cpp
class TGameSequence {
	TGameSequence(u8 stage = 0, u8 scenario = 0, JDrama::TFlagT<u16> flag = 0)
	{
		set(stage, scenario, flag);
	}
};

TGameSequence local;          // synthesized __ct__13TGameSequenceFv, 1 statement,
                              // then the real ctor one pass deeper
TGameSequence local(a, b);    // the real ctor directly, no wrapper
```

The same rule applies to a member of such a type that the enclosing constructor default-initializes, for example `TFlagT<u16> unkC;` in `JDrama::TViewObj`.
The wrapper has no symbol, and MWCC always expands it.

The signature: the calls inside a constructor body stay `bl` although the ladder says that they fit, while the same callees expand at that depth elsewhere.
Case: `TApplication::TApplication()` keeps `bl TFlagT::TFlagT(const TFlagT&)` and `bl TFlagT::set(u16)` for each `TGameSequence` member.
With a plain `TGameSequence() { set(0, 0, 0); }`, the function is at 36%. The default-argument form gives 100%.
`THitActor::THitActor` keeps `bl __ct__...TFlagT<Us>FUs` for the same reason.

### Use the default argument, do not write it out

When a call passes a value that is the callee's default argument, use the default.
Case: `BathWaterManager.cpp`. `JDrama::TViewObj("<TViewObj>")` inlined the nested `TNameRef`/`TFlagT` constructors one level too deep.
`JDrama::TViewObj()` kept them as `bl`, like the target, and took the constructor from 90.6% to 100%.

### Keep track of the inlines that exist

Correct inline calls are the most important part of a match.
When a block of code occurs again, think that it can be an inline.
But the original authors also copied code, so do not assume.
Before you start on a function, look at the inlines of the classes that it uses and of the current TU.

UNUSED functions from the map often must exist in the source, although they have no standalone code in the binary.
Their inlined bodies set the registers, the load offsets and the branch layout of the callers.
Use the map's signature and size as constraints, take the body from the call sites, and check the callers after each change.

A debug build of another game shows only the inlines that something calls.
An inline that no site uses leaves no trace.
The SMS map also lists only functions that got an out-of-line copy somewhere.
So the absence of an inline from TP, TWW or our map is not proof that it did not exist.
A one-line forwarder on an outer class (`a->getBlock()->set(x)` written as `a->set(x)`) is fair to try, if you measure it.

Two changes that each measure as zero can be worth an inline level together.
Case: `J3DModelData::entryTexMtxAnimator`. `J3DMaterial::setTexMtx()` alone changed nothing, a named local for the `new` alone was worse, and both together matched.

### A getter that repeats before every operation is a wrapper inline

m2c shows an inlined wrapper as one pointer local that gets a new value again and again:

```cpp
pBuf = DSPInterface::getDSPHandle(channel->unk0);
pBuf->setPauseFlag(1);
pBuf = DSPInterface::getDSPHandle(channel->unk0);
pBuf->flushChannel();
```

When the same lookup comes back before each operation, the original source almost always called a wrapper:

```cpp
DSPInterface::setPauseFlag(channel->unk0, 1);
DSPInterface::flushChannel(channel->unk0);
```

Both forms give the same instructions, but the wrapper form reads correctly and gives correct registers in long functions.
Case: `JAInter::StreamLib::callBack` went from 98.7% to 99.8%.
Keep the pointer local only where the code reads a field through it.

### Look at the out-of-line copies before you change an inline

When MWCC does not inline something, it emits a weak or local copy into the caller's TU, and `mario.MAP` lists it.
That copy is the original body. Diff it before you change the inline.
If the ROM calls an inline out of line at some sites and expands it at others, the difference is at those sites, not in its body.

The number of `blr` instructions in a weak copy tells you the shape of the body.
Case: the weak copy of `TSpineBase::getLatestNerve` has two `blr` and no join, so it is `if (mCurrent) return mCurrent; return mPrevious;`, not a ternary.
A ternary would make it `CI_CanInline6` and change every inline decision around it.

### `-inline deferred` and the order of definitions

When a TU uses `-inline deferred` (most game code), define the functions in reverse order of the map.
The `src/JSystem/JAudio` TUs do **not** use it: their order follows the map forward.
Without `-inline deferred`, MWCC can inline a helper only into a function defined later in the file.
Thus, only the symbols below an UNUSED function in the map can be its callers.

## Loads, stores and CSE

### MWCC does not reorder

MWCC never moves memory stores, loads and calls relative to each other.
Write them in the source in the same order as in the target.
This is a good first thing to fix, because it gives shape to the rest of the function.

Any call removes all cached loads.
Most stores through a pointer do the same.
Use this to find where the original code loaded and loaded again.

### MWCC removes redundant reads, never writes

Each write in the source occurs in the assembly.
Thus, when the assembly stores the same location twice, the source also wrote it twice.

MWCC can remove a redundant read, but not always: an inline expansion can prevent it.

### Where member-load CSE happens

The front end merges repeated reads of `this->member` (`frontend-01` already has one load).
Inlining is complete before this optimizer runs, so all accessor calls look the same at that time.
These things stop the merge (`IRO_GetExprKills` in `IroCSE.c`):

- a call,
- a store through a pointer (an indirect store).

A store to a plain local does not stop it.

A member at **offset 0** is not a CSE candidate: its address is the bare `this`, not `this + offset`.

### An inline accessor breaks member CSE

The merge follows the **spelling**, not the address.
A read inside an inlined accessor is bound to that call's temp, so a later direct read of the same member is a **second load**.
An extra `lwz` of a member that MWCC could keep in a register is evidence of an inline accessor.

Case: `SDLModel::entry()`. The target loads `mSdlFlags`, tests two bits, then loads it again for `mSdlFlags |= FLAG_UNK1`.
`if (!(mSdlFlags & FLAG_UNK8) || ...)` gave one load (87.5%).
`if (!checkSdlFlag(FLAG_UNK8) || ...)` with `u32 checkSdlFlag(u32 flag) const { return mSdlFlags & flag; }` gave the target (96.3%).

- Do not return `bool` from such a checker. In a `||` chain, a `bool` return makes MWCC compute 0 or 1 and then test it. Return the masked bits.
- A `void` accessor on `this` (an "on" or "off" setter) costs no temp and changes no instruction.

For a flag word that a function tests many times and clears in the taken branches, the ROM shape is one `lhz` for the `if`/`else if` chain and a new `lhz` before each `offFlag()`.
You get this only when **every** read goes through the accessor.
One raw `mFlags & X` in the chain becomes the CSE root, and the loads again disappear.

The mask width is a different effect.
`!checkFlag(~(top bits))` gives `clrlwi. r0, rN, 19` only when a `u16` is in the signature (parameter or return).
Prefer `BOOL checkFlag(u16 flag) const`.

`JDrama::TFlagT<u16>::check(mask)` computes `(mValue & mask) != 0`, so the compare is `cmpwi`.
A direct `f.mValue != 0` gives `cmplwi`.
An `lhz` followed by `cmpwi rN, 0` proves `check()`. Take the mask from the `on()`/`off()` call on the same field.
Write 0 back with `f = 0`: `off(0xffff)` is a read-modify-write.

### `const` on an inline's pointer parameter stops CSE

When an inline reads a field through a pointer parameter and the caller reads the same field, MWCC normally merges the two loads.
A pointer to `const` stops the merge:

```cpp
const char* getSymbolName(TSpcSymbol* symbol);        // one shared load
const char* getSymbolName(const TSpcSymbol* symbol);  // two loads
```

Case: `Strategic/spcinterp.cpp` `TSpcInterp::dump` went from 98.8% to 100%, and every other caller still matched.
Try this before you restructure anything.

### Reading a source array through `void*` stops CSE

```cpp
TVec3<s16>* src = (TVec3<s16>*)getVtxPosArray();
for (...) { a[i] = src[i]; b[i] = src[i]; c[i] = src[i]; }
```

MWCC can prove that `src` does not alias the destinations, so it loads `src[i]` once.
The smaller loop body then unrolls more (×8).
When the target loads `src[i]` for each store and unrolls ×2, read through a `void*` and cast at each use:

```cpp
void* src = getVtxPosArray();
for (...) {
    a[i] = ((TVec3<s16>*)src)[i];
    b[i] = ((TVec3<s16>*)src)[i];
    c[i] = ((TVec3<s16>*)src)[i];
}
```

Case: `DrawUtil.cpp` `TTrembleModelEffect::init` went from 54% to 100%.

### A global base against a local base

`IRO_IsSubableExpression` (`IroSubable.c`) rejects `base + constant` only when the base is a local that can live in a register.
A global base is always a CSE candidate, and gets one address temp for each constant that the code uses twice or more.

- ROM: `lwz rX, 0xNN(rBase)` everywhere. Ours: `addi rT, rBase, 0xNN` and `lwz rX, 0(rT)`. The ROM base was a local or a pointer parameter, and ours is the global.
- The reverse means that you added a local that the original did not have.

How you seed the local matters:

- `T& x = TheGlobal;` and `T* x = &TheGlobal;` propagate back into the global, and the temps come back.
- `T* x = GetTheGlobal();` (a getter) survives propagation.
- An `inline` helper whose parameter is the pointer also works, when the argument is `GetTheGlobalPtr()`, not `&TheGlobalObj`.

Every source-declared register local ranks above `this` in the allocator.
If the target keeps `this` in r31, the original had no declared register local there, and the temps have another cause.

### A member function called on a global: bare name against accessor

On a global written by name, `gpApplication.setNextArea(x)`, MWCC puts the constant address in place of `this`, folds the member offsets, and makes `gpApplication + 0x12` a CSE temp:

```
addi r30, r5, 0x12       ; &gpApplication.mNextArea
stb  r0, 0x12(r5)
...
addi r3, r30, 2          ; &mNextArea.unk2
```

When the receiver is an expression that MWCC does not repeat (an inline accessor call, a local pointer, a local reference), `this` is bound to a temp that holds `&gpApplication`:

```
addi r31, r4, gpApplication@l
stb  r0, 0x12(r31)
stb  r0, 0x13(r31)
addi r3, r31, 0x14
```

The second shape is the sign of an accessor such as `SMSGetApplication()`, `SMSGetMSound()` or `SMSGetMarDirector()`.
The two forms differ only where the result goes into another inline call.

The same test tells a getter from a raw field read.
A getter such as `u8 getStage() const { return unk0; }` forces its value into a temp.
The temp keeps a 4-byte slot, and when two such calls are the arguments of one `bl`, MWCC evaluates them in reverse order.
Case: `TMarDirector::currentStateFinalize` loads `mCurrArea.unk1` before `unk0` for `endStageEntranceDemo`; only the getters do this.
An accessor that returns a reference leaves no trace, so you cannot prove or refute it this way.

### Array element reads

For `mArray[index]` where `mArray` is a pointer member:

| Reads in the block | Code |
|---|---|
| 1, into a named local | `lwzx r0, base, idx`, then `mr` into the argument register |
| 2 | `lwzx r4, base, idx`, one load, straight into the register that needs it |
| 3 | `add r3, base, idx` and `lwz r4, 0(r3)`: MWCC also keeps the address |

A named local gets a scratch register, but a CSE is put in the register that its user wants.
For "test it, test it against a sentinel, then pass it", use a local for the two tests and a new read for the argument.

### An address held across calls

When the target holds one element address in a callee-saved register across calls, name the **array base**, not the element:

```cpp
FabricatedSeTrackParameter* seTrackUpdate = basic->unk0->unk0;
seTrackUpdate[index].mVolume; seTrackUpdate[index].mPan; ...
```

`seTrackUpdate + index * 0x18` uses only locals, so no call can remove it.
MWCC keeps it in a compiler temp, and a temp ranks above every named local.
Case: `JAIBasic::setParameterSeqSync` went from 95.0% to 99.9%.
Do not use this when a call is between the uses of a value read from memory. That is a real local.

### A pointer local read twice in one block

`IRO_ExpressionPropagation` copies an assignment into its reader only when the variable has exactly one read in the same basic block.
A local that survives folds a member offset into the load displacement.
A local that is propagated adds the offset to the index.

```
add   rB, rBase, rIndex      ; the original read the pointer two or more times
lwz   rB, 4(rB)
```

If our build gives `addi rI, rIndex, 4` and `lwzx`, write the member access out again at the second use.
CSE merges the two loads, so the second read costs no instruction.
Case: both loops of `J3DDeformer::deform`.

### Repeated vector components: the offset-0 field

When one object gives `x`, `y` and `z` to two stores in sequence, front-end CSE makes one temp for `y` and `z` but two for `x`.
`x` is at offset 0, a bare object reference.
The second `x` temp carries the value, so `x` is coloured after `y` and `z`: `x=f2, y=f0, z=f1` where the original has `f0, f1, f2`.
Access through a pointer or reference makes `x` an offset too, but it keeps the address live and can rotate the GPRs.

A target that loads three times and stores six times proves a hidden copy that is not address-taken, for example the temp of an implicit `Vec` to `TVec3<f32>` conversion.

### Local structs stay in registers only when you touch them field by field

`IRO_ScalarizeClassDataMembers` (`IroVars.c`) gives each field access of a local struct, class or array its own temp, so it can live in a register.
An access with an unknown offset, or one that overlaps a field with a different type or size, puts the **whole** variable in memory.
A whole-struct copy (`*(Vec*)this = *(Vec*)&other`, a 12-byte struct copy) overlaps every field.
So MWCC never keeps a `TVec3` half in registers and half on the stack.

A cast between the dereference and the object reference also puts the local in memory.
After a call, MWCC must reload every field of such a local.
The signature of the ROM `TVec3` copy: `local.y` and `local.z` stay in `f31`/`f30` across calls, and `local.x` (offset 0) is loaded again.

### MWCC does not optimize data that it stores as ints

When the compiler-generated copy of a `Vec` (3 floats) runs, it uses integer loads and stores.
MWCC then cannot keep the values in float registers and puts them on the stack.

When you set the four bytes of a `Color` one by one, MWCC cannot merge them into bit operations in a register, and keeps the struct on the stack.

### By-value class parameters and copy constructors

A class passed by value is copied through memory, also for a two-word POD:

```
lwz  r4, 0(r3)        ; source.mType stays in a register
lwz  r0, 4(r3)
stw  r0, 0x38(r1)     ; source.mData
stw  r4, 0x3c(r1)     ; copy.mType
lwz  r0, 0x38(r1)
stw  r0, 0x40(r1)     ; copy.mData
lwz  r0, 0x3c(r1)     ; the copy is read back from memory
```

This shape in the target is a reliable sign of a by-value parameter (in `EventWatcher.cpp`, `getNameRefPtr(TSpcSlice)`).
A named local initialized from the same call also gives two objects, but it moves the first field through a register.

For a by-value class argument to a function that MWCC does **not** inline:

- With a **user-declared** copy constructor, MWCC builds the argument in place: one store, no copy.
- With the **implicit** copy constructor, MWCC copies through a separate stack temp: `lwz`/`stw` pairs from slot to slot.

So stack-to-stack word copies around a small value class mean that the class has **no** user-declared copy constructor.
The weak `__ct__<C>FRC<C>` symbol in the map does not prove a declared copy constructor.
Case: deleting the hand-written copy constructor of `TSingleNodeLinkList::iterator` took `TPerformList::perform` from 41.8% to 99.1%.

For `JUtility::TColor`:

| Callee parameter | Argument | Code |
|---|---|---|
| `TColor` by value | `TColor(r,g,b,a)` or a default argument | 4 `stb`, `lwz`, `stw` to the parameter, `lwz`: one copy too many |
| `const TColor&` | either | no copy, but the frame is 8-16 bytes short |
| `u32` | `TColor(r,g,b,a)` written at the call site | 4 `stb`, `lwz` into the argument register: exact |
| `u32` | `TColor(u32Member)` written at the call site | `lwz; stw; lwz`: exact |
| `u32` | a `TColor` member through `operator u32()` | one `lwz` |

`JUtility::TColor` has **no** destructor.
`TSmplFader`'s constructor takes `TColor` by value and is inlined at every site, which `CI_CanInline0` forbids for a class with a destructor.
Adding `~TColor() {}` was measured: 82 functions worse, 1 better. Do not try it again.

### Triviality: user-declared destructors

A user-declared destructor (also an empty `~T() {}`) makes a class non-trivial.
MWCC keeps non-trivial types in memory, and the property goes up through members and bases.

Two signs of a **missing** destructor (the target keeps a value in memory, ours keeps it in a register):

- The target reloads struct fields after calls, but ours keeps them in registers.
- The target writes a new `new`-ed object pointer to the stack and reloads it as `this` during an inlined constructor.
  For `new T()` with a non-trivial `T`, the object is a separate address-taken temp.

Give the smallest value type an empty `~T() {}` and measure again with a full baseline: one destructor can change many sites.
Case: `~TMsRange<f32>()` (a field of `TSmallEnemyParams`) took several `TFooManager::load` functions from about 95% to 100%.

The reverse also occurs: a destructor that the original did not have breaks constructors.
Case: removing `~JAIBasic();` (no definition, and no `__dt__8JAIBasicFv` in the map, not even UNUSED) took the `MSound` constructor from 90.22% to 99.78%.
When a base class declares a destructor that the map does not list, remove the declaration and measure.

Every destructor has a hidden `s16` delete flag.
The generated tail tests `this != 0` and `flag > 0`, and inlines `operator delete`.
An empty destructor is therefore about 0x70 bytes, not 4.
Subtract this tail before you size an UNUSED destructor body.

### Chained assignment of two locals

```cpp
f32 y = pos.y;          // lfs  fT, pos.y
f32 g = y;              // fmr  f20, fT
                        // fmr  f21, fT      <- a temp and TWO copies

f32 y;
f32 g = y = pos.y;      // lfs  f20, pos.y
                        // fmr  f21, f20     <- one copy
```

In the first form, copy propagation changes `g = y` into `g = pos.y`, so the load gets its own temp and two copies.
The chained assignment makes `g` a copy of the **variable**, which propagation does not change.
Look for a load into a callee-saved register, followed by exactly one register move, with both registers live after it.

### A local in memory is not a spill

A value in a stack slot, loaded again at each use, is almost never a spill.
MWCC spills only when it runs out of 18 callee-saved registers.
Check the `spills:` section of `variables.txt` before you believe a spill.

These put a local in memory:

- any use of its address (`&x`),
- a struct or union type (`assign_register_by_type` gives registers only to int, enum, pointer and float),
- a sub-word type `u8` or `u16` for a named local in some cases (`u32` gets a register).

A load of a memory local is still hoisted out of a loop, because a store through an unrelated pointer does not alias a local.
To keep the load in the loop, use a struct or union local.
A one-byte local whose address escapes does not stop the hoist; a 4-byte array or a struct does.

### Do not use reinterpret casts to access fields

Never access a field through a raw byte offset and a cast.
The codegen almost always differs from a real field access.
Add the field to the class, or use the existing getter or setter.

## Register allocation

### How MWCC colours registers

`allocate_local_vregs` (`CodeGen.c`) numbers the vregs in this order:

1. arguments, in parameter order,
2. named locals, in **reverse** declaration order (`CParser_NewLocalDataObject` prepends each new local),
3. front-end temps `@NNN` (a temp made later gets a lower number),
4. back-end temps,
5. the TOC list.

`simplifygraph` (`Coloring.c`) sweeps the nodes in **ascending** number.
It pushes each node whose degree is below the number of available registers (29 for GPRs: `r0`, `r3`-`r12`, `r14`-`r31`), and decrements the degree of each neighbour.
It sweeps again until a sweep pushes nothing.
`colorgraph` pops the stack, so MWCC colours the nodes in **reverse push order**.
Each node takes the lowest free volatile register, or an already used callee-saved register.
Only if none is free does it take a **new** callee-saved register, counting down from `r31`.

The rules that follow:

- **The first-declared named local gets the highest callee-saved register.**
- **Every compiler temp outranks every named local.** Arguments rank below locals.
- A local in an inner scope ranks above the locals of the outer scope.
- `r0` goes to the higher-numbered node of an interfering pair. A load or store base can never be `r0`.

When a value is coloured earlier than its number says, it was not pushed in the first sweep.
This is the "degree limit" case below.

### Local declaration order picks the callee-saved registers

When the instructions match but the callee-saved block is rotated, permute the **declaration order** of the locals, not the code.
Declaring a local earlier moves it **higher** (towards `r31`).
Do the "move each declaration to the front" sweep first. It usually finds the responsible variable in one pass.
Case: `J3DModel::calcWeightEnvelopeMtx` went from 97.2% to 100% with `u8* pScale` moved from last to after `int max`.
When several positions give 100%, choose the one that reads most naturally.

A target that holds a loop-invariant value **below** a named local tells you that the original named it, and declared it after that local.
Case: `JASystem::Driver::updateMixer` went from 99.3% to 100% with `f32 vol; f32 invPan = 1.0f - pan;`.

### Inside an inline expansion the order flips

In an inline expansion, MWCC prepends the callee's locals a second time.
So the **last**-declared local of an inlined body gets the highest register, and the highest stack slot.

| Body of the inlined helper | Registers |
|---|---|
| `s32 uVar1 = 1; JKRSolidHeap* rootHeap = heap;` | `rootHeap` r31, `uVar1` r30 |
| `JKRSolidHeap* rootHeap = heap; s32 uVar1 = 1;` | `uVar1` r31, `rootHeap` r30 |

Thus, two named locals of an inline swap places between its out-of-line copy and each inlined site.
No declaration order satisfies both.
The fix is to reduce the function to **one** local that needs a stack home.
A local whose initializer has a type conversion becomes a compiler temp:

```cpp
void* getText(u32 offset);                        // was u8*
u8* src = (u8*)mBinary->getText(mProgramCounter);  // now a temp, not a local
u32 result;                                       // the only named local left
```

Case: this took `TSpcInterp::fetchU32`, `fetchS32`, `execvar` and `execfunc` to 100% at once.
The signature: the out-of-line copy and the inline sites want the same pair of slots in opposite orders, and both frames are already the right size.

Arguments of an inlined function still rank below its locals and stay in parameter order.
A target that holds the **first** parameter above the second cannot do that with bare parameters: the original put one of them in a local.

### Bound arguments run last parameter first

`CInline_SetupArgsExpression` binds each argument that it cannot repeat safely (a call, a forced getter load, a member read of an address-taken local) to a temp.
Each new binding becomes the left operand of a comma chain, so the bindings execute **last parameter first**.
The last parameter's temp gets the highest stack slot and the highest-numbered register.
Constants, unmodified locals and reads of `const` objects are not bound: they are evaluated where the body uses them.

When the ROM evaluates two complex arguments of an inlined function in source order, the parameter list is the other way round.
Case: `JDrama::TOrthoProj`'s constructor is `(near, far, top, bottom, left, right)`, because the ROM computes `rect.getWidth()` before `rect.getHeight()`.

### The first web and later webs

When exactly **one** register differs and every other instruction matches, a value was coloured at the wrong point in the priority order.
It took a register that something else had already used, instead of a new one.

MWCC splits a variable into webs.
The **first** web keeps the variable's own low number.
**Later** webs become temps with high numbers, so they go far up the priority order.

- To lower the priority of a value, do not reuse a variable that is live before. Give it its own variable.
- To raise its priority, reuse a variable.

Neither change adds an instruction, because the extra local coalesces away.
Diagnosis: find the vreg in `backend-*-before-regalloc.txt`, and look at its neighbours.
If the vreg does **not** list the register that the target used, that register was free: this is a priority problem, not an interference problem.
Case: `J2DWindow::draw_private` became exact with a fifth `u16` for the second edge block.

### When CSE temps are born

`IRO_CommonSubs` makes the temp **at the second occurrence** of the expression, not at the first and not at the last.
An address that the code reads at the top of a function and again in the last statement is born last, gets the lowest number, and is coloured last.
The outermost matching expression wins: a repeated `*(addr)` hides the repeated `addr` under it.
A plain read of a local that can live in a register is never a CSE temp itself, and `local + constant` is not either.

To give a value a low priority, keep it a plain named local, and do not read it again in the same block before the first call.

The optimizer pass that makes a temp sets its number, not its position in the AST.
Statement order is a real lever; operand order inside one statement is not.

### The degree limit: `this` in r31 instead of r28

When the instructions match but `this` sits in `r31` instead of `r28`, the cause is the **degree** of `this`.
`this` is live for the whole function, so it interferes with every vreg and with 12 physical registers:

    deg(this) = 12 + (number of live virtual registers)

At 16 vregs the degree is 28: `this` is pushed first and coloured last, and gets `r28`.
At 17 vregs the degree is 29: `this` waits for a later sweep, is coloured first, and gets `r31`.
Everything else moves by one.

In `regalloc-gpr-pass-1-all.txt`, the signature is `this` at the **top** of the assigned list with `neighbors: 30`.

To find the cause fast, make a variant that is exactly one vreg over the limit.
Then each rewrite that removes one vreg flips `this` back to `r28` at once.

Rewrites that each remove one vreg:

1. A shared `u16` value used in two places gets a `u16` CSE temp and a redundant `rlwinm`. Cast **both** uses to `(u32)`.
2. `var = call();` makes `mr tmp, r3` then `mr var, tmp`. `var += call();` (with `var` set to 0 before) folds the copy.

The opposite lever: **a reused local adds one vreg per assignment**, because each assignment starts a new web.
Case: `JAInter::StreamLib::Play_DirectPCM`. Twelve `getDSPHandle(...)->setter()` expressions through one reused `pDVar1` put `channel` in `r31`, and took the function from 81.6% to 100%.

### Coalesced nodes raise every neighbour's degree

The sweep skips coalesced nodes.
A coalesced node is never pushed, so it never decrements its neighbours: it adds +1 to the degree of each node that it interferes with, for the whole colouring.

These make a coalesced node:

- a named local that goes straight into a call argument (`mr r4, vN` coalesces `vN` into `r4`),
- a value that a load puts in a temp and that then goes into an argument register, for example an inline getter's forced load or the bound `this` of an inline wrapper.

These make no node:

- a value that MWCC writes directly into the argument register,
- an address argument (`&this->field`), which propagation removes.

When the colouring stalls (a full sweep pushes nothing), MWCC pushes the node with the lowest `spillCost / degree`.
That node is coloured **last** and takes the **lowest** callee-saved register.
This is the only way a node gets a register below `this`.
Loop counters have a high spill cost. The jump-table base and the division magic number in a switch loop have a low cost.

### Replay the allocator when only registers differ

When a function has no opcode or order differences, only register names, the interference graph has the same shape as the target.
Only the node set can differ: one node too many or too few.
Guessing at source shapes is slow. Replay the allocator:

1. Dump the graph with the MWCC debugger.
2. Reproduce `simplifygraph` and `colorgraph` in about 90 lines of Python. Check that the replay gives our build's registers exactly.
3. Search: delete each node, un-coalesce each coalesced node, merge each pair, or add k coalesced nodes next to `this`. Compare each result with the target's registers.
4. The edits that work name the source change.

Cases:

- `JASystem::BankMgr::noteOn`: our graph had one node too many. The fix was one variable written twice (see below).
- `TDrawSyncManager::start` (98.2% to 100%): every solution was "one more neighbour of `param_2`". A named local used while `param_2` is live (`OSMessage* msgs = new OSMessage[0x14];`) fixed it.
- `CPolarSubCamera::ctrlJetCoasterCamera_` (98.3% to 99.8%): `this` needed 4 more coalesced neighbours. Real global accessors gave them: `TFlagManager::getInstance()` twice, `getConsole()`, `SMSGetMarDirector()`.

When `this` must move up to `r31`, count how many nodes are missing.
Then look for global pointer calls (`gpX->f()`, `smInstance->f()`) where a real accessor adds a node with no instruction change.

### One value written twice into one variable is one node

```cpp
chanKey |= instParam.unk3C << 0x10;   // 2 nodes: the load temp and the shift temp
u32 keymap = instParam.unk3C;         // 1 node: the local is both destinations
keymap <<= 0x10;
chanKey |= keymap;
```

`u32 keymap = instParam.unk3C << 0x10;` still makes two nodes.
The sign in the target: a load and its arithmetic in **one** register.

`X* p = call();` makes `mr tmp, r3` then `mr p, tmp`.

- If `p` is only a load or store base, propagation removes it.
- If `p` ever goes into an argument register, it keeps its node, and the return temp becomes a coalesced node.

No spelling of `X* p = call(); if (!p) ...; p->method();` reduces this. Search elsewhere.

### A local that feeds a call argument moves its computation earlier

MWCC sets up a call as `mr r3, <this>`, `mr r4, <arg1>`, and then computes an argument expression straight into its register.
The temps of that expression are live across the moves, so they can never get `r3` or `r4`.
A named local moves the computation in front of the moves.

```
    target                     ours
    mulli r3, r0, 0xc          mulli r5, r0, 0xc
    addi  r5, r3, 4            addi  r3, this, 0
    addi  r3, this, 0          addi  r5, r5, 4
```

The target uses `r3` for the multiply, so the value was a local.

An inline accessor in argument position evaluates its index before the base.
Case: `SDLModel::entryModelDataSDL`. `getShapePacket(idx)` in place of `&mShapePackets[idx]` gave the target's load order and all its callee-saved registers.
But an accessor in argument position can also move a load before the register setup: `load(..., getTextureResource())` dropped `JPAResourceManager::load` from 99.8% to 85.2%.
When the target loads the arguments in order, put the extra level on the receiver chain or inside the callee.

### Constant locals

Constant propagation puts an immediate in place of a constant local only where the operand can be an immediate:

```cpp
int emitterNum = 0x100;
new JPAEmitterManager(res, particleNum, emitterNum, emitterNum * 2, nullptr);
// li r28, 0x100 ; slwi r7, r28, 1 ; li r6, 0x100
```

An `li rN, K` next to `slwi rM, rV, n`, where `rV` also holds `K`, is **one** variable used twice.

Two counters declared once at **function scope** and set to 0 in two switch cases give `li rJ, 0` and then `addi rN, rJ, 0`.
Declared inside each case, they give two `li`.

### `mr` against `addi rD, rS, 0`

A late peephole changes `mr rD, rS` into `addi rD, rS, 0` only when the copy has an instruction before it in its basic block.
A `mr` that is first in its block stays `mr`.
The scheduler moves it after that, so its final position tells you nothing.

An isolated `mr`/`addi` difference often comes from a dead instruction that took part in scheduling.
Case: `JKRAram.cpp` `firstSrcData` (93.3% to 100%). The forwarding inline returned `BOOL`, but its callee returned `bool`.
The dead conversion displaced the copy.
Compare a forwarding inline's return type with its callee's: `BOOL`/`bool`, `int`/enum, `u32`/`s32`.

A redundant cast is not free either.
`((u16)x & 0xFFF0) + 0x10` emits a dead `rlwinm` that the scheduler sees before MWCC removes it, and that changes the registers.
Write the narrower mask alone.

### A move between two locals never coalesces

`coalescenodes()` (`InterferenceGraph.c`) coalesces a move between two vregs only when both are temps.
A move between two named locals, or between a local and a front-end temp, always stays.
A move into a real register (`return local;`) coalesces unless the two interfere.

So a dead `fmr f1, f0` before the epilogue means **two** variables: an inner variable feeds an outer one, and the function returns the outer one.
Copy propagation keeps `outer = inner` only when the outer variable has more than one definition, which means a single exit that other arms also write.

```cpp
f32 result;                       // outer: 3 definitions, one exit
if (...) {
    f32 pan;                      // inner
    if (...) pan = 0.5f; else if (...) pan = ...; else pan = ...;
    result = pan;                 // survives -> fmr f1, f0
} else if (...) {
    result = unk4 & 1;
} else {
    result = 0.5f;
}
return result;
```

A `static inline` helper with the same chain also gives 100%, because an inlined multi-return function also gives the caller a temp.
When a helper and a plain local both match, prefer the form that invents no symbol.

### Reuse the destination as the carrier

An inline with **one** `return` gives the caller its local, and that coalesces with the destination.
An inline with **several** returns (a ternary chain, a clamp) gives a new temp.
When the last of a series of inlines has several returns and the function ends with a dead `fmr f1, f0`, thread every step through the destination:

```cpp
result = linearTransform(x, xStart, xEnd, 0.0f, y, true);
result = expf(result);
result = linearTransform(result, 1.0f, expf(y), yStart, yEnd, true);
```

Case: `JALCalc::getParamByExp` went from 98.2% to 100%.

### `int` against `s32` for a float conversion

`int` and `s32` (`signed long`) are both 32 bits, but MWCC treats them differently:

```cpp
int iVar1 = (param_1 + 1.0f) * 50.0f;   // fctiwz; stfd; lwz; then stfd; lwz again
s32 iVar1 = (param_1 + 1.0f) * 50.0f;   // fctiwz; stfd; lwz only once
```

With `int`, MWCC makes a second stack temp and converts again for later uses (and the frame grows by 8).
Two `stfd`/`lwz` pairs from the same `f0` in the target mean `int`. One pair means `s32` or `long`.
Case: `MSHandle::MSACos` went from 91.2% to 100%.

### Reference locals

A reference local before member accesses can change register allocation:

```cpp
unk0[i].mPos.set(...);           // the base can be loaded each time
unk0[i].mVel.set(0.0f, 0.0f, 0.0f);

Node& node = unk0[i];            // the base stays in a register
node.mPos.set(...);
node.mVel.set(0.0f, 0.0f, 0.0f);
```

Use a reference local only when:

1. the target clearly loads the base pointer once and uses it again,
2. direct access gives redundant loads that the target does not have, or
3. the diff makes you confident that the original had one.

In other cases, use direct member access or getter calls, and let CSE do the rest.
Reference aliases used as a general method often damage offsets and registers.

### An inlined function loads `this` once

When a simple version loads a pointer many times and the original loads it once, the original can have used an inline:

```cpp
field->nonInlinedFuncCall1();     // `field` can be loaded twice:
field->nonInlinedFuncCall2();     // the compiler assumes that calls change memory

field->inlineFuncThatCallsBoth(); // `field` loaded once, as `this`
```

This can look the same as a reference local.

### Intermediate variables

Intermediate variables are a strong lever on register pressure, load and store timing, and scheduling.
Start with the simplest form and change it step by step:

1. Try a fully inline arithmetic form (no intermediate variables).
2. Try forms with temporaries.
3. Change the grouping and the order, also with compound assignments.

```cpp
f32 c0 = (z1 - z0) * (x2 - x1) - (x1 - x0) * (z2 - z1);

f32 dx = x1 - x0;
f32 dz = z1 - z0;
f32 c0 = dz * (x2 - x1) - dx * (z2 - z1);

f32 c0 = (z1 - z0) * (x2 - x1);
c0 -= (x1 - x0) * (z2 - z1);
```

Try forms that a human would write first.

A named cast local, `T* x = static_cast<T*>(search(...)); x->f();`, is a common style in this codebase.
It often fixes registers where a call on the cast does not.

## The stack frame

### Frame arithmetic

```
frame = round8( 8 + round8(locals) + 4 × saved GPRs + 8 × saved FPRs )
```

The saved registers are at the top: `stmw rN, (frame - 4×(32-N))(r1)`.
A 4-byte gap between the highest local and the `stmw` offset is rounding, not an object. Do not search for a declaration to fill it.

Because of the rounding, one 4-byte slot can change the frame by 8 or by 0.
Thus, never judge one lever alone. Count the slots in `variables.txt`, or use `mwcc_izer.py stack`.

**Read the direction first.**
Compare local offsets from the **top** of the frame, not from `r1`.
If the saved registers and the conversion temp are at the same distance from the top in both builds, the difference is **below** all named storage: dead temp slots.

- A frame that is too **small** (a deficit) usually means that the original had more inline expansions or deeper inline chains at the same instructions. It rarely means a missing local.
- A frame that is too **big** (a surplus) means fewer inline expansions.

### Frame layout

- `assign_locals_to_memory` places slots in size passes (1, 2, 4, ... bytes), each pass in list order.
- Earlier declaration means a higher address. The first memory local is just under the conversion-temp pool, which is just under the saved registers.
- The int-to-float conversion temp is 8 bytes, 8-byte aligned, above every named local.
- The function's own parse-time temps are at the top. Below them are the inline expansions in creation order.
- Each inline expansion's locals are in reverse declaration order. Use the ROM offsets to read the declaration order of an inlined helper's locals.
- Removing one temp lowers everything above it. The frame and the offset of an early temp move together, so the pair is a second constraint.

### What reserves a slot

MWCC reserves slots for objects that the final code never touches.
Each temp keeps its slot, also when the optimizer removes every instruction that used it.

An inline expansion makes one temp for each of these:

- the **receiver**, when it is an expression (`pArray[i]->getName()`). A call on the function's own `this` makes none. Two calls on the same object share one receiver temp.
- each **argument that is an expression**. A plain variable argument makes none. The temp has the size of the parameter type (a `u16` parameter temp is 2 bytes).
- the **return value**, when the function returns a value. A `void` inline makes none.
- each **named local in the inlined body**.

Per site: `4 × (expression receiver) + sum(sizeof each expression argument) + 4 × (named locals) + 4 × (non-void return)`.
Count it before you guess a spelling.

| Inlined call | Dead 4-byte slots |
|---|---|
| `mFoo.get()`, `mFoo` at **offset 0** | 0 |
| `mFoo.get()`, `mFoo` an embedded member at a non-zero offset | 2 |
| `mPtr->get()`, `mPtr` a pointer member | 3 |
| any void setter | 0 |
| free or namespace function that returns a global or a computed value | 1 |
| free function that returns `void` or a compile-time constant | 0 |
| `obj->get()` on a **local** pointer or object | 0 |
| static member function that computes from its argument | 0 |
| static member function that returns a global | 1 |

The return temp survives only a **bare** discarded call:

| Statement | Slots |
|---|---|
| `(void)f();`, `(void)(f());`, `(f()) ? (void)0 : (void)0;` | 1 |
| `(void)(x < f());`, `(x < f()) ? (void)0 : (void)0;` | 0 |
| `(void)0;` | 0 |

So a release-stripped assert reserves a slot only if its condition is a bare call to such an inline.

In a chain, an accessor reserves frame only when its result is the receiver of **another inlined call**.
The last link of a chain is free.
An accessor whose result goes into an out-of-line `bl` is free.
Raw field access through the same pointers costs nothing.

| `changeBck` expression | Frame |
|---|---|
| guard only | 0x18 |
| `getActorKeeper()` | 0x20 |
| `...->getMActorAnmData()` | 0x28 |
| `...->getUnk2C()` | 0x38 |
| `...->getAnmPtr(index)` | 0x38 |
| the same chain as raw fields | 0x18 |

A few more costs:

- `getList()->getFirst()` costs 2 temps. `JSUList<T>* list = &mList; list->getFirst()` costs 1.
- `T* x = f(); x->g();` inside an inline body costs 2 temps per site, `f()->g();` costs 1.
- `JGadget::TList<T>::iterator::operator++(int)` costs 3 temps per site; `++it` costs none. When a list loop is short by 0xC, try `it++`.
- A ternary can reserve more stack than the same logic written with `if`.
- A struct local reserves its slot, also when it lives in registers.
- An array local in a block reserves its slot, also when every index is constant.
- A redundant cast on a pointer that already has that type moves every offset below it by 4.
- Each int-to-float conversion **site** reserves 8 bytes. A conversion of a constant folds and reserves nothing.
- A constant local that MWCC folds at every use gets no register, so its slot stays. An `int` used in both integer and float contexts can cost two.
- An **unused** local keeps its slot. A used local of 4 bytes or less gets a register and no slot.
- A sub-word named local (`u8`, sometimes `u16`) can go to memory at the top of the frame and cost 8 bytes. `u32`, `int` and `s32` get a register.
- IRO makes temps when it linearizes the AST (`IroLinearForm.c`) and deletes them later. Their slots stay at the bottom of the frame.

### `T x = f();` costs one more object than `T x; x = f();`

When `f` is inlined, `T x = f();` builds the return value in its own temp and copies it into `x`: two objects.
Declare-then-assign puts the return value straight in `x`: one object.
No instruction changes, only the frame.

- Frame one object too big: look for an initialization to split.
- Frame one object too small: look for a call result that should have a name.

Case: `TSpcInterp::update` was 8 bytes too big with an inlined 12-byte pointer to member. Declare-then-assign gave 100%.
Case: `TSpcBinary::init` was 8 bytes too small. `u32 hash = calcKey(...); symbol->mNameHash = hash;` gave 100%.

### A local can remove a bound temp

`CInline_SetupArgsExpression` binds an argument that is unsafe to repeat (a call always is).
A read of an unmodified local is safe, so MWCC substitutes it and makes no temp:

```cpp
interp->push((int)interp->pop().typeof());   // binds a temp for the argument
u32 type = interp->pop().typeof();           // no temp
interp->push((int)type);
```

Case: `spcTypeof` went from 96.3% to 100%.
This helps only when the new local gets a register.
If it needs a slot, it moves every offset above it. The same change dropped `execadd` from 100% to 96.6%.

A call in argument position reserves no slot.
A call split into its own statement that assigns a carrier variable adds 4 bytes per split.

### Named locals inside proven helpers

A plain function (namespace scope, or a member that takes everything as arguments) reserves **nothing** when inlined, unless its body puts a value in a named local.
Then it costs one slot at each expansion.

Case: `JAISystemInterface::setSePortParameter` was 6 slots short. It expands the UNUSED `setPortParameter` six times.
`TOuterParam* outer = param_2->getOuterParam(); outer->setParam(...)` in the helper gave exactly 6.
Case: `JAIBasic::loadDVDFile` was 8 bytes short. `allocHeap` with `void* result; if (...) result = ...; else result = ...; return result;` closed it.

This is not a fabricated pad: the local is used, and the map proves the helper.
An UNUSED map symbol just before the failing function is the strongest hint for the helper.

The 15-statement gate and the caller's frame pull in opposite directions.
An initialized declaration is one statement and one caller temp.
When a callee sits at exactly 15 statements, a new local must be paid for with a statement that you remove.

### Measuring a deficit

Measure a deficit before you guess a fix.

1. Count the missing slots: `mwcc_izer.py stack`, or diff `variables.txt` against the ROM offsets.
2. Calibrate with a probe. `(void)SomeNs::getGlobal();` reserves one slot and emits no instruction. Insert N of them, and find the N whose `stwu` is the target's.
3. Line up every `r1` access of both sides. The gaps show how many bytes are missing and where.
4. `volatile f32 d[2];` at a candidate point confirms a size and a position.

Clear every pad and probe out of a function **before** you measure anything else in it.
A pad sets the frame, so the next candidate competes with it and measures worse than it is.
Case: with three dead locals in `MSBgm::stopBGM`, `JAISound::getTrack()` measured 99.88%; without them it measured 100%.

Never commit a probe, a pad or a `volatile char trash[N]`.
See the rules on frame deficits in [AGENTS.md](../AGENTS.md#fakematches-important).

### Legitimate levers for a frame difference

Each of these is ordinary source that a person writes.
Use one only when something more than the frame supports it (an instruction change, a sibling function, a reference, the map).

- A real inline accessor that the rest of the codebase uses (for example `SMSGetMarDirector()->getConsole()`), applied at every site.
- A named local for a chain result that is the receiver of the next call.
- A proven helper with a named local in its body.
- `it++` in place of `++it` on a JGadget list iterator.
- Declare-then-assign, or the reverse.
- Constant locals that the original evidently had (for example a division count).
- Default constructor arguments in place of written-out values.

Case (`System/EventWatcher.cpp` `evManiCoinDown`): `gpMarDirector->getConsole()->startAppearStar()` gave a frame of 0x28.
The accessor gave 0x30 with the wrong layout. The accessor plus a `console` local gave 100%.
In the same file, `evSetNextStage` is worse with the accessor, so the original used the bare global there.

A signed loop counter against an unsigned bound changes the unrolled shape of a small helper.
If that helper is inlined, the change moves the caller's frame too.
So a frame difference in a caller can be a signedness bug in a helper's loop.

## Vectors and math

### Use the `TVec3` inlines

`JGeometry::TVec3<f32>` and the other `JGeometry` types have many inlines, and the original code used them most of the time.
When plain math does not match, try the inlines.

### Multiplication by zero or one

MWCC removes `1.0f * x` and `0.0f * x` that the source writes, but not when they come from an inlined function.
This is common when the code builds an "up" vector and uses `cross`.
Write `1.0f * unk170.z - 0.0f * unk170.y` as a real up vector and a `cross` call.

### Constructor against component assignment

```cpp
JGeometry::TVec3<f32> pos(x, y, z);  // all loads, then all stores

JGeometry::TVec3<f32> pos;           // load/store pairs, one component at a time
pos.x = x;
pos.y = y;
pos.z = z;
```

`v.set(a, b, c)` computes all three arguments first (`z` first), then stores x, y, z.
Separate assignments store each component before the next is computed.
A store of `.x` before the code that computes `.y` means separate assignments.

When the components share a factor in an object that the computation also writes (for example random draws on the emitter that owns the seed), separate assignments load the factor again each time.
One load means `set(getRandomSF(), getRandomSF(), getRandomSF())` and then `scale(unk204)`.

### `zero()` against `set(0, 0, 0)`

`.zero()` stores in **reverse** order (`z`, `y`, `x`). `.set(0, 0, 0)` stores in forward order.

### `operator=` against `set()`

```cpp
node.mPos = param_1;          // lwz/stw (word copy)
node.mPos.set(param_1);       // lfs/stfs (float copy)
node.mPos.set(ex, ey, ez);    // lfs/stfs, like component assignment
```

The `TVec3` copy constructor is `TVec3(const TVec3& b) : Vec(b) { }`, and the ROM's weak copies are 0x1C of `lwz`/`stw`.
Any new `TVec3` copy theory must keep the weak `set<f>`, `__ami__` and `TSplinePath::getPoint` out of line.

### A `Vec` passed as `const TVec3<f32>&`

A `Vec` expression bound to a `const JGeometry::TVec3<f32>&` parameter makes a temp through `TVec3(const Vec&) { set(b); }`.
If the conversion is inside an inline body, that `set(const Vec&)` stays a `bl set__Q29JGeometry8TVec3<f>FRC3Vec`.
If it is written directly in the function, `set` expands.
So a single `bl` to that weak `set` means "a `Vec` went into a `TVec3` reference, one inline level down".
Each such conversion is also a 12-byte frame temp.

### Fused multiply-add is per expression

`a + b * c` in one expression gives `fmadds`.
The same product in its own statement gives `fmuls` and `fadds`.
Inlined `TVec3` operations (`sub`, `scale`, `add`) are separate statements.
So a ROM with no `fmadds` where the math is clearly `a + b*c` used vector operations or split statements.

### `fnmsubs`

Code like `B - A * C` or `A * C - B` compiles to `fmsubs` or `fnmsubs`.
`fnmsubs` is technically `-(A * C - B)`, but MWCC uses it for `B - A * C`.
Ghidra and m2c write `-(a * b - c * d)`. Rewrite it as `c * d - a * b`.

### Zero components and `+=`

When the target adds a literal `0.0` to some components (`lfs f4, @zero; fadds f4, f4, f0`), the original built a vector with literal zeros and then added another vector:

```cpp
JGeometry::TVec3<f32> point2(0.0f, -1.0f * unkC.y, 0.0f);
point2 += data.unk58;
```

### `-1.0f * x` against `-x`

`-x` gives `fneg`.
A separate `fmuls` by the `-1.0` constant (not fused into a following `fmadds`) means that the source wrote `-1.0f * x`.
Case: `BathWaterManager.cpp` `calcBathtub`.

### Literal order shows which constants came from inlines

With `-inline deferred`, the literals inside an inline body are created at expansion time, after every literal of the caller's own text.
If the target creates a constant before a literal that the source clearly parses first, that constant is in the source text, not behind an inline.
A deferred inline call is also evaluated after the plain operand: `PI() * x` gives `fmuls fD, fx, fpi`, and `3.14159f * x` gives `fmuls fD, fpi, fx`.
Case: `TCylinder::makeDL` went from 98.4% to 99.5% with pi as a plain literal.

### `MsMin`/`MsMax` and `.sdata` floats

MWCC puts float literals in `.sdata2` (read-only).
A float constant in `.sdata` (writable) is a temp made for a `const T&` parameter.
A `.sdata` pair such as (1, -1) or (3, -3) is the clamp idiom:

```cpp
if (v > 0.0f)
    result = MsMin(v, hi);
else
    result = MsMax(v, lo);
```

`MsMin`/`MsMax` (in `MarioUtil/MathUtil.hpp`) take `const T&`.
The return value is copied into the variable with one more `fmr` per arm, which a plain ternary does not do.
A TU with no `.sdata` section cannot use them with literal arguments.
`MsClamp` takes its floats by value (the map gives `MsClamp<f>__Ffff`).

## Loops

### The array index type changes strength reduction

`-opt nostrength` does **not** stop strength reduction in the back end.
`array[i]` in a loop normally becomes an induction variable (`addi rN, rN, sizeof(T)`) and `lwzx`.
`isinductionvariable()` accepts only `mulli`, a pure left shift, and indexed loads and stores.
A `clrlwi` from a `u16` index breaks it, and the `mulli` stays in the loop.

Symptom: the target has `li rX, 0`, `addi rX, rX, <sizeof>` and `lwzx`, and our build has `clrlwi` and `mulli`.
Fix: reach the element without a `u16` index.

A signed counter against an unsigned bound (`int i < u32 size`) gives a shorter unrolled body than `u32 i`.

### Induction variable copy (open problem)

In some ROM loops, the derived induction variable starts as a copy of the counter (`addi r30, r29, 0` after `li r29, 0`).
Our build starts it with the constant (`li r30, 0`).
`initializeinductionvariable` emits the copy only for indexed loads and stores whose base or index is the counter.
First, check whether the loop is an inlined helper. Case: `JASystem::Dvd::updateBuffer` was the inlined `writeBufferSize`.

### Pointer advances

Two pointer advances usually merge into one `addi rX, rX, 2`.
`adjustforward()` (`Peephole.c`) moves an `addi rX, rX, imm` forward in its block, into the displacement of the next memory access through `rX`.
It stops at a store of `rX`, a branch, a call, or an indexed access through `rX`.
`*p++` twice, `*p = a; p++; *p = b; p++;`, and `p[0]/p[1]` plus `p += 2` all merge.

To keep two separate `addi 1`, put a cast on the dereference: `status->stickX = *(s8*)data++;`.
The update forms (`lbzu`/`stbu`) come from a later peephole.

### Pointer add operand order

For an address, MWCC emits `add rD, rA, rB`, and `rA` shows the domain of the arithmetic:

- Pointer arithmetic normalizes: `off + p`, `p + off`, `p[n]` all give `add rD, rBase, rOffset`.
- Integer arithmetic does not: `off + (uintptr_t)p` gives `add rD, rOffset, rBase`.
- A constant displacement with the index in `rA`: `*(u16*)&p[i * 4 + 6]`.
- `&p[c + i]` keeps `addi rD, rD, c` in the loop with the index first. `p + i + c` puts the base first, but MWCC hoists `p + c`.
- `p + i` with no constant, and `p += i`, keep the source order.

When the target holds a pointer in `r0`, name the byte offset and add the base into the same variable:

```cpp
uintptr_t entry = bank_id * sizeof(FabricatedWaveBankEntry);
entry           = (uintptr_t)mWaveBankList + entry;
```

### Constant hoisting

MWCC hoists constant loads (`lfs`, `lfd` from SDA) out of loops.
The set of hoisted constants depends on the expressions in the loop, the inlines it calls, and the order of operations.
If the target hoists a constant and our build does not, the loop body or an inline in it has a different structure.
Do not try to move the load.

### Local arrays and unrolled loops

MWCC can put the elements of a small local array (`f32 dist[3]`) or struct in registers and reorder them.
When the target keeps the array on the stack (consecutive `stfs`/`lfs` in a strict order), write the fill as a small `for` loop that MWCC unrolls:

```cpp
f32 dist[3];
for (int i = 0; i < 3; ++i)
    dist[i] = unk24[i];
```

`(void)&arr;` also works, but it is a fake artifact. Use it only to find the other problems first.
Case: `DrawUtil.cpp` `TSilhouette::loadAfter` went to 99.7% with no address-of hacks.

## Casts, types and signedness

### Narrowing casts are a hint

A `clrlwi` or `extsh` before a use rarely means that the original had a cast.
Look first for a parameter, a return value, a local or a member type that narrows the value.

### Signedness and width show in the compare

`cmpwi` against `cmplwi`, and `extsb`/`extsh` against `clrlwi`, give the sign and the width of a value.
Fix the declared type. Do not add casts.

### Common store idioms

`u8` to `f32` (for example from a `GXColor`):

```
lbz   r0, OFF(rBase)
lis   r4, 0x4330
lfd   f1, "@magic"@sda21(r0)      ; 4503599627370496.0
stw   r0, X(r1)
stw   r4, X-4(r1)
lfd   f0, X-4(r1)
fsubs f0, f0, f1
stfs  f0, FIELD(rThis)
```

Source: `unk84.x = (f32)bodyColor[0].r;`.

`(u8)` on an `int` return before a store to a `u16` field:

```
bl    JUTNameTab::getIndex(...)
clrlwi r0, r3, 24
sth    r0, FIELD(rThis)
```

Source: `mField = (u8)getIndex(...);`. Without the cast, MWCC emits `sth r3` directly.

### Casts of pointer variables are free

`(const u8*)(const void*)param_1` and `(JKRHeap*)unk0` change nothing.
For a cast between a pointer and an integer, always use `uintptr_t`, never `u32`. On PowerPC this changes no byte.

## Data, sections and symbols

### Sections

- An array with an explicit size of 8 bytes or less can go to `.sdata` (`@sda21`, `lwzx`). An **unsized** array (`fileNames[]`) always goes to `.data` (`lis`/`addi`). Case: `BathWaterManager.cpp` `fileNames[]`.
- A static member whose type is its own class (`H8 H8::probeSelf`) has no size when MWCC reads the declaration, so it goes to `.bss`, not `.sbss`. Use a nested class type. The mangled name does not change.
- A constructor does not stop small-data placement.
- An anonymous aggregate literal from a header inline compiled **in the precompiled header** goes to `.data`. The same literal in a `.cpp` goes to `.rodata`. When the target reads a constant from `.data` and ours reads it from `.rodata`, the original called the header inline. A high `@NNNN` number also shows a PCH object.
- The const-ness of a global changes its section.

Section size check: `original section size = linked size in the map + sum of UNUSED sizes in that section`.
Compare it with `powerpc-eabi-objdump -h` of our object.
An extra `.rodata` literal moves every later offset (MWCC addresses `.rodata` as `base + immediate`). An extra dead `.data` object moves nothing.

### Function-local statics: `init$NNN` is the guard

A function-local `static` with a non-trivial initializer (for example `static JGeometry::TVec3<f32> pos(1815.0f, 1500.0f, 1550.0f);`) makes two `.sbss` symbols:

- the variable (`pos$NNN`),
- a 1-byte guard `init$NNN` (the name is always `init`).

```asm
lbz   r0, init$NNN@sda21
extsb. r0, r0
bne   .L_skip_init
... initializer code ...
li    r0, 1
stb   r0, init$NNN@sda21
.L_skip_init:
```

Do not write the guard as a `static bool`.
It is the guard of another static with a real constructor.
Aggregate-initialized PODs and zero-initialized statics have no guard.

Statics inside an inline function (also a member function defined in the class body) get `<name>$localstatic<N>$<mangled function>`.
`<name>` is the developer's own variable name.

### Static arrays of pointers to member functions

MWCC does not emit such an array as data.
It emits one 12-byte constant `@NNN` per non-null element, and `__sinit` copies each one into the array with `lwz`/`stw`.
Null elements come from `__ptmf_null`.
The map's closure lists each `@NNN` with the function it points to. Pair them with the `__sinit` copies to recover the table.
Track the loads per destination register, not as a queue.

### Mangling

- A symbol in an anonymous namespace has an `@unnamed@` prefix. A local symbol without it is a plain `static`.
- A namespace-scope function mangles like a static member of a class with that name. `(func,local)` with a qualified name in the map is a `static` function in a namespace (a class member is never local), or a function template instance. Write it in a `namespace X { ... }` block.
- A local class of a member function mangles as `Q2<class><name>$<uniqueid><file>_cpp`.

### The unique-ID counter

MWCC names literals, labels, temps and local classes from one shared counter.
The gap between two `@NNN` numbers counts the entities that the source makes between their first uses.
This measures things that make no instruction.
Read the target numbers in `build/GMSJ01/asm/<unit>.s` and ours with `nm`, and align them by order, not by value.

| Construct | IDs |
|---|---|
| an extra inline level on a call | 1 per site |
| a named pointer local in place of a repeated `p->` | 1 |
| `if (a) { if (b) ... }` in place of `if (a && b)` | 1 more |
| `if (c) ...;` | 2 |
| each extra `&&`/`\|\|` term | +1 |
| `if`/`else` | 3 |
| `while` | 4 |
| `for` | 4 at the head, +1 at the closing brace |
| an anonymous `enum` block in a class | 1 |
| plain statements, calls, float and string literals | 0 |

A header inline costs 0 at the call site.
Use the counter to choose between spellings that give the same instructions, but several sources can give the same count.

### Templates

Every implicitly instantiated member of a class template is **weak**.
A TU that can see a member's body emits a weak copy. A TU that sees only the declaration emits an external reference.
An extra weak copy in an early TU takes the symbol away from the TU that the ROM got it from.
Put such bodies in a `<Header>.tpp` file and include it only from the TUs that the map shows as owners.

An explicit instantiation (`template class TViewObjPtrListT<TViewObj, TViewObj>;`) makes the members **global** and sets their emission order.
MWCC 1.2.5 does not accept a qualified name in the directive: wrap it in the namespace.
Add these directives one at a time and measure each.
MWCC 1.2.5 has no `implicit_templates` pragma.

### An `asm` function turns off the peephole

A whole-function `asm` definition turns off both peephole passes for every function **after** it in the TU.
An `asm { }` block inside a C++ function does not.
Signature: a group of functions at the end of a TU with no record-form instructions (`rlwinm rX; cmpwi rX, 0` in place of `rlwinm.`) and no `lbzu`/`stbu`.

## Recognizing source patterns from raw asm

### Initializer-heavy functions

For a function that is mostly field assignments (`init()`, constructors), read the raw `.s` file:

1. Find the function in `build/GMSJ01/asm/<path>.s` by its mangled name (`.fn <mangled>`).
2. Map each `stb`/`sth`/`stw`/`stfs` to a field with the `/* 0xNN */` offsets in the header. Each store is one statement.
3. Float and integer constants are at the end of the same file as `.obj "@NNNN"` blocks, referenced as `lfs f0, "@NNNN"@sda21(r0)`.
4. String literals are in `.rodata`/`.sdata2`, referenced as `addi r4, r30, 0xNNN` from the TU's rodata base.
5. Compare the offsets one by one with the source. Anything in the asm but not in the source is missing.

### `JSUMemoryInputStream`

Game code mostly uses the `operator>>` overloads, sometimes chained (`stream >> vec.x >> vec.y >> vec.z;`).
The frame of the `JDrama::TNameRef::load` overloads matches only with these forms.
`stream.readU16()` also occurs. Choose by codegen at each site.
A raw `stream.read(&someInt, 4);` is the least likely form.

## Known accessor verdicts

These results come from sweeps over the whole game with `ninja baseline` and `ninja changes_all`.
Do not repeat a refuted sweep without new evidence.

| Accessor | Verdict |
|---|---|
| `TFlagManager::getInstance()`, `SMSGetMarDirector()`, `TMarDirector::getConsole()` | Real. Use them outside the owning classes. A few sites read the raw global because the ROM shows it. |
| `SMSGetMarDirector()->getGamePad(N)` | Real (proven by `TCardSave::load`). |
| `SMSGetMarioParticleManager()` | Refuted: 77 functions worse, 1 better. Use raw `gpMarioParticleManager`. |
| `SMSGetMap()` | Refuted: 51 worse, 7 better. Use raw `gpMap`. |
| `TPlacement::getPosition()` | Fake. Thousands of matching functions read `mPosition` raw. Do not use it to close a frame deficit. |
| `TLiveActor::getVelocity()`, `getModel()` | Mixed use is real. Choose the getter or the raw field at each site by codegen, if no other function gets worse. |
| `TTakeActor::getHolder()` | Use it only where a protected `mHolder` would not compile: reads through another object. Own reads stay raw `mHolder`. |
| `J3DModelData::getVtxPosArray()` | Real. |
| `TParamVec::set` | Refuted at all 14 sites. |
| `JUtility::TColor` destructor | Refuted (see above). |
| a `JAIHeapBlock` accessor set from TWW | Refuted: SMS reads those fields raw. |

The "protected field" test: imagine that a field is `protected`.
Use its getter only where the compiler would refuse the raw access: reads through another object, `self->` in nerves, globals like `gpMarioOriginal`.
A write through another object would not compile either: that is a sign of a missing setter-like inline.
