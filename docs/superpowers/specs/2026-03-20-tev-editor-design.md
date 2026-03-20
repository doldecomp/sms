# TEV Editor Panel — Design Spec

**Date:** 2026-03-20
**Project:** J3DBlend (blemd) Blender Addon
**Goal:** Read-only Blender Properties panel displaying GX TEV stage configuration for BMD materials

---

## Overview

A Blender Properties panel that visualizes the GX TEV (Texture Environment) combiner pipeline for materials imported from BMD files. Auto-opens on BMD import. Read-only initially, with editing added as a follow-up.

---

## UI Location

Properties sidebar > Material tab, below Blender's existing material panels. The panel is only visible when the active material has `gc_mat_*` custom properties (set during BMD import).

Panel class: `MATERIAL_PT_gx_tev` registered via `bpy.types.Panel`.

---

## Panel Layout

### Header Section
- Material name
- Number of active TEV stages (from `gc_mat_tevStageNum`)
- Cull mode (from `gc_mat_cullMode`)
- Alpha compare summary (ref0, op, ref1)

### Per-Stage List (Expandable Rows)
For each active TEV stage (0 to `tevStageNum - 1`), a collapsible sub-panel showing:

**TEV Order:**
- Texture index (`gc_mat_tevOrder_{n}_texIdx`) — which TEX1 texture
- Texcoord index (`gc_mat_tevOrder_{n}_texCoordIdx`) — which UV channel
- Channel ID (`gc_mat_tevOrder_{n}_chanId`) — color channel source

**Color Combine:**
- Inputs A, B, C, D (`gc_mat_tevStage_{n}_colorInA/B/C/D`)
- Operation (`gc_mat_tevStage_{n}_colorOp`)
- Bias, Scale, Clamp (`gc_mat_tevStage_{n}_colorBias/Scale/Clamp`)
- Output register (`gc_mat_tevStage_{n}_colorRegId`)

**Alpha Combine:**
- Inputs A, B, C, D (`gc_mat_tevStage_{n}_alphaInA/B/C/D`)
- Operation (`gc_mat_tevStage_{n}_alphaOp`)
- Bias, Scale, Clamp (`gc_mat_tevStage_{n}_alphaBias/Scale/Clamp`)
- Output register (`gc_mat_tevStage_{n}_alphaRegId`)

**Konst Selections:**
- Konst color select (`gc_mat_tevKColorSel_{n}`)
- Konst alpha select (`gc_mat_tevKAlphaSel_{n}`)

**Swap Mode:**
- Ras swap (`gc_mat_tevSwapMode_{n}_rasSel`)
- Tex swap (`gc_mat_tevSwapMode_{n}_texSel`)

### Footer Section
- Blend mode (type, src factor, dst factor, logic op)
- Z-mode (enable, func, write enable)
- Fog info (type, enabled)

---

## Data Source

All data is read from `gc_mat_*` custom properties on Blender material objects. These are set during BMD import by `Mat3.py` / `BModel.py`. No new data structures are introduced.

### Property Naming Convention

The existing import stores material indices into MAT3 sub-tables as custom properties. The panel reads these indices and looks up human-readable names via enum mappings defined in the panel code.

### GX Enum Mappings (defined in tev_panel.py)

```python
TEV_COLOR_IN = {0: "cprev", 1: "aprev", 2: "c0", 3: "a0", 4: "c1", 5: "a1",
                6: "c2", 7: "a2", 8: "texc", 9: "texa", 10: "rasc", 11: "rasa",
                12: "one", 13: "half", 14: "konst", 15: "zero"}

TEV_ALPHA_IN = {0: "aprev", 1: "a0", 2: "a1", 3: "a2", 4: "texa",
                5: "rasa", 6: "konst", 7: "zero"}

TEV_OP = {0: "add", 1: "sub", 8: "comp_r8_gt", 9: "comp_r8_eq",
          10: "comp_gr16_gt", 11: "comp_gr16_eq", 12: "comp_bgr24_gt",
          13: "comp_bgr24_eq", 14: "comp_rgb8_gt", 15: "comp_rgb8_eq"}

TEV_BIAS = {0: "zero", 1: "+0.5", 2: "-0.5"}
TEV_SCALE = {0: "1x", 1: "2x", 2: "4x", 3: "0.5x"}
TEV_REGISTER = {0: "prev", 1: "reg0", 2: "reg1", 3: "reg2"}

CULL_MODE = {0: "none", 1: "front", 2: "back", 3: "all"}
BLEND_TYPE = {0: "none", 1: "blend", 2: "logic", 3: "subtract"}
```

---

## Auto-Open Behavior

After BMD import completes in `BModel.py`:

1. Find the Properties editor area in the current Blender screen layout
2. Set its `space_data.context` to `'MATERIAL'`
3. Ensure the mesh object is selected so the material panel is populated
4. The TEV panel auto-appears because the material has `gc_mat_*` properties

Implementation: small hook at the end of `BModel.Import()` or in the import operator's `execute()` return.

---

## File Structure

### New file: `tev_panel.py`

Located in the blemd addon directory. Contains:
- `GX_TEV_Panel` (bpy.types.Panel) — main panel
- `GX_TEV_StagePanel` (bpy.types.Panel) — per-stage sub-panel (or drawn inline)
- GX enum lookup dicts
- `register()` / `unregister()` functions called from `__init__.py`

### Modified files:
- `__init__.py` — register/unregister the new panel
- `BModel.py` — add auto-open hook after import

---

## Property Discovery

On panel draw, the code:
1. Gets `context.active_object.active_material`
2. Checks for `gc_mat_tevStageNum` custom property — if missing, panel shows "Not a BMD material"
3. Reads stage count, then iterates stages reading `gc_mat_tevStage_{n}_*` properties
4. Falls back to "N/A" for any missing property (graceful degradation)

---

## Scope Boundaries

### In scope (Phase 1 — read-only):
- Display all TEV stages with full combiner details
- Display blend mode, z-mode, fog, alpha compare
- Auto-open on import
- GX enum → human-readable name mapping

### Out of scope (Phase 2 — editing):
- Modifying TEV stage values
- Writing changes back to MAT3 custom properties
- Live preview of TEV changes
- Creating new materials from scratch
- TEV stage add/remove/reorder

---

## Testing

1. Import `ma_mdl1.bmd` — panel should auto-open showing material TEV data
2. Click through each material slot — panel updates to show that material's TEV stages
3. Verify stage count, texture indices, and combine operations match what `Mat3.py` parsed
4. Test with a non-BMD material — panel should show "Not a BMD material" or hide
