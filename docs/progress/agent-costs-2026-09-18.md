# Agent token costs (from task-notification usage lines)

## Old policy (full CLAUDE.md + whole catalog in the brief, full diffs)
| batch | type | tokens | tool uses | tokens/tool | minutes | outcome |
| --- | --- | ---: | ---: | ---: | ---: | --- |
| 130 | sweep (address lever) | 209,072 | 90 | 2,323 | 39 | 7 exact |
| 131 | one-function pass II | 389,472 | 163 | 2,389 | 49 | 2 closed / 2 linked |
| 132 | structural (relocs, bindings) | 158,814 | 70 | 2,269 | 17 | 1 exact, 3 bindings |
| 133 | research (JGadget) | 226,049 | 96 | 2,355 | 26 | 1 header rule, 1 exact |
| 134 | structural (link blockers) | 313,088 | 173 | 1,810 | 38 | 2 linked, 29 bindings |
| rules card | docs (read whole catalog once) | 325,020 | 36 | 9,028 | 20 | RULES.md |
| 135 | library one-function pass | 483,409 | 209 | 2,313 | 53 | 5 linked, 3 improved |
| 136 | closure, 6 units | 574,308 | 183 | 3,138 | 58 | 2 linked, 2 more exact |

## New policy (short CLAUDE.md, AGENT_GUIDE, RULES.md, --clusters)
| batch | type | tokens | tool uses | tokens/tool | minutes | outcome |
| --- | --- | ---: | ---: | ---: | ---: | --- |
| 138 | Sonnet lever sweep, 24 library units | 236,585 | 103 | 2,297 | 13 | 2 functions improved, 0 exact (29/32 candidates had no lever site) |
| 137 | header round 22 (Opus, NEW policy) | 264,032 | 134 | 1,970 | 29 | 4 accepted, +32 fuzzy pts, MarNameRefGen order PASS |
| 141 | structural (Opus, NEW policy) | 213,121 | 73 | 2,919 | 20 | J3DClusterLoader order PASS; 2 items stale |
| 140 | library closure II, 7 units (Opus, NEW policy) | 302,414 | 130 | 2,326 | 33 | 3 exact, 0 linked |
| 139 | closure, 6 units (Opus, NEW policy; twin of 136) | 305,321 | 137 | 2,229 | 38 | 2 linked, 2 more improved |
| 144 | research, register ranking (Opus, NEW policy) | 184,713 | 63 | 2,932 | 21 | allocation rule found; sunmgr 100% (link blocker) |
| 143 | closure, 8 small units (Opus, NEW policy) | 323,592 | 123 | 2,631 | 36 | 2 exact, 1 bug fix, 0 linked |
| 145 | register pass + sunmgr link (Opus, NEW policy) | 206,845 | 75 | 2,758 | 19 | sunmgr linked; 4 register sites exhausted, rule refined |
| 146 | research, inline refusal (Opus, NEW policy) | 202,764 | 83 | 2,443 | 23 | hypotheses disproved; budget table measured; 2 fns lifted |
| 147 | header round 23 (Opus, NEW policy) | 205,674 | 95 | 2,165 | 23 | JASDSPChannel linked; 2 rejected with measurements |
| 148 | depth pass, 23-site census (Opus, NEW policy) | 319,193 | 155 | 2,059 | 28 | 12 fns lifted, 3 exact, 0 linked |
| 150 | library re-pass, 8 units (Opus, NEW policy) | 224,934 | 98 | 2,295 | 21 | 3 linked |
| 151 | closure, 7 three-function units (Opus, NEW policy) | 249,579 | 98 | 2,547 | 21 | 1 exact, 2 near, 0 linked |
| 149 | closure, 7 mid-size units (Opus, NEW policy) | 349,354 | 146 | 2,393 | 45 | 2 exact, 1 fn 38->74, 0 linked |
| 154 | header round 24 (Opus, NEW policy) | 247,510 | 107 | 2,313 | 24 | 2 accepted, 2 rejected w/ measurements |
| 152 | game re-pass, 12 one-function units (Opus, NEW policy) | 249,990 | 109 | 2,293 | 27 | 1 linked, 2 improved |
| 153 | single-function deep batch (Opus, NEW policy) | 319,130 | 65 | 4,910 | 33 | 66 -> 99.7, header sign fix, 0 linked |
| 156 | header round 25 (Opus, NEW policy) | 184,456 | 67 | 2,753 | 19 | 1 accepted (MarioParticle PASS), 2 rejected w/ measurements |
| 157 | closure, 8 four-function units (Opus, NEW policy) | 295,592 | 118 | 2,505 | 31 | 0 exact, 1 fn +13, residues classified |
| 159 | research, operator* split (Opus, NEW policy) | 196,601 | 60 | 3,277 | 22 | consumption split, no legal overload; V2 near-miss +12/-1 |
| 155 | library closure III, 4 units (Opus, NEW policy) | 402,225 | 178 | 2,260 | 54 | 4 exact, 0 linked |
| 158 | closure, 7 five/six-function units (Opus, NEW policy) | 332,106 | 197 | 1,686 | 39 | 6 exact, 3 bugs, coasterkiller data 100, 0 linked |
| 160 | header round 26 (Opus, NEW policy) | 192,089 | 72 | 2,668 | 20 | RenderModeObj linked; isReachedToGoal exact |
| 161 | research, JGadget pool (Opus, NEW policy) | 237,101 | 65 | 3,648 | 22 | class closed negatively: block stride 8 vs 12 |
| 164 | header round 27, free-carrier survey (Opus, NEW policy) | 235,633 | 85 | 2,772 | 21 | 1 exact, DrawSyncManager UNUSED bodies, survey table |
| 163 | follow-up closure, 5 units (Opus, NEW policy) | 225,740 | 88 | 2,565 | 25 | 0 exact; header lead worth +7/-2 |
| 162 | game re-pass, 6 two-function units (Opus, NEW policy) | 282,917 | 108 | 2,620 | 28 | 1 exact, 1 instruction-exact, 0 linked |
| 167 | header round 28 (Opus, NEW policy) | 110,741 | 54 | 2,051 | 10 | +7 exact |
| 165 | library re-pass II, 6 units (Opus, NEW policy) | 234,319 | 80 | 2,929 | 24 | 0 closed; residues characterised |
| 169 | header round 29, consuming-binding survey (Opus, NEW policy) | 148,896 | 64 | 2,327 | 15 | +1 exact; shape exhausted |
| 166 | structural pass, game (Opus, NEW policy) | 350,209 | 190 | 1,843 | 31 | 11 exact via reloc-target bugs; new detector class |
| 168 | closure, 4 near-done units (Opus, NEW policy) | 292,995 | 115 | 2,548 | 37 | 3 exact (sunmodel), 0 linked |
| 171 | research, FPR rule (Opus, NEW policy) | 203,154 | 68 | 2,988 | 18 | callee-saved FPR rule found; 1 exact; volatile FPRs = block trades |
| 170 | library structural pass (Opus, NEW policy) | 328,929 | 169 | 1,946 | 40 | 3 linked (JPADraw, JASHardStream, JAIGFrameSequence) |
| 173 | header round 30 (Opus, NEW policy) | 198,904 | 64 | 3,108 | 22 | Vec accessor sibling (+2.4 on perform); SleepBossHanachan refuted |
| 174 | FPR re-pass (Opus, NEW policy) | 192,738 | 78 | 2,471 | 17 | 2 exact (camerashake); in-place parameter lever; census triaged |
| 172 | reloc-target pass II (Opus, NEW policy) | 391,469 | 248 | 1,578 | 41 | 8 exact, 33 improvements, Item data 6->88, tools/check-relocs.py |
| 176 | structural pass II (Opus, NEW policy) | 269,103 | 130 | 2,070 | 25 | 4 exact (3 real bugs) |
| 177 | execShake + countTexDegree (Opus, NEW policy) | 240,853 | 65 | 3,705 | 25 | 0 closed; 2 real fixes; both diagnosed |
| 175 | 4-byte lever re-pass (Opus, NEW policy) | 338,839 | 161 | 2,105 | 38 | 2 linked (MovieSubtitle, MarDirectorPreEntry), fishoid clipBoids exact |
| 178 | reloc-target pass III, big units (Opus, NEW policy) | 415,597 | 223 | 1,864 | 42 | 13 functions up (30-40 pts each), 0 exact, 0 linked |
| 179 | structural pass III (Opus, NEW policy) | 373,765 | 188 | 1,988 | 36 | 2 exact, 5 real bugs, CardManager data 3->56 |
| 180 | structural pass IV (Opus, NEW policy) | 355,850 | 176 | 2,022 | 37 | 4 exact, 3 real bugs, 3 MISSING weak accessors restored |
| 181 | 4-byte lever re-pass II (Opus, NEW policy) | 306,264 | 137 | 2,235 | 34 | 2 exact (CLBScreenFPosToSPos, calcGoalForce), 0 linked |
