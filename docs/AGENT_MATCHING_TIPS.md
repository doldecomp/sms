# MWCC Matching Tips for Agents

This document collects practical knowledge about how the Metrowerks CodeWarrior C++ compiler (MWCC GC/1.2.5) generates PowerPC code. These tips help when iterating on source code to achieve a byte-identical match against the original binary.

Read this document before attempting to match functions. The patterns described here are recurring and will save significant trial-and-error time. Expand the document as needed. Ask for human review whenever the contents of the document disagree with observed reality.

## TVec3 / Vector Codegen Patterns

`JGeometry::TVec3<f32>` is a 12-byte struct with `x`, `y`, `z` float members. How you read/write it drastically affects code generation.

### Construction: component-by-component vs constructor

```cpp
// Constructor form — compiler batches all loads, then all stores:
//   lfs f0, ...; lfs f1, ...; lfs f2, ...
//   stfs f0, 0(rN); stfs f1, 4(rN); stfs f2, 8(rN)
JGeometry::TVec3<f32> pos(x, y, z);

// Component-by-component — compiler interleaves load/store pairs:
//   lfs f0, ...; stfs f0, 0(rN)
//   lfs f0, ...; stfs f0, 4(rN)
//   lfs f0, ...; stfs f0, 8(rN)
JGeometry::TVec3<f32> pos;
pos.x = x;
pos.y = y;
pos.z = z;
```

Check the target assembly to see which pattern (batched vs interleaved) is used, and write the source accordingly.

### Assignment: `operator=` vs `.set()`

```cpp
// operator= (struct copy) — generates lwz/stw (word load/store):
//   lwz r0, 0(rSrc); stw r0, 0(rDst)
//   lwz r0, 4(rSrc); stw r0, 4(rDst)
//   lwz r0, 8(rSrc); stw r0, 8(rDst)
node.mPos = param_1;

// .set(vec) (float copy) — generates lfs/stfs (float load/store):
//   lfs f0, 0(rSrc); stfs f0, 0(rDst)
//   lfs f0, 4(rSrc); stfs f0, 4(rDst)
//   lfs f0, 8(rSrc); stfs f0, 8(rDst)
node.mPos.set(param_1);

// .set(x, y, z) (3-arg form) — generates lfs/stfs like component assignment
node.mPos.set(expr_x, expr_y, expr_z);
```

The target assembly will clearly show `lwz`/`stw` (integer move) vs `lfs`/`stfs` (float move). Choose the source pattern that matches.

## Reference Locals Affect Register Allocation

Introducing a reference local before accessing struct members can change how the compiler allocates registers:

```cpp
// Direct access — compiler may reload base pointer each time:
unk0[i].mPos.set(...);
unk0[i].mVel.set(0.0f, 0.0f, 0.0f);

// Reference local — compiler keeps base in a register:
Node& node = unk0[i];
node.mPos.set(...);
node.mVel.set(0.0f, 0.0f, 0.0f);
```

Check whether the target reloads the base address or reuses a register. If it reuses, a reference local is likely in the original code.

## Inlined Functions Only Load `this` Once

Whenever a simple implementation loads a pointer into a register multiple times but the original did it once — the original might have used a function that got inlined.

```cpp
// `field` pointer could get loaded twice because
// the compiler assumes function calls can modify any memory
field->nonInlinedFuncCall1();
field->nonInlinedFuncCall2();

// `field` pointer loaded as `this` once
field->inlineFuncThatCallsBoth();

...

class FieldClass {
...
  void inlineFuncThatCallsBoth() {
    nonInlinedFuncCall1();
    nonInlinedFuncCall2();
  }
...
};
```

This is sometimes indistinguishable from a local reference being used.

## Constant Hoisting and Loop Codegen

The compiler hoists constant loads (`lfs`, `lfd` from SDA/SDA2) before loops. The exact set of constants hoisted depends on:
- Which expressions appear in the loop body
- Which inline functions are called (they may reference additional constants)
- The order of operations within the loop

If the target hoists a constant (e.g., `lfd f28, @5181@sda21`) before a loop but our build does not, it means the compiler sees a different code structure. This is usually a symptom of a deeper structural mismatch in the loop body or inlined functions, not fixable by just moving the load.

## Local Symbol Mangling: `@unnamed@` vs `static`

MWCC mangles symbols inside anonymous namespaces with an `@unnamed@` prefix.

- If a local symbol's mangled name includes `@unnamed@`, model it as being in an anonymous namespace.
- If the symbol is local but does not include `@unnamed@`, prefer a plain `static` function/variable instead.

## Symbol Order with `-inline deferred`

When a TU is compiled with `-inline deferred` (see TU-specific flags in `configure.py`), define symbols in reverse order relative to the map/symbol listing for that TU.

- In practice, function-definition order in the `.cpp` should be reversed for those TUs.
- If order-sensitive matching drifts for an `-inline deferred` TU, verify definition order before attempting smaller codegen tweaks.
