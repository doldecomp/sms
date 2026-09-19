# Decomp-Fortschritt (GMSP01 / PAL)

## Wichtiger Kontext zur Zielversion

Dieses Repo zielt primär auf **GMSJ01 (NTSC-J)**. Die hier verwendete Disc ist
**GMSP01 (PAL)**. Upstream bezeichnet PAL als „slightly broken, feel free to fix";
konkret:

- `.github/workflows/build.yml` Zeile 15: `version: [GMSJ01] # GMSP01` —
  PAL ist aus der CI **auskommentiert**, wird also nicht gegengeprüft.
- CI baut `ninja all_source progress report.json`; das Target `ok`
  (SHA1-Verifikation der DOL) läuft **nie**. Ein „DOL OK" ist erst bei 100 %
  Completion erreichbar, nicht als Setup-Gate.

## Ausgangs-Baseline (GMSP01, Commit b4cab1d2)

Aus `build/GMSP01/report.json`:

| Metrik | Wert |
| --- | --- |
| Fuzzy match | 61,40 % |
| Code matched | 26,29 % (938.880 / 3.571.488) |
| Code complete | 8,54 % (305.020) |
| Data matched | 44,78 % (285.583 / 637.803) |
| Data complete | 15,17 % (96.763) |
| Funktionen matched | 41,71 % (5.319 / 12.751) |
| Units complete | 173 / 730 |

DOL-Vergleich gegen Original (`a2edfa86…`, identische Größe 4.094.112 Bytes):
1.673.643 abweichende Bytes (41 %), davon 46,2 % im `.text`-Hauptsegment.

## Behobene PAL-Build-Blocker

Der PAL-Build hat vorher **gar nicht gelinkt**. Behoben:

1. **6 undefinierte Symbole** beim Link von `marioEU.elf`:
   - `OSGetLanguage`, `OSGetEuRgb60Mode`, `OSSetEuRgb60Mode` — PAL-only
     SDK-Funktionen, in `src/dolphin/os/OSRtc.c` ergänzt (`#ifdef VERSION_GMSP01`).
   - `SMSGet{Game,Title,GCLogo}VideoHeight` — PAL erwartet `__FUl`
     (nimmt TV-Format als Parameter), Quelle deklarierte `__Fv`.
2. **`config/GMSP01/build.sha1`** verwies auf `build/GMSP01/mario.dol`,
   gebaut wird aber `marioEU.dol` (`config.yml`: `name: marioEU`).

### Lokale Voraussetzung (Windows)

`.gitattributes` setzt `* text=auto`; auf Windows erzeugt das CRLF und damit
einen vom Upstream abweichenden Worktree. Nötig:

    git config core.eol lf

(`core.autocrlf false` allein genügt **nicht**, das `text`-Attribut überstimmt es.)

## Gematchte Funktionen

Byte-genau gegen die Original-DOL verifiziert:

- `OSGetLanguage` (0x8033FF8C, 0x6C) — MATCH
- `OSGetEuRgb60Mode` (0x8033FFF8, 0x70) — MATCH
- `OSSetEuRgb60Mode` (0x80340068, 0xA4) — MATCH
  (brauchte `char trash[0x2]` für den 0x20-Stackframe, wie `OSSetProgressiveMode`)

## Offene Nonmatching-Fälle

- **`src/System/Resolution.cpp`** — die drei PAL-`VideoHeight`-Funktionen sind
  funktional korrekt und linken, aber die **Funktionsreihenfolge im TU** weicht ab.
  PAL emittiert `SMSGetGCLogoVideoHeight` zuerst, der aktuelle Quelltext zuletzt;
  die PAL-Reihenfolge ist exakt die Umkehrung der jetzigen Dateireihenfolge.
  Ein Match erforderte, die Dateireihenfolge versionsabhängig zu spalten — das
  würde GMSJ01 betreffen und ist ohne JPN-Disc nicht gegenprüfbar. Zurückgestellt.
- `SMSGetTitleVideoHeight` dupliziert in PAL den Switch, statt an
  `SMSGetGameVideoHeight` weiterzureichen (in der PAL-Fassung so umgesetzt).

## Nächste Kandidaten

Noch nicht ausgewählt — sinnvoll erst nach der Entscheidung, ob weiter auf PAL
oder auf GMSJ01 gearbeitet wird (siehe oben: PAL ist ohne CI-Absicherung und
ohne Upstream-Fortschrittstracking).
