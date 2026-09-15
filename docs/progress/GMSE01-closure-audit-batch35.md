# Priority 1 closure audit — batch 35

Baseline: `92740f19`, local GMSE01 revision 0 configuration.
The preceding checkpoint completed two manager files and was verified progress.
This checkpoint resolves the deferred regional string-boundary problem and source-links MarDirectorCreateObjects and TargetArrow.
It adds 444 source-linked code bytes and 392 source-linked data bytes, with no reconstructed code changes.

## Root cause: Application's English disc-error strings

The imported map's first two error-message objects at 0x803A403C and 0x803A40B8 have sizes 124 and 109.
The local executable contains complete null-terminated strings of 121 and 106 bytes at those addresses.
The first still rounds to 124 bytes at four-byte alignment, so the second starts at the mapped address.
The second rounds to 108 rather than 112, shifting the remaining Application strings by four bytes.
The next file begins at 0x803A4308, eight bytes before the imported boundary 0x803A4310, after section alignment.
That eight-byte discrepancy persists through the end of .rodata.

The earlier batch 6 endpoint-only trial did not address this origin or downstream symbol attribution.
This pass audits the complete affected range and corrects symbols and splits together.
The immutable original DOL, not the imported map, determines these corrections.

## Regional metadata corrections

- Set the two error-message sizes to 0x79 and 0x6A.
- Subtract four from Application symbol addresses from imported 0x803A4128 through 0x803A42F8.
- Subtract eight from .rodata symbols at imported addresses 0x803A4310 onward.
- Update 69 .rodata split lines consistently, including Application's end.
- Keep the executable's final .rodata endpoint at 0x803AB658.
  The final constant begins at 0x803AB618 and occupies its full 0x40 bytes; the prior 0x38 size was an artifact of its incorrect starting address.
- Remove all 35 old retail_padding labels in this region, totaling 116 bytes.
  Every byte they covered lies inside a correctly attributed object after the correction.
  These were split fragments, not independently evidenced source variables or nonzero padding.

The regional metadata also describes downstream extracted library objects, so their .rodata boundaries must follow the same verified layout.
No library implementation or source-link manifest entry was added for those objects.
All unrelated sections and earlier .rodata addresses remain unchanged.

## Independent checks

Scanned map-sized strings against the DOL before editing metadata.
Verified 6,502 unchanged strings, 1,273 downstream strings with the eight-byte displacement, and all 17 Application strings with the four-byte displacement.
The two error-message null terminators confirm their corrected sizes directly.
Confirmed every removed gap byte is covered by a corrected object.
The input SHA-1 remains a6782903ef79d4196c8489ecb1b57decb5b3728f.

A full rebuild with the existing source-link manifest passes byte comparison and SHA-1 after the metadata correction.
Both deferred game objects then report exact code and every data section.
Their map checks pass: one function for MarDirectorCreateObjects and four for TargetArrow, with no UNUSED entries or warnings.
Their source-link promotion and combined full build pass byte comparison and SHA-1 again.
The original gap labels missing from source are satisfied by natural linker alignment; no source padding was added.

Required baseline/changes_all and comparison of all 12,904 functions pass.
Every function's presence and similarity is unchanged, and no unit loses matched data.
No gameplay test was performed.

## Measurement interpretation

Matched data increases by 14,824 bytes because objects are now compared against the correct original ranges; this is recovered measurement accuracy, not newly reconstructed source.
Game data attribution shrinks by eight bytes at Application, while the final extracted constant gains eight bytes in its category.
Aggregate data coverage still measures the same 641,075 bytes.
Matched code is unchanged.

Source linking: 82 game files / 86,596 code bytes.
Game: 24.97447% matched / 3.040953% source-linked.
Aggregate: 38.566406% matched / 2.4029427% source-linked.
The small-file queue remains 100 authorized files / 91,684 unmatched code bytes.
The deferred exact-code queue now contains only CameraInbetween and PollutionEvent; their source/map blockers remain as previously recorded.

See GMSE01-batch35.json for the measured checkpoint.
Local evidence is in build/GMSE01-string-offset-audit-batch35.json, GMSE01-rodata-corrections-batch35.json, GMSE01-rodata-verification-batch35.json and the build/map/function logs.
Continue the plan's file-completion order with these corrected regional comparisons.
