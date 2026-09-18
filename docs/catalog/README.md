# Matching catalog

What this clone has learned about matching GMSE01, split by topic.
Read only the file that fits the problem at hand.
General MWCC guidance stays in [`../AGENT_MATCHING_TIPS.md`](../AGENT_MATCHING_TIPS.md).

| File | Read when |
| --- | --- |
| [RULES.md](RULES.md) | Always: the one-line rules card; open a topic file only at the section a rule points to |
| [frame-gaps.md](frame-gaps.md) | Instructions match but the `stwu` frame size differs |
| [codegen-tells.md](codegen-tells.md) | A diff shows a bool, branch, inline, float or load-order difference and you need the source shape behind it |
| [tu-reconstruction.md](tu-reconstruction.md) | Starting a TU: vtable order, param names, string prefixes, `__sinit`, reordering, layout evidence, UNUSED bodies |
| [linking.md](linking.md) | An object matches but breaks the DOL when source-linked |
| [region-us.md](region-us.md) | Offsets or strings differ from the Japanese layout (MSound, GCConsole2, ProgSelect, HelpActor) |
| [units/boss-hanachan.md](units/boss-hanachan.md) | Any `BossHanachan*` unit |
| [units/player.md](units/player.md) | `TMario`, `TWaterGun`, MarDirector particle loading |
| [units/other-units.md](units/other-units.md) | Exceptions and rejected trials for a specific Camera, Enemy, GC2D, Map, MSound, NPC, System, MoveBG or Animal unit |

## Using it

1. `grep -rn '<class|helper|symptom>' docs/catalog` before changing a shared declaration or helper.
2. Inventory related definitions and callers with `rg` across `src`, `include` and the map.
3. Compare a representative caller's full diff before changing a shared definition, and apply the change only where the instructions support it.
4. `ninja baseline` before, `ninja changes_all` after; DOL SHA-1 after source-link changes.

Similar source text is a lead, not proof of equivalent codegen.
A CLI score that rounds to 100.0% is not an exact match.

## Keeping it small

- **Edit the existing entry** instead of appending a new one. When a finding is overturned, rewrite the entry and keep a one-line "ruled out" note so nobody retries it.
- Record the rule, the function that proved it and the rejected alternatives. Leave out build/SHA-1/percentage boilerplate; that belongs in the commit.
- Put a rule in a topic file only if it applies beyond one unit; otherwise it goes under that unit in `units/`.
- Batch history lives in git and `docs/progress/history.md`, not here.
