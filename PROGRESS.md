# Decomp-Fortschritt

## Ausgangs-Baseline (GMSJ01 / NTSC-J)

- Upstream-Basis: `b4cab1d2` (`BossPakkun closer`)
- Arbeitsbranch: `decomp-work`
- Referenz-DOL: `build/GMSJ01/mario.dol: OK`
- Erwarteter SHA-1: `9f5a8caf56f5356aeac9d3ed28bf8de976a03625`
- Report: `report.json`, erzeugt mit
  `build/tools/objdiff-cli report generate -o report.json`

| Metrik | Ausgangswert |
| --- | ---: |
| Fuzzy match | 77,34 % |
| Code matched | 41,08 % (1.474.716 / 3.590.088 Bytes) |
| Code complete / linked | 18,00 % (646.308 Bytes) |
| Data matched | 59,44 % (380.623 / 640.331 Bytes) |
| Data complete | 20,72 % (132.708 Bytes) |
| Funktionen matched | 66,10 % (8.514 / 12.881) |
| Units complete | 396 / 736 |

## Stand nach acht Iterationen

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,11 % (1.475.800 Bytes) | +1.084 Bytes |
| Funktionen matched | 66,15 % (8.521 / 12.881) | +7 |
| Code complete / linked | 18,80 % | +0,80 pp |
| Units complete | 402 / 736 | +6 |

Die Referenz-DOL bleibt `OK`.

### Bereiche

| Bereich | Fuzzy | Code matched | Units linked |
| --- | ---: | ---: | ---: |
| Game Code | 71,28 % | 27,57 % | 2,86 % (78 / 387) |
| JSystem | 99,71 % | 87,29 % | 62,14 % (172 / 200) |
| SDK | 99,97 % | 98,88 % | 98,34 % (146 / 149) |

## Windows-Setup

Die JPN-RVZ liegt als Hardlink unter `orig/GMSJ01/disc.rvz`; `orig/*/*` ist
git-ignored. Native Windows-Tools werden verwendet, kein wibo/Wine.

Der Checkout braucht LF:

    git config core.autocrlf false
    git config core.eol lf

## Bisherige Branch-Änderungen

Vor dem Wechsel auf GMSJ01 wurden PAL-Buildfehler behoben. Alle Änderungen
sind mit `VERSION_GMSP01` isoliert; der GMSJ01-Referenz-Build bleibt `OK`.

Byte-genau gegen die PAL-DOL gematcht:

- `OSGetLanguage`
- `OSGetEuRgb60Mode`
- `OSSetEuRgb60Mode`

Offen für PAL: Die `VideoHeight`-Funktionen in `System/Resolution.cpp` linken,
matchen wegen abweichender TU-Funktionsreihenfolge aber noch nicht.

## Offene GMSJ01-Nonmatching-Fälle

- `Player/MarioAccess.cpp`: `SMS_IsMarioOnWire` (72 Bytes, 93,83 %).
  Das Ziel lädt `mHolder` für Nullprüfung und Typzugriff zweimal; MWCC fasst
  die beiden Quellzugriffe zusammen. Verschachtelte Bedingungen,
  Zugriffsmethoden sowie volatile/alias-basierte Varianten erreichten nach
  mehreren Versuchen höchstens 96,61 %, waren aber synthetischer als die
  saubere 93,83-%-Fassung. Gemäß Iterationsregel als `// NONMATCHING`
  zurückgestellt.

- `Strategic/HitActor.cpp`: `THitActor::calcEntryRadius`. Aktueller Quelltext
  matcht zu **91,90 %** (124 Bytes); mit `char trash[0x30]`-Padding stieg es
  auf 97,68 %. Frame ist reproduzierbar; die restlichen Abweichungen liegen in
  FPSCR-Registerwahl und Reihenfolge der `fmadds`/`frsp`-Stores und ließen
  sich nicht eindeutig auf MWCC-übliche Ausdrucksformen abbilden.

- `Strategic/livemanager.cpp`: `TLiveManager::perform` (252 Bytes, 99,92 %).
  Bestes Experiment `char trash[0x10]` reproduziert Frame-Größe (0x50 Bytes).
  Verbleibend sind die fünf `stb r0, 0x24..0x27(r1)`/`lwz r28, 0x24(r1)`-Bytes:
  das Original speichert nur `r27..r30` via `stmw r27, 0x3c(r1)` (4 Register,
  Save-Bereich `0x3c..0x4b`) und legt das Color-Local davor bei `0x34`; wir
  speichern `r27..r31` (5 Register, Save-Bereich `0x38..0x4b`), wodurch das
  Color-Local auf `0x24` fällt. Die drei Branch-Differenzen sind identische
  relative Offsets (Differenz = 0x84 wie bei den Instruktionsadressen), also
  kein Code-Anordnungsproblem. Varianten `volatile u32 color`-Local und
  `g = graphics`-Local erreicht 99,60 %/96,67 % – schlechter.

- `THPPlayer/THPAudioDecode.c`: `AudioDecoderForOnMemory` (176 Bytes, 89,27 %).
  Register-Diff betrifft die gehoisteten `ActivePlayer`- und `AudioDecodeThread`-
  Pointer sowie den Frame-Index (`r28` ist beidseitig `readSize`; die echte
  Verschiebung ist `frame` in `r29` statt `r31`). `while(TRUE)` → `for(;;)`
  ist ein No-Op auf die Registerallokation; ein belastbarer Match setzt
  voraus, dass die lokalen Variablen in einer bestimmten Reihenfolge deklariert
  sind und der Compiler die Hoists nicht zusammenlegt.

- `MoveBG/MapObjPollution.cpp`: `loadAfter` (172 Bytes, 88,07 %).
  Quellaufruf von `registerRevivalTexStamp` arbeitet mit `int/short`-Parametern;
  eine forcierte `(s16)/(s32)`-Typisierung brachte keine Änderung. Differenz
  bleibt in `r29`/`r30`-Registerwahl und Reihenfolge der `addi`s innerhalb der
  Schleife.

## Gematchte GMSJ01-Funktionen

- `JSystem/JAudio/JAInterface/JAIBasic.cpp`:
  `JAIBasic::initDriver` — **100 %** (120 Bytes). Ein ungenutztes
  `char trash[2]` reproduziert den originalen 0x30-Byte-Stackframe; der
  übrige Inline-Code war bereits identisch.

- `JSystem/J3D/J3DGraphLoader/J3DMaterialFactory.cpp`:
  `J3DMaterialFactory::newNBTScale` — **100 %** (168 Bytes). Ein nach
  `dflt` deklariertes `char trash[8]` reproduziert Stackframe und
  Local-Offsets des Originals.

- `JSystem/J3D/J3DGraphLoader/J3DMaterialFactory_v21.cpp`:
  `J3DMaterialFactory_v21::newNBTScale` — **100 %** (168 Bytes).
  Derselbe nach dem Rückgabewert deklarierte `char trash[8]` gleicht
  Stackframe und Local-Offsets an; `.text` und `.sdata2` matchen vollständig.

- `MSound/MSoundBGM.cpp`: `MSBgm::init` — **100 %** (136 Bytes).
  `char trash[0x20]` stellt den originalen 0x48-Byte-Stackframe wieder her;
  danach matchen auch `.text`, `.data`, `.bss` und `.sdata` der Unit zu 100 %.

- `Enemy/egggen.cpp`: `TEggGenerator::control` — **100 %** (120 Bytes).
  `char trash[0x18]` reproduziert den 0x30-Byte-Stackframe; anschließend
  matchen `.text`, `.rodata`, `.data` und `.sdata2` der Unit zu 100 %.

- `Enemy/DebuTelesa.cpp`: `TDebuTelesa::receiveMessage` — **100 %**
  (176 Bytes). `char trash[8]` gleicht den Stackframe von 0x28 auf 0x30 Bytes an.

- `JSystem/JParticle/JPAParticle.cpp`:
  `JPAParticle::checkCreateChildParticle` — **100 %** (196 Bytes).
  `char trash[0x10]` stellt den 0x58-Byte-Stackframe wieder her; `.text`,
  `.data` und `.sdata2` matchen vollständig.

## Nächster GMSJ01-Kandidat

`Strategic/livemanager.cpp`: `TLiveManager::perform` (252 Bytes, 99,84 %) bleibt
offen; `AudioDecoderForOnMemory` und `MapObjPollution::loadAfter` sind
dokumentierte Nonmatches.
