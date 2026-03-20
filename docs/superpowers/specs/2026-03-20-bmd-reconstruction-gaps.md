# BMD Full Reconstruction — Technical Gap Analysis

**Date:** 2026-03-20
**Project:** J3DBlend (blemd) Blender Addon — BMD Export
**Goal:** Full BMD reconstruction from Blender scene data without import serialization/caching

---

## 1. Current State

The J3DBlend addon can import BMD files into Blender and export them back. Currently, only **VTX1** (vertex pools) and **SHP1** (shape/batch/primitive data) are reconstructed from the Blender mesh. All other sections are preserved as raw bytes from import.

### What Works Today

| Section | Reconstruction | Notes |
|---------|---------------|-------|
| **VTX1** | Full | Positions, normals, UVs, colors. Inverse skinning via PrecomputeVertexDRW. Coordinate conversion, deduplication, s16/f32 encoding. |
| **SHP1** | Full | Per-material batches, rigid/weighted classification, EVP1 envelope matching, per-packet 32-byte GX alignment, correct winding order. |
| **INF1** | Partial | `Rebuild()` method exists but is not wired into the main export path. |
| **JNT1** | Partial | `FromArmature()` + `DumpData()` exist, reading `gc_*` custom properties from Blender bones. |
| **EVP1** | Serialization only | `DumpData()` can write from parsed data. No builder from Blender vertex groups. |
| **DRW1** | Serialization only | `DumpData()` can write from parsed data. No builder from Blender data. |
| **MAT3** | Raw only | `DumpData()` raises `NotImplementedError`. 30 sub-tables with cross-referencing indices. |
| **TEX1** | Raw only | Header writing exists but no pixel data encoding. |
| **MDL3** | Raw only | Optional GX display list cache. Not needed for basic rendering. |

### Critical Bugs Fixed (Session 2026-03-19/20)

1. VTX1 arrayFormat sentinel (0xFF terminator)
2. Per-packet GX display list 32-byte alignment
3. SHP1 primitive data offset 32-byte alignment
4. Batch ordering from INF1 scene graph extraction
5. VTX1/SHP1 shared DRW assignment (PrecomputeVertexDRW)
6. EVP1 envelope matching with weight comparison (duplicate bone sets)
7. useMtxIndex = actual DRW1 index (not hardcoded 0)
8. Triangle winding order (Blender CCW → GX CW)
9. Blended inverse skinning for multi-bone vertices
10. Majority-vote batch classification with multi-DRW detection

---

## 2. Sections Requiring New Builders

### 2.1 DRW1 — Draw Matrix Table

**Purpose:** Maps each draw matrix slot to either a single bone (rigid) or an EVP1 envelope (weighted). Every SHP1 vertex references a DRW1 entry to determine its runtime transform.

**Binary layout:** Simple flat table — `u8[] isWeighted` + `u16[] data`. Rigid entries (isWeighted=0) store bone index. Weighted entries (isWeighted=1) store EVP1 envelope index. All rigid entries come first.

**Algorithm to build from Blender:**
1. Scan all vertices across all materials
2. For each unique single-bone assignment: create rigid entry `(isWeighted=0, data=bone_index)`
3. For each unique multi-bone weight combination: create weighted entry `(isWeighted=1, data=evp_index)` — requires EVP1 to be built first
4. Deduplicate entries by (type + data value)
5. Sort: rigid first, then weighted

**Data sources:** Blender vertex groups, JNT1 bone indices, EVP1 envelope indices.

**Complexity:** Medium. Flat table, but must coordinate with EVP1 and be built before SHP1.

**SuperBMD approach:** Iterates all mesh vertices, classifies by weight count. Single-bone = rigid. Multi-bone = weighted. Deduplicates using `Weight.Equals()`. Weighted indices are initially 0 and filled during SHP1 generation.

---

### 2.2 EVP1 — Envelope/Skinning Data

**Purpose:** Stores inverse bind matrices (one per bone) and weighted envelope definitions (bone indices + weights for each unique multi-bone combination).

**Binary layout:**
- `u8[]` bone counts per envelope
- `u16[]` bone indices (flat, consecutive)
- `f32[]` weights (parallel to indices)
- `Mtx[]` inverse bind matrices (3x4 float = 48 bytes each, one per bone)

**Algorithm to build from Blender:**
1. **Inverse bind matrices:** For each armature bone, compute `inverse(bone_rest_world_matrix)`. This transforms from model space to bone-local space. In Blender: `bone.matrix_local.inverted()` with coordinate system conversion (Blender Y-up → GC Z-up).
2. **Envelope entries:** Scan all multi-bone vertices. For each unique combination of (bone_indices, weights), create one envelope. Deduplicate — vertices with the same bones and weights share an envelope.
3. **Coordinate conversion:** Inverse bind matrices need the same Blender→GC coordinate swap applied.

**Data sources:** Blender armature rest pose (`bone.matrix_local`), vertex groups.

**Complexity:** Medium. Computing inverse bind matrices is straightforward (matrix inversion). Envelope deduplication requires scanning all weighted vertices and comparing bone+weight sets.

**SuperBMD approach:** Reads inverse bind matrices directly from Assimp's `bone.OffsetMatrix` (which IS the inverse bind matrix). Transposes for row/column major conversion. Envelope weights come from DRW1's weight classification.

---

### 2.3 INF1 — Scene Graph

**Purpose:** Defines the rendering hierarchy — which bones own which materials and shapes. Controls transform inheritance and draw order.

**Binary layout:** Flat stream of (type: u16, value: u16) entries encoding a tree via push/pop:
- 0x10 = Joint (bone index)
- 0x11 = Material (material index)
- 0x12 = Shape (batch index)
- 0x01 = Begin child scope
- 0x02 = End child scope
- 0x00 = Terminator

Also stores: flags (matrix calc type), packet count, vertex count.

**Algorithm to build from Blender:**
1. Walk the armature bone hierarchy depth-first
2. For each bone: emit Joint node
3. For rigid batches attached to this bone: emit Material → Shape under this joint
4. For weighted/multi-bone batches: emit at root level (SuperBMD approach)
5. Emit matching Begin/End child scopes

**Data sources:** Blender armature hierarchy, SHP1 batch→material mapping, DRW1 rigid→bone mapping.

**Complexity:** Medium. A `Rebuild()` method already exists in the codebase. Main concern is matching the original scene graph ordering (which affects Z-sorting and draw order).

**SuperBMD approach:** Two passes — root-level multi-bone meshes first, then bone-attached rigid meshes via recursive hierarchy walk. Always Joint→Material→Shape.

---

### 2.4 JNT1 — Joint/Bone Table

**Purpose:** Stores per-bone transform data (scale, rotation, translation) and bounding boxes.

**Binary layout:** Per-bone (0x40 bytes): kind (u16), scale_compensate (u8), scale (Vec3 f32), rotation (S16Vec3), translation (Vec3 f32), radius (f32), bbox min/max (Vec3 f32 each).

**Current state:** `FromArmature()` already exists — reads `gc_*` custom properties from Blender bones. `DumpData()` is fully implemented.

**What's needed for from-scratch:**
1. Extract bone local transforms from Blender armature rest pose (not gc_ custom properties)
2. Convert Blender transforms to J3D format: decompose matrix → scale + rotation (as s16) + translation
3. Compute bounding boxes from vertex positions assigned to each bone
4. Set matrix_type (0=Basic for most cases)

**Data sources:** Blender armature `bone.matrix_local`, `bone.parent.matrix_local` (for computing local-relative transforms).

**Complexity:** Simple-Medium. Transform decomposition and coordinate conversion. The existing `FromArmature()` provides a template.

---

### 2.5 MAT3 — Materials

**Purpose:** Defines material properties: colors, textures, TEV combiner stages, blending, culling, fog, alpha test, z-mode.

**Binary layout:** Extremely complex — 0x84-byte header with 30 offset slots pointing to separate data pools. Each material is a 0x14C-byte index table referencing into these pools. Deduplication at the pool level.

**Approach options:**

**Option A — Minimal material (for static/test meshes):**
Build a single default material with basic settings: one TEV stage that samples texture and modulates with vertex color. This covers 90% of simple use cases. ~100 lines of code.

**Option B — Blender material conversion:**
Parse Blender's shader node tree and map to GX TEV stages. This is a massive undertaking — Blender's PBR shader model doesn't map cleanly to GX's fixed-function TEV combiners. Not recommended.

**Option C — Material presets (SuperBMD approach):**
Load material definitions from a JSON file. Users author materials in a reference BMD, export the material JSON, then reference it when building new BMDs. This is how SuperBMD handles it — `LoadFromJson()` is the primary path.

**Recommendation:** Option A for initial implementation (minimal material that just shows a texture). Option C for production use (JSON material presets).

**Complexity:** Very Complex for full reconstruction. Simple for minimal material.

---

### 2.6 TEX1 — Textures

**Purpose:** Stores texture image data in GameCube hardware formats.

**Binary layout:** Per-texture header (0x20 bytes) + raw encoded pixel data. Formats: I4, I8, IA4, IA8, RGB565, RGB5A3, RGBA8, CMPR (S3TC/DXT1), CI4, CI8, CI14X2.

**What's needed:**
1. **Image format encoding:** Convert RGBA pixels to GC tile-interleaved formats. Each format has specific tile sizes (e.g., CMPR = 8x8 tiles of 4x4 DXT1 blocks).
2. **Mipmap generation:** Downsample and encode each level.
3. **Palette encoding:** For CI formats, build color palette via quantization.
4. **Header generation:** Format, dimensions, wrap modes, filter modes.

**Approach options:**

**Option A — Use existing Python GC texture libraries:**
Libraries like `wimgt` (Wiimms Image Tool) or Python ports can handle GC format encoding. Search for existing implementations.

**Option B — Implement encoding from scratch:**
Reference SuperBMD's `BinaryTextureImage.EncodeData()` or noclip.website's texture decoder (reverse the decode).

**Option C — Raw image passthrough:**
For testing, store textures as RGBA8 (simplest GC format, no compression). Works but large.

**Recommendation:** Option C for initial testing, Option A for production if a library exists.

**Complexity:** Complex. GC texture formats are hardware-specific with tile interleaving.

---

## 3. Implementation Plan

### Phase 1 — Static Mesh Support (Est. 4-6 hours)

Build the minimum viable pipeline for a static (single-bone, no animation) BMD:

1. **JNT1 builder** — One root bone at origin with identity transform. Bounding box from mesh extents.
2. **DRW1 builder** — One rigid entry pointing to root bone.
3. **EVP1 builder** — Empty (no envelopes, just one identity inverse bind matrix).
4. **INF1 builder** — Root joint → one material → one shape.
5. **MAT3 minimal** — One default material (basic TEV: texture * vertex_color).
6. **TEX1 minimal** — One texture in RGBA8 format (uncompressed).

**Test:** Replace a simple game object's BMD with a custom cube.

### Phase 2 — Skeletal Mesh Support (Est. 6-8 hours)

Extend to multi-bone animated meshes:

1. **JNT1 from Blender armature** — Full bone hierarchy, local transforms, bounding boxes.
2. **DRW1 from vertex groups** — Rigid + weighted entries, deduplication.
3. **EVP1 from vertex groups** — Inverse bind matrices from rest pose, envelope deduplication.
4. **INF1 full hierarchy** — Wire up existing `Rebuild()` with correct batch ordering.
5. **MAT3 multi-material** — Support multiple basic materials.

**Test:** Import Mario BMD, modify mesh, export with full reconstruction (no raw data dependency).

### Phase 3 — Production Quality (Est. 8-12 hours)

Polish for real-world use:

1. **TEX1 proper encoding** — CMPR compression, mipmaps, palette formats.
2. **MAT3 JSON presets** — Material definition files for complex materials.
3. **Triangle strip optimization** — Convert raw triangles to tristrips for smaller files.
4. **Bounding box computation** — Per-batch and per-bone from vertex data.
5. **VTX1 format optimization** — s16 fixed-point with appropriate decimal points instead of f32.

---

## 4. Section Dependency Graph

```
Blender Armature ──→ JNT1 ──→ EVP1 (inverse bind matrices)
                       │           │
Blender Vertex    ──→ DRW1 ←──── EVP1 (envelope indices)
Groups                 │
                       ├──→ VTX1 (inverse skinning)
                       ├──→ SHP1 (matrix tables)
                       └──→ INF1 (batch-to-bone mapping)

Blender Materials ──→ MAT3
Blender Textures  ──→ TEX1

Build order: JNT1 → EVP1 → DRW1 → VTX1 + SHP1 → INF1 → MAT3 → TEX1
```

---

## 5. GX Hardware Requirements (Must Not Violate)

These were discovered during the VTX1/SHP1 reconstruction work and apply to ALL sections:

1. **32-byte alignment** for all display list data (GXCallDisplayList DMA requirement)
2. **32-byte alignment** for the primitive data section offset within SHP1
3. **VTX1 arrayFormat must end with 0xFF sentinel** or game reads garbage format entries
4. **useMtxIndex** in SHP1 matrix init data must be the actual DRW1 index for rigid batches
5. **Triangle winding:** GX expects CW, Blender stores CCW — swap v1/v2 on export
6. **Section padding:** Each section must be padded to 32-byte boundary
7. **DRW1 ordering:** All rigid entries must come before all weighted entries
8. **DRW1 envelope duplication bug:** Nintendo's dev tools duplicated all envelope entries. Some J3D games require this duplicated data. SMS may need it — test with and without.

---

## 6. Reference Implementations

| Tool | Source | Key Insight |
|------|--------|-------------|
| **SuperBMD** | github.com/Sage-of-Mirrors/SuperBMD | Only open-source tool that builds complete BMDs from scratch. Material JSON presets. C#/Assimp. |
| **SMS Decomp** | `C:\Users\ryana\Documents\sms\src\JSystem\J3D\` | Definitive binary layout reference. J3DShapeFactory, J3DModelLoader. |
| **noclip.website** | github.com/magcius/noclip.website | Best-documented TypeScript parser. Documents DRW1 envelope duplication bug. |
| **j3dview** | github.com/blank63/j3dview | Python BMD viewer/editor with NumPy. Texture export/replace. |
| **GCFT** | github.com/LagoLunatic/GCFT | Python GUI for editing MAT3 material properties with visual preview. |
| **BrawlCrate** | github.com/soopercool101/BrawlCrate | Similar Nintendo model formats (MDL0). Vertex encoding reference. |
| **BlenderMD** | github.com/AugsEU/BlenderMD | Blender plugin wrapper that calls SuperBMD for BMD export. |

### Wiki Documentation
- **CloudModding ZGCN**: wiki.cloudmodding.com/zgcn/BMD_and_BDL — Most comprehensive section-by-section breakdown
- **Luma's Workshop**: lumasworkshop.com/wiki/BMD/BDL_(File_Format) — Detailed format spec with J3DLoader flags
- **WindEditor Wiki**: github.com/LordNed/WindEditor/wiki/BMD-and-BDL-Model-Format — Good SHP1/primitive docs

---

## 7. Risk Assessment

| Risk | Impact | Mitigation |
|------|--------|------------|
| MAT3 complexity | High | Start with minimal 1-TEV material, add JSON presets later |
| GC texture encoding | Medium | Use RGBA8 initially (no compression), add CMPR later |
| Inverse bind matrix precision | Medium | Validate against known-good imports by comparing round-trip |
| Scene graph ordering affects Z-sort | Low | Use SuperBMD's proven two-pass algorithm |
| VTX1 s16 quantization artifacts | Low | Start with f32, optimize to s16 later |
