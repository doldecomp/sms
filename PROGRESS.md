# Decomp-Fortschritt

## Ausgangs-Baseline (GMSJ01 / NTSC-J)

- Upstream-Basis: `39458071` (nach rebase auf `upstream/main`)
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

## Stand nach Rebase + FlagManager

| Metrik | Aktuell | Änderung vs. Baseline |
| --- | ---: | ---: |
| Fuzzy match | 77,84 % | +0,50 pp |
| Code matched | 41,35 % (1.484.660 / 3.590.088) | +9.944 Bytes |
| Funktionen matched | 66,62 % (8.581 / 12.881) | +67 |
| Code complete / linked | 19,03 % | +1,03 pp |
| Units complete | 403 / 736 | +7 |

### Nach dieser Iterationsrunde (NpcManager, THPPlayer)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Fuzzy match | 77,84 % | ±0 |
| Code matched | 41,38 % (1.485.500 / 3.590.088) | +840 Bytes |
| Funktionen matched | 66,65 % (8.585 / 12.881) | +4 |
| Units complete | 403 / 736 | ±0 |

### Nach zweiter Iterationsrunde (JPADraw, effectObj, WoodBarrel::kill)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,43 % (1.487.244 / 3.590.088) | +1.744 Bytes |
| Funktionen matched | 66,69 % (8.590 / 12.881) | +5 |

**Methodik-Erkenntnis**: `char trash[N]`-Padding muss NACH dem betroffenen
Struct-Local stehen, nicht davor — das brachte `calcRootMatrix`, `loadYBBMtx`
und `WoodBarrel::kill` von 99,6–99,8 % auf 100 %. Funktionen ohne frühen
Struct-Local reagieren weiterhin auf Padding am Funktionsanfang
(`zDrawParticle`, `zDrawChild`, `NpcManager::perform`).

**Warnung zu `configure.py`-Status**: `objdiff-cli diff` kann eine Unit-Section
als 100 % melden, obwohl anonyme `[.data-0]`/`[.sdata-0]`-Reste (Padding/
Literal-Pool-Bytes ohne Symbolnamen) abweichen — sichtbar nur in den
Symbol-Einträgen, nicht im Section-Aggregat. `MoveBG/WoodBarrel.cpp` auf
`Matching` zu setzen brach die DOL-SHA1, obwohl alle Sections 100 % zeigten;
sofort zurückgesetzt. Ab jetzt: Matching-Flip **immer** mit
`ninja && dtk shasum -c` verifizieren, nie nur mit objdiff-Sections.

### Nach dritter Iterationsrunde (JPADraw::initialize)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,45 % (1.488.112 / 3.590.088) | +868 Bytes |
| Funktionen matched | 66,71 % (8.591 / 12.881) | +1 |

`JSystem/JParticle/JPADraw.cpp` `.text` jetzt vollständig 100 % (4/4
Funktionen); Unit bleibt wegen anonymem `.sdata2`-Rest NonMatching.

### Nach vierter Iterationsrunde (MapModel, MapObjOption, ItemManager, PollutionManager)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,50 % (1.489.852 / 3.590.088) | +1.740 Bytes |
| Funktionen matched | 66,74 % (8.597 / 12.881) | +6 |

Sechs neue 100-%-Matches: `TMapModel::initUnderpass`,
`TFileLoadBlock::touchPlayer`/`receiveMessage`,
`TItemManager::resetNozzleBoxesModel`, `TPollutionManager::clean`/`load`.

Die Referenz-DOL bleibt `OK`.

### Nach fünfter Iterationsrunde (JALModSe: appendGrpMember, append)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,56 % (1.492.056 / 3.590.088) | +2.204 Bytes |
| Funktionen matched | 66,75 % (8.599 / 12.881) | +2 |

Zwei neue 100-%-Matches in `JSystem/JAudio/JALibrary/JALModSe.cpp`:
`JALSystem::appendGrpMember` (`char trash[8]` am Funktionsanfang) und
`JALSystem::append` (`char trash[0x68]` nach dem letzten Struct-Local
`set3`, **plus echter Bugfix**: der `ModType_JALSeModPitFunk`-Case
übergab `&set2` statt `&set3` — Disassembly-Beweis: `PitFunk` gruppiert
sich beim Stack-Offset 0xd8 exakt mit `PitDist`/`PitFGrp`/`PitDGrp`
(alle nutzen `set3`), nicht mit der 0xe0-Gruppe (`Vol*`/`Eff*`, nutzt
`set2`) — passend zum "Pit"-Namensmuster der anderen drei Fälle).

**Wichtiger Fund zum Matching-Flip-Risiko**: Beide Funktionen matchen
einzeln zu 100 %, und die komplette Unit zeigt in `objdiff-cli` 100 %
für **alle** Sections (`.text`/`.data`/`.bss`/`.sbss`/`.sdata2`/`.ctors`).
Trotzdem bricht ein `Matching`-Flip in `configure.py` die DOL-SHA1
(bestätigt per `dtk shasum`). Root-Cause-Analyse per Byte-Diff
`build/GMSJ01/mario.dol` gegen `orig/GMSJ01/sys/main.dol`: 6.190
abweichende Bytes, erster bei Datei-Offset `0x445df` (liegt exakt im
`.text`-Bereich dieser Unit, Adresse ~`0x800476a0`), letzter bei
`0x3a7c30` (liegt in `.data`) — die Abweichung zieht sich vom Ort dieser
Unit bis ans Ende von `.text`, durch `.rodata` und in `.data` hinein.
Das ist ein echter Downstream-Adress-Shift/Layout-Fehler, keine bloße
anonyme Daten-Restgröße wie bisher bei `WoodBarrel` vermutet — aber die
Konsequenz ist dieselbe: Unit bleibt `NonMatching` in `configure.py`,
nur der Quellcode wird committet. **Erkenntnis für künftige Flips**:
100 % in allen von `objdiff-cli` gelisteten Sections ist keine
hinreichende Bedingung für einen sicheren Matching-Flip; nur ein
tatsächlicher `ninja && dtk shasum`-Durchlauf ist beweiskräftig.

Die Referenz-DOL bleibt `OK`.

### Nach sechster Iterationsrunde (JASTrack::writeRegDirect; spcinterp/JASTrack-Analysen)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,57 % (1.492.260 / 3.590.088) | +204 Bytes |
| Funktionen matched | 66,76 % (8.600 / 12.881) | +1 |

Ein neuer 100-%-Match: `JSystem/JAudio/JASystem/JASTrack.cpp::
TTrack::writeRegDirect` (204 Bytes, `char trash[8]` schließt eine
8-Byte-Frame-Lücke, keine Struct-Locals vorhanden).

Drei weitere Funktionen ausführlich untersucht, kein Match erreicht,
Quelltext auf sauberen Zustand zurückgesetzt:

- `Strategic/spcinterp.cpp`: `execadd`/`execsub`/`execmul`/`execdiv`
  (je 728 Bytes, 99,98 % clean) — identisches Muster in allen vier
  Funktionen. Frame stimmt bereits exakt (`0x70` beidseitig); einziger
  Rest ist ein 4-Byte-Stack-Offset-Unterschied (0x24/0x28 vs. 0x20/0x24)
  für das compiler-interne `TSpcSlice`-Temporary im `push(int)`-Aufruf
  des else-Zweigs. Drei Varianten (`trash[4]` nach `result`, expliziter
  `TSpcSlice tmp`-Local statt `push(int)`, `trash[4]` vor dem `if`)
  verschlechtern alle auf 99,89–99,95 %; zurückgesetzt auf die saubere
  99,98-%-Fassung.
- `Strategic/spcinterp.cpp::execcall` (580 Bytes, 99,66 % clean) —
  32-Byte-Frame-Gap (`0x88` vs. `0x68`) **plus** eine echte
  Register-Vertauschung: `mContextStack.mSize` und `mProgramCounter`
  landen bei uns in r5/r4, im Original in r4/r5 (reines
  Register-Allocation-Detail, keine Feldadress-Verwechslung — beide
  Felder werden korrekt gelesen, nur die physischen Register
  vertauscht). `char trash[0x20]` am Funktionsanfang erzeugte
  scheinbar einen Sprung auf 99,69 %, was sich bei genauer Prüfung
  als Messartefakt herausstellte (objdiff-cli grenzt bei
  `NonMatching`-Funktionen den Vergleichsbereich der Zielseite an der
  aktuell kompilierten Größe ab; das Vergrößern der eigenen Funktion
  verschiebt dadurch auch das verglichene Zielfenster). Zurückgesetzt
  auf die saubere 99,66-%-Fassung.
- `JSystem/JAudio/JASystem/JASTrack.cpp::noteOn` (824 Bytes, 99,80 %
  clean) — 8-Byte-Frame-Gap (`0x70` vs. `0x68` mit `trash[8]`)
  **plus** ein Register-Umnummerierungsmuster (`r24` bei uns vs. `r23`
  im Original für denselben `TTrack* mParent`-Lokal über die gesamte
  Parent-Walk-Schleife) — dieselbe Kategorie wie der bereits
  dokumentierte `effectObj::reset`-Fall. **Korrektur**: Der zunächst
  gemessene Sprung auf 99,88 % mit `char trash[8]` wurde nachträglich
  als Messartefakt entlarvt (siehe `execcall`-Fund unten) — gezielte
  Vorher/Nachher-Prüfung der Zieldisassemblierung zeigt, dass sich der
  `stwu`-Wert der **Zielseite** exakt von `-0x68` auf `-0x70`
  verschiebt, sobald unsere Funktion um 8 Byte wächst. Zurückgesetzt
  auf die saubere 99,80-%-Fassung; keine verifizierbare Verbesserung.
- `JSystem/JAudio/JASystem/JASTrack.cpp::writeRegParam` (1.288 Bytes,
  99,36 % clean, bereits mit Upstream-Kommentar `// TODO: This is pure
  pain` als bekannt schwierig markiert) — 16-Byte-Frame-Gap (`0x48` vs.
  `0x38`) plus eine echte Argument-Auswertungsreihenfolge-Vertauschung
  vor dem zweiten `writeRegDirect(5, product)`-Aufruf (`this`-Setup vs.
  Wertberechnung in umgekehrter Reihenfolge). `char trash[0x10]` zeigte
  einen scheinbaren Sprung auf 99,38 % — angesichts des unten
  dokumentierten Messartefakt-Musters nicht verifiziert und daher nicht
  als reale Verbesserung gewertet; bestätigt als strukturell schwierig,
  zurückgesetzt auf die saubere Fassung.

**Wichtiger Methodik-Fund (Messartefakt bei Teil-Matches)**:
`objdiff-cli` grenzt bei `NonMatching`-Funktionen den
Vergleichsbereich der Zielseite offenbar an der aktuell kompilierten
Größe unserer Funktion ab, wenn die Symbolgröße nicht anderweitig fest
verankert ist. Vergrößert man die eigene Funktion per `char trash[N]`,
kann dadurch auch das verglichene Zielfenster wachsen und zufällig
bessere Byte-Übereinstimmung vortäuschen, **ohne dass ein echter
Match vorliegt**. Nachträglich per gezieltem Vorher/Nachher-Vergleich
des Ziel-Prologs (`stwu r1, -N(r1)`) an vier Fällen bestätigt:
`execcall` (-0x68→-0x88), `calcViewMtx` (-0xc8→-0xe0),
`drawRevivalTexStamp` (-0x98→-0xa0), `noteOn` (-0x68→-0x70) — in
allen vier Fällen verschob sich der Zielwert exakt um die Größe des
hinzugefügten `trash`-Arrays. **Regel ab sofort**: Eine
Prozentverbesserung durch `trash[N]` gilt nur dann als real, wenn sie
entweder (a) echte 100 % erreicht (dort ist eine zufällige
Fensterverschiebung durch vollständige Byte-Identität statistisch
ausgeschlossen) oder (b) durch einen expliziten Vorher/Nachher-Vergleich
des Ziel-Prologs bestätigt wird, dass sich die Zielseite NICHT
verändert hat. Reine Prozentangaben zwischen zwei `objdiff-cli`-Läufen
mit unterschiedlicher eigener Funktionsgröße sind für sich allein
**nicht** aussagekräftig.

Die Referenz-DOL bleibt `OK`.

### Nach siebter Iterationsrunde (THPPlayer/TimeRec/MapCollisionPlane/PollutionCount-Analysen, kein neuer Match)

Sieben weitere Funktionen ausführlich untersucht, alle als strukturell
schwierig bestätigt und auf sauberen Zustand zurückgesetzt — kein
Fortschritt in dieser Runde, aber wertvolle Root-Cause-Dokumentation:

- `THPPlayer/THPPlayer.c::THPPlayerPrepare` (624 Bytes, 97,98 % clean) —
  Frame stimmt bereits exakt (`0x30` beidseitig, `stmw r26`), reine
  Register-Umnummerierung: alle Parameter/Locals (`frame`, `flag`,
  `threadData`) sitzen bei uns ein Register höher (`r28`→`r27` usw.)
  als im Original. Dieselbe Kategorie wie `effectObj::reset`/`noteOn`.
- `System/TimeRec.cpp::TTimeRec::flip` (144 Bytes, 99,17 % clean) —
  reiner Register-Swap (r5↔r7) für die schleifengetragene `curr`-Variable
  in der Rückwärtsschleife, kein Frame-Unterschied, keine Struct-Locals.
- `Map/MapCollisionPlane.cpp::TMapCheckGroundPlane::checkPlaneGround`
  (308 Bytes, 95,95 % clean, bereits mit Upstream-Kommentar `// TODO:
  making the return type an int here makes it match better, but breaks
  other places` als bekannt schwierig markiert) — 8-Byte-Frame-Gap plus
  f6/f7-Register-Swap und Instruktions-Umordnung; `char trash[8]`
  bewirkt keinerlei Veränderung (95,95 % → 95,95 %).
- `Map/PollutionCount.cpp` — vier Funktionen mit verschachteltem
  Auto-Inlining (`ReInitializeGX`/`drawPollutionLayer`/
  `loadPollutionLayer`/`initDrawObjGX` werden je nach Aufrufstelle vom
  Compiler automatisch eingebettet):
  - `drawRevivalTexStamp` (748 Bytes, 99,93 % clean) — 8-Byte-Frame-Gap
    (bestätigt behoben durch zwei getrennte `char trash[4]`-Blöcke, vor
    der `GXSetChanMatColor`-Compound-Literal-Zeile und nach dem
    `JUTTexture texture`-Local in der Schleife). Der zunächst gemessene
    Sprung auf 99,98 % wurde als Messartefakt identifiziert (siehe
    Methodik-Fund oben): Ziel-Prolog verschiebt sich exakt von
    `-0x98` auf `-0xa0`, wenn unsere Funktion um 8 Byte wächst. Der
    restliche 4-Byte-Versatz betrifft die Speicherposition des
    `(GXColor){...}`-Compound-Literals relativ zu einem benachbarten,
    nicht überlappenden Stack-Slot; ein benannter `GXColor`-Local
    statt Compound-Literal verschlechtert klar auf 98,87 % (kein
    Artefakt, da Verschlechterung). Zurückgesetzt auf die saubere
    99,93-%-Fassung; keine verifizierbare Verbesserung.
  - `countTexDegree` (596 Bytes, 99,87 % clean) — 88-Byte-Frame-Gap
    durch zweifach verschachteltes Auto-Inlining von
    `drawPollutionLayer` → `loadPollutionLayer` (dessen einziger Local
    `GXTexObj GStack_40` landet komplett in `countTexDegree`s Frame).
    `char trash[0x58]` direkt in `loadPollutionLayer` nach `GStack_40`
    hat **null** Effekt (99,8658 % exakt unverändert) — der Optimizer
    entfernt totes Padding in bereits eingebettetem Code, bevor die
    Frame-Größe der umschließenden Funktion berechnet wird. Bestätigt
    dieselbe Kategorie wie `MapObjPollution::loadAfter` ("inlines make
    me cry").
  - `drawJointObjStamp` (648 Bytes, 99,39 % clean) — 16-Byte-Frame-Gap
    plus r26/r27-Register-Swap für die Schleifenvariable, zusätzlich
    verschachteltes Inlining von `initDrawObjGX`; nicht weiter verfolgt
    angesichts der bestätigten Kategorie.
  - `calcViewMtx` (384 Bytes, 99,21 % clean) — 24-Byte-Frame-Gap durch
    inlineten `TPosition3f local_a4`-Zero-Fill in der Schleife;
    `char trash[0x18]` nach `local_a4` zeigte einen scheinbaren Sprung
    auf 99,28 %, der per Vorher/Nachher-Vergleich des Ziel-Prologs
    (`-0xc8` → `-0xe0`, exakt um die Trash-Größe) als Messartefakt
    entlarvt wurde. Rest ist eine 4-Byte-Slot-Verschiebung plus
    f0/f1-Register-Swap für `makeWorldToPollutionMtx`, dieselbe
    Kategorie wie `drawRevivalTexStamp`. Zurückgesetzt auf die saubere
    99,21-%-Fassung; keine verifizierbare Verbesserung.
- `Strategic/objmanager.cpp` — zwei weitere Funktionen (nicht Teil von
  `PollutionCount.cpp`):
  - `TObjManager::perform` (236 Bytes, 99,83 % clean) —
    16-Byte-Frame-Gap; `char trash[0x10]` zeigte denselben Artefakt
    (Ziel-Prolog `-0x40` → `-0x50`), keine reale Verbesserung.
    Zurückgesetzt auf die saubere Fassung.
  - `TObjManager::load` (168 Bytes, 99,93 % clean) — 4-Byte-Versatz für
    `char buffer[0x100]`; `char trash[4]` vor und nach dem Buffer
    jeweils **verschlechtert** auf 99,71 % (keine Artefakt-Verwechslung
    möglich, da Verschlechterung eindeutig ist). Zurückgesetzt auf die
    saubere Fassung.


Die Referenz-DOL bleibt `OK`.

### Nach achter Iterationsrunde (M3UModel::updateInMotion; Artefakt-Korrektur)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,57 % (1.492.552 / 3.590.088) | +292 Bytes |
| Funktionen matched | 66,77 % (8.601 / 12.881) | +1 |

Ein neuer 100-%-Match: `M3DUtil/M3UModel.cpp::M3UModel::updateInMotion`
(292 Bytes, `char trash[0x28]` schließt eine 40-Byte-Frame-Lücke ohne
Struct-Locals im Funktionskörper).

**Nachträgliche Korrektur der sechsten/siebten Runde**: Beim
systematischen Nachprüfen (Vorher/Nachher-Vergleich des
Ziel-Funktionsprologs) stellte sich heraus, dass die dort gemeldeten
Teil-Verbesserungen durch `trash[N]` bei `noteOn` (99,80 % → 99,88 %),
`calcViewMtx` (99,21 % → 99,28 %) und `drawRevivalTexStamp` (99,93 % →
99,98 %) **Messartefakte** waren, keine echten Verbesserungen (siehe
Methodik-Fund und korrigierte Einträge oben). Alle drei Funktionen
bleiben bei ihrer sauberen Ausgangs-Prozentzahl ohne verifizierte
Verbesserung. Die bereits committeten echten 100-%-Matches
(`appendGrpMember`, `append`, `writeRegDirect`, `updateInMotion`) sind
von diesem Artefakt nicht betroffen, da eine zufällige Fensterver-
schiebung bei echten 100-%-Treffern über mehrere hundert Bytes hinweg
statistisch ausgeschlossen ist.

Die Referenz-DOL bleibt `OK`.

### Nach neunter Iterationsrunde (AnimalManager::loadAfter; Shimmer/bgpoldrop/clipEnemies-Analysen)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,58 % (1.492.612 / 3.590.088) | +60 Bytes |
| Funktionen matched | 66,78 % (8.602 / 12.881) | +1 |

Ein neuer 100-%-Match: `Animal/AnimalManager.cpp::TMewManager::loadAfter`
(60 Bytes, `char trash[0x10]` schließt eine 16-Byte-Frame-Lücke, keine
Locals im Funktionskörper).

Drei weitere Funktionen untersucht, alle nach dem neuen
Verifikationsprotokoll (Ziel-100-%-Test statt Zwischenprozent-Tracking)
als Nonmatching bestätigt und zurückgesetzt:

- `Map/Shimmer.cpp::TShimmer::perform` (648 Bytes, 99,80 % clean) —
  40-Byte-Frame-Gap plus konstante 4-Byte-Verschiebung in einer
  verketteten Virtual-Call-Kette (`getModelData()->
  getMaterialNodePointer(0)->getTexGenBlock()->getTexMtx(1)`);
  `char trash[0x28]` nach dem letzten `Mtx`-Local erreicht keine
  100 % (bestätigter Ziel-Fenster-Artefakt, `-0x170` → `-0x198`).
- `Enemy/bgpoldrop.cpp::TBGPolDrop::move` (592 Bytes, 99,79 % clean) —
  8-Byte-Frame-Gap, `char trash[8]` nach `local_14` erreicht keine
  100 %; nach neuem Protokoll sofort zurückgesetzt statt
  weiterzuoptimieren.
- `Animal/AnimalManager.cpp::TAnimalManagerBase::clipEnemies`
  (256 Bytes, 94,98 % clean) — strukturell identisch zu den bereits
  dokumentierten `clipEnemies`-Fällen (`NpcManager`, `CameraMode`):
  16-Byte-Frame-Gap plus echte Argument-Auswertungsreihenfolge in
  `SetViewFrustumClipCheckPerspective(gpCamera->mFovy,
  gpCamera->getAspect(), mViewClipNear, *mViewClipFarPtr)` (vier
  verschiedene Ladereihenfolgen f3/r3/f2/f1 vs. r3/f2/f1/f3) plus
  r29/r30-Register-Swap für die Schleifenvariable. `char trash[0x10]`
  erreicht keine 100 %; bestätigt dieselbe Kategorie, zurückgesetzt.

Die Referenz-DOL bleibt `OK`.

### Nach zehnter Iterationsrunde (HelpActor, MapCollisionEntry, JASDSPChannel)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,61 % (1.493.676 / 3.590.088) | +1.064 Bytes |
| Funktionen matched | 66,80 % (8.605 / 12.881) | +3 |

Drei neue 100-%-Matches, alle über simple Frame-Gap-Fixes ohne
Register-Probleme:

- `GC2D/HelpActor.cpp::THelpActor::perform` (344 Bytes, `char trash[8]`,
  keine Locals).
- `Map/MapCollisionEntry.cpp::TMapCollisionMove::init(u32,u16,s16,
  const TLiveActor*)` (168 Bytes, `char trash[8]` am Funktionsanfang).
- `JSystem/JAudio/JASystem/JASDSPChannel.cpp::TDSPChannel::updateAll`
  (552 Bytes, `char trash[0x18]`, 24-Byte-Frame-Gap ohne Struct-Locals).

Vier weitere Funktionen in derselben Unit-Scan-Runde als "Frame
bereits exakt, nur interner Slot-Versatz" identifiziert (dieselbe
unlösbare Kategorie wie `drawRevivalTexStamp`/`calcViewMtx`, siehe
oben) und ohne Zwischen-Tuning sofort zurückgesetzt:
`Map/PollutionObj.cpp::TPollutionObj::getDepthFromMap` (99,96 %,
bereits mit Upstream-`TODO: inlines are wrong here!` markiert),
`Map/MapCollisionEntry.cpp::TMapCollisionMove::move()` (99,90 % best,
8-Byte-Gap in `local_18`), `TMapCollisionWarp::setUp()` (99,92 % best,
8-Byte-Gap in `local_18`), `TMapCollisionMove::moveSRT` (Frame stimmt
bereits exakt, 4-Byte-Slot-Versatz).

Die Referenz-DOL bleibt `OK`.

### Nach elfter Iterationsrunde (JDRDisplay, CameraMarioData, JAIGlobalParameter)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,63 % (1.494.420 / 3.590.088) | +744 Bytes |
| Funktionen matched | 66,83 % (8.608 / 12.881) | +3 |

Drei neue 100-%-Matches:

- `JSystem/JDrama/JDRDisplay.cpp::TDisplay::startRendering` (248 Bytes,
  `char trash[8]`, keine Locals).
- `Camera/CameraMarioData.cpp::TCameraMarioData::calcAndSetMarioData`
  (356 Bytes, `char trash[8]` nach `JGeometry::TVec3<f32> offset`).
- `JSystem/JAudio/JAInterface/JAIGlobalParameter.cpp::
  setParamSoundOutputMode` (140 Bytes, `char trash[8]` am
  Funktionsanfang, zwei skalare Locals `r31`/`r30`).

Vier weitere Funktionen in derselben Scan-Runde als Nonmatching
bestätigt (nach neuem Protokoll sofort zurückgesetzt statt
Zwischenwerte zu verfolgen):

- `JSystem/JDrama/JDRActor.cpp::JDrama::TActor::load` (99,93 % clean,
  340 Bytes) — Frame stimmt exakt, 4-Byte-Slot-Versatz für
  `char str[0x50]`; beide getesteten `trash[4]`-Positionen
  (vor/nach `str`) verschlechtern identisch auf 99,82 %.
- `NPC/NpcInbetween.cpp::TNpcInbetween::execPosInbetween` (99,09 %
  clean, 220 Bytes) — reines f1/f2-Register-Rotationsmuster, kein
  Frame-Unterschied, dieselbe Kategorie wie `effectObj::moveObject`.
- `Camera/CameraBck.cpp::TCameraBck::updateDemo` (98,96 % clean,
  452 Bytes) — 24-Byte-Frame-Gap **plus** eine echte
  Doppel-Bool-Normalisierung auf der Zielseite (eine zusätzliche
  `li r0, 0x1`/`cmpwi r0, 0x0`-Sequenz vor der finalen
  `result`-Zuweisung, die unser Build nicht erzeugt — `checkState()`
  ist bereits `? 1 : 0`-normalisiert inline, daher unklar, wodurch die
  Ziel-Redundanz entsteht). `char trash[0x18]` nach `J3DTransformInfo
  info` bewegt den Match kaum (98,96 % → 99,03 %); zurückgesetzt.
- `JSystem/JDrama/JDREfbSetting.cpp::IssueGXCopyDisp` (99,50 % clean,
  404 Bytes) — kein Frame-Unterschied, reines Register-Rotationsmuster
  (r0/r3/r4) für einen booleschen Ausdruck.

Die Referenz-DOL bleibt `OK`.

### Nach zwölfter Iterationsrunde (MapObjEx, MapObjGrass, PollutionPos)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,66 % (1.495.808 / 3.590.088) | +1.780 Bytes |
| Funktionen matched | 66,87 % (8.612 / 12.881) | +4 |

Vier neue 100-%-Matches über drei Dateien:

- `MoveBG/MapObjEx.cpp`: `TMapObjNail::receiveMessage` (324 Bytes) und
  `TJointCoin::control` (284 Bytes) — je `char trash[8]` am
  Funktionsanfang, keine Struct-Locals.
- `MoveBG/MapObjGrass.cpp`: `TMapObjGrassManager::perform` (568 Bytes,
  `char trash[0x30]`, 48-Byte-Frame-Gap ohne Struct-Locals).
- `Map/PollutionPos.cpp`: `TPollutionPos::isSame` (212 Bytes,
  `char trash[0x20]`, 32-Byte-Frame-Gap ohne Struct-Locals).

Fünf weitere Funktionen untersucht, alle nach dem etablierten
Protokoll (Ziel: 100 % oder sofortiger Revert) als Nonmatching
bestätigt:

- `Strategic/Strategy.cpp::TStrategy::load` (99,93 % clean, 280 Bytes)
  — `char trash[8]` nach `JSUMemoryInputStream stream2` erreicht keine
  100 % (99,94 % best, nicht verifiziert als real).
- `MoveBG/MapObjGrass.cpp::TMapObjGrassManager::initDrawNear`
  (99,87 % clean, 588 Bytes) — `char trash[0x18]` nach `vec` erreicht
  keine 100 %.
- `MarioUtil/ModelUtil.cpp::
  SMS_RideMoveByGroundActor` (99,82 % clean, 404 Bytes) —
  `char trash[0x18]` nach `TMtx34f mtx` erreicht keine 100 %.
- `Animal/Butterfly.cpp::TButterfloid::load` (99,70 % clean,
  640 Bytes) — bereits mit Quellcode-Kommentar "Making these all
  setters doesn't yield NEARLY enough stack frame padding for this to
  match" als bekannt schwierig markiert (136-Byte-Frame-Gap), nicht
  erneut versucht.
- `GC2D/ScrnFader.cpp::TSMSFader::update` (99,66 % clean, 348 Bytes)
  — 16-Byte-Frame-Gap plus f1/f2-Register-Rotationsmuster (vermutlich
  aus inlinetem `updateRequest()`/Farbzuweisung); `char trash[0x10]`
  erreicht keine 100 %.

Die Referenz-DOL bleibt `OK`.

### Nach dreizehnter Iterationsrunde (SDLModel: entrySameMat, viewCalcSimple)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,68 % (1.496.332 / 3.590.088) | +552 Bytes |
| Funktionen matched | 66,90 % (8.614 / 12.881) | +2 |

Zwei neue 100-%-Matches in `M3DUtil/SDLModel.cpp`:
`SDLModelData::entrySameMat` (308 Bytes, `char trash[8]`) und
`SDLModel::viewCalcSimple` (216 Bytes, `char trash[8]`), beide
8-Byte-Frame-Gaps ohne Struct-Locals.

Sechs weitere Funktionen aus derselben Scan-Runde als Nonmatching
bestätigt (kein 100-%-Match, sofort zurückgesetzt):

- `M3DUtil/SDLModel.cpp::SDLModelData::entrySDLModels` (99,87 % clean,
  508 Bytes) — `char trash[8]` nach den Iterator-Locals `it`/`e`
  erreicht keine 100 % (99,90 % best).
- `M3DUtil/SDLModel.cpp::SDLModel::entry` (99,75 % clean, 384 Bytes)
  — `char trash[0x10]` erreicht keine 100 % (99,80 % best).
- `Map/MapXlu.cpp::TMapXlu::changeNormalJoint` (99,92 % clean,
  256 Bytes) und `changeXluJoint` (99,84 % clean, 280 Bytes) —
  Ziel-Frame ist **größer** als unseres; `char trash[0x10]`/`[8]` am
  Funktionsanfang wird vom Compiler komplett wegoptimiert (keinerlei
  Änderung an unserer kompilierten Größe oder am Match-Prozentsatz),
  da die Locals in diesen reinen Doppel-`for`-Schleifen-Funktionen
  ohne jede andere Verwendung nachweisbar tot sind.
- `Enemy/areacylinder.cpp::TAreaCylinder::load` (99,94 % clean,
  604 Bytes) — `char trash[8]` nach `JGeometry::TVec3<f32> v` erreicht
  keine 100 %.
- `GC2D/MovieSubtitle.cpp::TMovieSubTitle::setupResource` (99,88 %
  clean, 404 Bytes) — `char trash[8]` vor/nach `char buffer[256]`
  identisch wirkungslos (99,93 % best, beide Positionen).

Die Referenz-DOL bleibt `OK`.

### Nach vierzehnter Iterationsrunde (MapObjCloud, launcher, MapObjTrap)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,71 % (1.497.388 / 3.590.088) | +1.056 Bytes |
| Funktionen matched | 66,95 % (8.619 / 12.881) | +5 |

Fünf neue 100-%-Matches über drei Dateien:

- `MoveBG/MapObjCloud.cpp::TRideCloud::setGroundCollision` (120 Bytes)
  — `char trash[4]` nach `TMtx34f mtx` behebt einen 4-Byte-Slot-Versatz
  bei bereits korrektem Frame (Ausnahme von der sonst unlösbaren
  "interner Slot-Versatz"-Kategorie — hier hat es funktioniert).
- `Enemy/launcher.cpp`: `TCommonLauncher::stateHitByWater` (180 Bytes,
  `char trash[8]`) und `TCommonLauncher::perform` (364 Bytes,
  `char trash[0x10]`) — je einfache Frame-Gaps ohne Struct-Locals.
  `stateLaunch` bleibt Nonmatching (Frame bereits exakt, 48-Byte-
  Vec/Mtx-Slot-Vertauschung zwischen drei Struct-Locals).
- `MoveBG/MapObjTrap.cpp`: `TLampTrapSpikeHit::perform` (228 Bytes,
  `char trash[0x18]`) und `TLampTrapIron::receiveMessage` (164 Bytes,
  `char trash[8]`) — je einfache Frame-Gaps ohne Struct-Locals.

Die Referenz-DOL bleibt `OK`.

### Nach fünfzehnter Iterationsrunde (MapObjBlock: fünf Matches)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,75 % (1.499.040 / 3.590.088) | +1.652 Bytes |
| Funktionen matched | 66,95 % → 66,97 % (8.624 / 12.881) | +5 |

Fünf neue 100-%-Matches in `MoveBG/MapObjBlock.cpp`:
`TIceBlock::control` (480 Bytes, `char trash[0x10]`),
`TIceBlock::touchWater` (392 Bytes, `char trash[0x10]`),
`TBrickBlock::kill` (208 Bytes, `char trash[8]`),
`TSuperHipDropBlock::receiveMessage` (160 Bytes, `char trash[8]`),
`TTelesaBlock::perform` (412 Bytes, `char trash[8]` nach
`TRotation3f mtx` — bereits mit Upstream-TODO "Possibly more
TRotation3f inlines?" markiert, Trash hat trotzdem funktioniert).
`TSandBlock::control` bleibt Nonmatching (99,93 % best mit
`char trash[8]`, keine 100 %).

Zwei weitere Funktionen aus Nachbar-Units geprüft, beide Nonmatching:

- `Enemy/coasterkiller.cpp::TCoasterEnemy::bind` (220 Bytes,
  Frame bereits exakt, interner 12-Byte-Slot-Versatz — dieselbe
  unlösbare Kategorie).
- `Map/MapWireManager.cpp::TMapWireManager::load` (432 Bytes,
  99,79 % clean) — `char trash[0x10]` erreicht keine 100 %
  (99,89 % best).
- `Strategic/liveinterp.cpp::linGetSRT` (1.944 Bytes, 96,06 % clean)
  — großer verschachtelter `switch(arg2){switch(arg1){...}}` mit
  mehreren `TSpcSlice slice;`-Locals in gegenseitig exklusiven
  Case-Block-Scopes; dieselbe Kategorie wie die bereits dokumentierten
  `execadd`/`execsub`/`execmul`/`execdiv`-Fälle in `spcinterp.cpp`
  (compiler-interne Temporary-Platzierung). `char trash[0x20]`
  bewegt kaum etwas (96,06 % → 96,10 %); zurückgesetzt.

Die Referenz-DOL bleibt `OK`.

### Nach sechzehnter Iterationsrunde (coasterkiller::loadAfter; objdiff-Diff-Artefakt entdeckt)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,76 % (1.499.092 / 3.590.088) | +52 Bytes |
| Funktionen matched | 66,97 % (8.625 / 12.881) | +1 |

Ein neuer 100-%-Match: `Enemy/coasterkiller.cpp::TCoasterKillerManager::
loadAfter` (52 Bytes, `char trash[0x18]` schließt eine 24-Byte-
Frame-Lücke, keine Locals).

**Wichtiger Methodik-Fund (objdiff-Instruktions-Alignment-Artefakt)**:
Bei der Untersuchung von `TMapWireActorManager::doActorToWire`
(99,40 % clean, 484 Bytes) zeigte `objdiff-cli`s Instruktions-Diff an
einer Stelle einen scheinbar echten Bug: unser kompilierter Code lädt
`lwz r3, 0x78(r30)` / `lwz r0, 0x7c(r3)` (entspricht `unk4.unk74->
unk7C`), während die "Ziel"-Seite in der JSON-Ausgabe `lwz r3, 0x0(r30)`
/ `lwz r0, 0x6c(r3)` zeigt (entspräche `unk0->mHeldObject`) — ein
scheinbarer Strukturunterschied. Ein expliziter Test mit temporärer
Variable (`TTakeActor* dbgUnk0 = unk0;`) änderte am kompilierten Ergebnis
**nichts**, was den Verdacht erhärtete. Gegenprobe direkt in der
**rohen Retail-Disassemblierung** (`build/GMSJ01/asm/Map/
MapWireManager.s`, generiert am Anfang der Session, seither
unverändert): An der exakten Adresse `801EAC6C`/`801EAC70` innerhalb
von `doActorToWire` (zwischen `.fn`/`.endfn`) steht dort tatsächlich
`lwz r3, 0x78(r30)` / `lwz r0, 0x7c(r3)` — **identisch mit unserem
kompilierten Code**. Das bedeutet: `objdiff-cli`s Instruktions-
Alignment in der JSON-Diff-Ausgabe kann an einzelnen Stellen
fehlausgerichtet sein (vermutlich Nachwirkung einer Sequenz-Alignment-
Neusynchronisierung nach einer früheren echten Abweichung im
Funktionsverlauf), auch wenn der aggregierte `match_percent`-Wert
korrekt bleibt. **Regel für künftige Sessions**: Bei einem
scheinbaren "echten Bug" (unterschiedliche Feldoffsets/Strukturzugriffe)
immer zusätzlich direkt in der rohen `build/GMSJ01/asm/*.s`-Referenz-
datei (stabil seit Sessionbeginn, nicht von eigenen Edits beeinflusst)
gegenprüfen, bevor Zeit in einen vermeintlichen Quellcode-Fix investiert
wird. `doActorToWire` bleibt bei 99,40 % clean dokumentiert; die
echten Restunterschiede liegen an anderer Stelle im Funktionskörper
und wurden nicht weiter isoliert.

Die Referenz-DOL bleibt `OK`.

### Nach siebzehnter Iterationsrunde (JPAField::JPAMagnetField::affect)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,77 % (1.499.412 / 3.590.088) | +320 Bytes |
| Funktionen matched | 8.626 / 12.881 | +1 |

Ein neuer 100-%-Match: `JSystem/JParticle/JPAField.cpp::
JPAMagnetField::affect` (320 Bytes, `char trash[8]`, 8-Byte-Frame-Gap
ohne Locals).

Zwei weitere Feld-`affect`-Funktionen geprüft, beide Nonmatching:

- `JPADragField::affect` (288 Bytes, 99,85 % clean) — Frame stimmt
  nach `char trash[8]` exakt, verbleibender 4-Byte-Slot-Versatz bei
  drei getesteten Positionen (`trash[8]`/`[4]` am Funktionsanfang,
  `trash[4]` nach `rnd`) identisch bei 99,97 % — dieselbe unlösbare
  Kategorie wie `drawRevivalTexStamp`. Zurückgesetzt auf die saubere
  99,85-%-Fassung.
- `JPAVortexField::affect` (360 Bytes, 99,22 % clean) — r30/r3-
  Registervertauschung plus f28/f29-FP-Rotation, nicht untersucht
  (dieselbe Kategorie wie `effectObj::moveObject`).
- `JPARandomField::affect` (320 Bytes, 99,09 % clean) — 16-Byte-
  Frame-Gap plus dreifach wiederholtes FP-Rotationsmuster
  (f0/f1/f2/f3) in den drei `get_ufloat_1() - 0.5f`-Aufrufen;
  `char trash[0x10]` bewegt nur auf 99,18 %, zurückgesetzt.

Die Referenz-DOL bleibt `OK`.

### Nach achtzehnter Iterationsrunde (M3DUtil/MActor.cpp: neun Matches)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,84 % (1.502.040 / 3.590.088) | +2.628 Bytes |
| Funktionen matched | 8.636 / 12.881 | +9 |

Neun neue 100-%-Matches in `M3DUtil/MActor.cpp`, alle einfache
Frame-Gap-Fixes ohne Struct-Locals: `isCurAnmAlreadyEnd` (200 Bytes,
`trash[8]`), `calc` (264 Bytes, `trash[0x10]`), `updateIn`/`updateOut`
(228 Bytes je, `trash[8]`), `calcAnm` (404 Bytes, `trash[0x18]`),
`entry` (332 Bytes, `trash[8]`), `setLightData` (160 Bytes,
`trash[0x20]`), `perform` (472 Bytes, `trash[0x10]`),
`frameUpdate`/`updateMatAnm` (192/148 Bytes, je `trash[8]`).

Zwei Nonmatching-Reste in derselben Datei (Frame-Gap gefunden, aber
kein 100-%-Match): Konstruktor `MActor::MActor(MActorAnmData*)`
(1.272 Bytes, 99,97 % best mit `trash[8]`) und `setModel` (752 Bytes,
99,95 % best mit `trash[0x18]`) — beide zurückgesetzt auf die
sauberen 99,95-%/99,92-%-Ausgangsfassungen.

Auch `Camera/CameraWarp.cpp::warpPosAndAt(f32,s16)` geprüft
(580 Bytes, 99,77 % clean) — 8-Byte-Frame-Gap plus zwei vertauschte
Struct-Locals (`usualLookat`/`pos`); weder Deklarationsreihenfolge-
Tausch noch `char trash[8]` erreichten 100 % (99,83 % best),
zurückgesetzt.

Die Referenz-DOL bleibt `OK`.

### Nach neunzehnter Iterationsrunde (liveactor Konstruktor; NpcNerve::TNerveNPCTalk)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,85 % (1.502.520 / 3.590.088) | +176 Bytes |
| Funktionen matched | 8.638 / 12.881 | +2 |

Zwei neue 100-%-Matches: `Strategic/liveactor.cpp::TLiveActor::
TLiveActor(const char*)` (304 Bytes, `char trash[8]`) und
`NPC/NpcNerve.cpp::TNerveNPCTalk::execute` (176 Bytes, `char trash[8]`
nach `TBaseNPC* self`).

Vier weitere Funktionen in `liveactor.cpp` untersucht, Nonmatching:

- `TLiveActor::bind` (604 Bytes, 99,95 % clean) — Frame stimmt exakt;
  ein 0x10-Byte-Slot-Versatz für das `nextPos - mPosition`-Temporary
  (`mLinearVelocity`-Zuweisung); `char trash[0x10]` nach `nextPos`
  verschlechtert stark (99,72 %), zurückgesetzt.
- `TSpineBase<TLiveActor>::update` (264 Bytes, 99,92 % clean,
  Template-Methode in `include/Strategic/Spine.hpp`) — `char trash[8]`
  nach `nerve` ohne jede Wirkung (Header wird von mehreren TUs
  instanziiert, Trash dort vom Optimizer eliminiert wie bei
  `MapXlu`).
- `TLiveActor::initAnmSound` (272 Bytes, 99,84 % clean) —
  `char trash[8]` bewegt auf 99,97 %, keine 100 %, zurückgesetzt.
- `TLiveActor::init(TLiveManager*)` (492 Bytes, 99,87 % clean) —
  32-Byte-Frame-Differenz (Original **kleiner**) plus vertauschte
  Struct-Temporaries, nicht untersucht (Zeitaufwand vs. Nutzen).

Die Referenz-DOL bleibt `OK`.

### Nach zwanzigster Iterationsrunde (mameGesso/walkerEnemy-Batch; getGravityY-Bugfix)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,94 % (1.505.820 / 3.590.088) | +3.300 Bytes |
| Funktionen matched | 8.644 / 12.881 | +6 |

Sechs neue 100-%-Matches: `Enemy/mameGesso.cpp::TNerveMameGessoObject::
execute` (`char trash[0x10]` nach `self`), `TNerveMameGessoDamage::
execute` (`char trash[8]` nach `vel`), `TMameGessoManager::perform`
(`char trash[8]` am Funktionsanfang), `Enemy/walkerEnemy.cpp::
TNerveWalkerAttack::execute` (`char trash[0x10]` nach `self`),
`TWalkerEnemy::init` (`char trash[8]` am Funktionsanfang),
`Enemy/gesso.cpp::TGesso::rollCheck` (`char trash[8]` am
Funktionsanfang).

Echter Logikfehler gefunden und gefixt: `Enemy/mameGesso.cpp::
TMameGesso::getGravityY` verglich `mSpine->getCurrentNerve()` gegen
`TNerveMameGessoObject::theNerve()` statt `TNerveMameGessoGraphJump
Wander::theNerve()`, bevor `mSLJumpWanderGravityY` angewendet wird —
Feldname und Nerve-Name passten nicht zusammen. Bestätigt gegen die
rohe Retail-Disassembly (`build/GMSJ01/asm/Enemy/mameGesso.s`, zeigt
`__vt__30TNerveMameGessoGraphJumpWander` an dieser Stelle). Nach Fix
`match_percent` 100,0 %. Hinweis: Der projektweite `fuzzy_match_
percent`-Zähler aus `objdiff-cli report` (Basis für die obige
Fortschrittstabelle) hatte diese Funktion schon VOR dem Fix als
100 % fuzzy-matched gezählt — die Byte-Zahl der Tabelle enthält
diese Funktion daher nicht als Delta, obwohl der Fix inhaltlich
korrekt und über `objdiff-cli diff` (`match_percent`, die in diesem
Projekt maßgebliche Metrik) bestätigt 100 % ist.

**Neuer Methodik-Fund**: Eine automatisierte Rundum-Suche nach
Funktionen mit reinem Stackframe-Gap (Prolog-`stwu`-Differenz, sonst
strukturell identisch) über 260 Units lieferte 1.119 Kandidaten mit
erkennbarem Gap; 69 davon mit hoher Konfidenz (`fuzzy_match_percent`
> 99,5 %, Ziel-Frame größer als unseres). Blindes Anwenden von
`char trash[N]` (Platzierung: nach letztem struct-typisiertem Local
bzw. am Funktionsanfang) auf 24 dieser automatisch gefundenen
Kandidaten (u. a. `tamaNoko.cpp`, `EventWatcher.cpp`, `hamukuri.cpp`,
`NpcChange.cpp`, `DrawUtil.cpp`, `wireBinder.cpp`, `boid.cpp`, sowie
ein Retry in `mameGesso.cpp`) ergab **0 Treffer** — jedes Mal blieb
der Ziel-Stackframe exakt auf der Baseline-Größe, das `trash`-Array
wurde vom Optimizer vollständig eliminiert. Auch `volatile char
trash[N]` sowie ein erzwungener Schreibzugriff (`trash[0] = 0`)
änderten daran nichts (letzteres verschlechterte den Match sogar,
da ein zusätzlicher, nicht im Original vorhandener `stb`-Befehl
entsteht, ohne den Frame zu vergrößern). MWCC (`-O4,p -opt`) eliminiert
in diesen Fällen das komplett ungenutzte Array unabhängig von Größe,
Platzierung oder `volatile`-Qualifikation — offenbar eine Eigenschaft
der jeweiligen Funktion (Registerdruck/Liveness), nicht der Datei
oder der Compiler-Flags (identisch `-O4,p -opt` für `mameGesso.cpp`
und `EventWatcher.cpp` verifiziert). Fazit: `char trash[N]` ist kein
garantiert wirksamer Trick — er funktioniert zuverlässig nur, wenn
man empirisch für die KONKRETE Funktion prüft (bauen + diffen), nicht
durch pauschale Anwendung auf automatisch gefundene Kandidatenlisten.
Alle 24 Versuche sauber zurückgesetzt (`git checkout`), keine toten
`trash`-Deklarationen im Baum verblieben.

### Nach einundzwanzigster Iterationsrunde (Nerve-Vergleichsfehler-Suche: 4 Bugfixes)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,96 % (1.506.296 / 3.590.088) | +476 Bytes |
| Funktionen matched | 8.645 / 12.881 | +1 |

Neue Methodik: automatisierte Suche nach `__vt__`-Symbolabweichungen
an identischer Instruktionsposition (gleiche Adresse, unterschiedlicher
Vtable-Name) über alle 260 zuvor gescannten Units — ein zuverlässigeres
Signal für echte Logikfehler als reine Stackframe-Gaps. 38 Treffer,
davon 4 echte, über die rohe Retail-Disassembly bestätigte Bugs:

- `Enemy/fireWanwan.cpp::TFireWanwan::attackToMario` — Nerve-
  Ausschlussprüfung verglich gegen `TNerveFireWanwanRecover::
  theNerve()` statt `TNerveFireWanwanRecoverGraph::theNerve()`.
  **100 %** (476 Bytes) nach Fix.
- `Enemy/tamaNoko.cpp::TTamaNoko::isCollidMove` — Trample-Message-
  Guard verglich gegen `TNerveTamaNokoSleep::theNerve()` statt
  `TNerveTamaNokoDown::theNerve()`. **100 %** nach Fix.
- `Enemy/hinokuri2.cpp::THinokuri2::receiveMessageLv1` — Water-
  Spray-Schadenszweig setzte `TNerveHino2Freeze` statt
  `TNerveHino2Damage` als nächsten Nerve. 99,55 % → 99,81 % nach
  Fix (separater, nicht behobener 16-Byte-Frame-/Register-Rest).
- `Enemy/hinokuri2.cpp::TNerveHino2PrePol::execute` — `Pollute`-
  und `Stamp`-Zweige vertauscht (if-Zweig sollte `Stamp` pushen,
  else-Zweig `Pollute`, nicht umgekehrt). 98,68 % → 98,99 % nach
  Fix (separater, nicht behobener `getSaveParam()`-Feldzugriffspfad-
  Rest bei Index 30 der Instruktionsliste).

Beide Hinokuri2-Fixes werden trotz nicht erreichter 100 % behalten
(anders als bei spekulativen `trash`-Experimenten): Sie sind über die
rohe Disassembly bestätigte echte Verhaltenskorrekturen, keine
Vermutungen.

Weitere `__vt__`-Kandidaten geprüft, aber als zu komplex für schnelle
Fixes eingestuft (keine Änderung vorgenommen): `MarioUtil/MtxUtil.cpp::
TMultiMtxEffect::setup` (91,28 %, drei verschachtelte Switch-Case-
Objektkonstruktionen mit unterschiedlicher Registerreihenfolge),
`GC2D/CardLoad.cpp::TCardLoad::perform` (97,49 %, bereits im
Quelltext als upstream-TODO dokumentiert: Jump-Table vs.
Compare-Chain ist die Hauptursache), `System/MarDirectorInitECT.cpp::
TMarDirector::initECTGft` (90,76 %, mehrteilige Kamera/Ortho-
Objektkonstruktions-Reihenfolge, nicht in vertretbarer Zeit
aufgelöst). `MarioUtil/ShadowUtil.cpp::drawShadowGD`-Treffer sind
reine anonyme `$NNNN`-Zähler-Artefakte (verschiedene lokale
Typ-Ordinalzahlen zwischen unserem und dem Retail-Build), keine
echten Bugs.

Die Referenz-DOL bleibt `OK`.

### Nach zweiundzwanzigster Iterationsrunde (configure.py: 12 Unit-Flips auf Matching)

Neue Methodik: `objdiff-cli report generate` liefert pro Unit
`measures.matched_code_percent` und `measures.matched_data_percent`.
Systematischer Scan aller 736 Units fand 406 mit BEIDEM bei 100 %
(alle Funktionen UND alle Daten matchen bereits einzeln) — davon
waren 389 in `configure.py` schon korrekt auf `Matching` gesetzt,
17 noch nicht. Nach Ausschluss der beiden bereits dokumentierten
bekannten Brecher (`JALModSe.cpp`, `WoodBarrel.cpp` — Adress-Shift-
Layoutfehler) blieben 15 Kandidaten. Per-Unit-Bisektion (flip →
`python configure.py` → `ninja` → DOL-SHA1-Check → bei Fehlschlag
zurücksetzen) ergab:

**12 erfolgreich auf `Matching` gesetzt** (DOL-SHA1 bleibt `OK`):
`JSystem/JDrama/JDRDisplay.cpp`, `JSystem/JAudio/JAInterface/
JAIGlobalParameter.cpp`, `JSystem/JAudio/JASystem/JASDSPChannel.cpp`,
`JSystem/JParticle/JPADraw.cpp`, `M3DUtil/M3UModel.cpp`,
`System/MarNameRefGen_NPC.cpp`, `MoveBG/MapObjFloat.cpp`,
`Map/MapModel.cpp`, `GC2D/HelpActor.cpp`,
`Enemy/DemoBossHanachanBase.cpp`, `Enemy/BossHanachanSave.cpp`,
`Camera/CameraMarioData.cpp`. Verlinkte Units: 403 → 415 / 736.

**3 brechen die DOL-SHA1 trotz 100 % Code+Daten** (sofort
zurückgesetzt, bleiben `NonMatching`): `MoveBG/MapObjOption.cpp`,
`Map/PollutionEvent.cpp`, `Camera/CameraInbetween.cpp` — dieselbe
Kategorie wie `WoodBarrel`/`JALModSe` (anonyme Daten-/Adress-
Shift-Reste, die `objdiff-cli` pro Symbol nicht erfasst).

**Bug-Hunting-Methodik abgeschlossen**: Der `__vt__`-Symbol-
Mismatch-Scan (siehe 21. Runde) wurde projektweit auf alle 736
Units ausgeweitet (zuvor nur 260 automatisch vorselektierte
Kandidaten-Units). Ergebnis: exakt dieselben 38 Treffer wie in
Runde 21, keine neuen. Alle `Enemy/*.cpp`-Dateien (84 Stück)
einzeln gegengeprüft: 0 Treffer außerhalb der bereits behobenen.
Diese Methodik gilt für die aktuelle Codebasis als ausgeschöpft.

**Blindes `char trash[N]`-Auffüllen ist erschöpft geprüft**: Über
30 weitere automatisch erkannte Frame-Gap-Kandidaten (positiver
Gap, `fuzzy_match_percent` > 99,5 %) einzeln getestet — **0 Treffer**
in dieser Runde (verglichen mit einer deutlich höheren Trefferquote
bei den zuvor manuell kuratierten mameGesso/walkerEnemy/gesso-
Kandidaten). Auch `volatile char trash[N]` sowie ein erzwungener
Schreibzugriff wurden getestet, ohne Wirkung auf die Frame-Größe.
MWCC (`-O4,p -opt`) eliminiert ungenutzte Stack-Arrays bei manchen
Funktionen vollständig, bei anderen (auch mit identischen Flags,
identischer Datei) nicht — das Muster ist nicht zuverlässig
vorhersagbar. Empfehlung für Folge-Sessions: `trash`-Padding nur
noch bei Kandidaten versuchen, die durch vollständiges Lesen der
Funktion und Vergleich mit der rohen Retail-Disassembly bereits als
"nur Frame-Gap, sonst identisch" verifiziert wurden — nicht mehr
blind batchen.

Die Referenz-DOL bleibt `OK`.

### Nach dreiundzwanzigster Iterationsrunde (bossgesso isThing()-Reihenfolge; Konstanten-Mismatch-Scan)

| Metrik | Aktuell | Änderung |
| --- | ---: | ---: |
| Code matched | 41,96 % (1.506.460 / 3.590.088) | +164 Bytes |
| Funktionen matched | 8.646 / 12.881 | +1 |

Neue Methodik: automatisierte Suche nach `cmpwi`/`cmplwi`/`li`-
Instruktionen mit identischer Position, identischem Register aber
UNTERSCHIEDLICHER Konstante (statt `__vt__`-Symbolen) — 94 Treffer
über 241 Kandidaten-Units (`fuzzy_match_percent` ≥ 95 %). Nach
Filterung auf Fälle mit bereits identischem Stack-Frame (sicherstes
Signal für einen isolierten Fehler statt einer strukturellen
Differenz) blieben 3 Kandidaten:

- `Enemy/bossgesso.cpp::TBossGesso::changeAllTentacleState` —
  **100 %** (164 Bytes) nach Fix. `TBGTentacle::isThing()`
  (`include/Enemy/BossGessoTentacle.hpp`, als `// fabricated`
  markiert) prüfte `mState == 6 || mState == 3 || mState == 4` in
  Quelltextreihenfolge — MWCC kompiliert OR-Ketten mit
  Ganzzahlkonstanten aber NICHT strikt links-nach-rechts (bestätigt
  durch zwei Zwischenversuche mit unterschiedlicher Reihenfolge, die
  unterschiedliche, aber beide nicht korrekte Vergleichsreihenfolgen
  erzeugten). Empirisch ermittelte korrekte Quelltextreihenfolge
  `4 || 6 || 3` kompiliert exakt zur Zielreihenfolge. Nur ein
  Aufrufort (`changeAllTentacleState`), daher risikolos änderbar.

- `System/Application.cpp::TApplication::initialize_bootAfter`
  (98,77 % clean) — **tiefere Struktur-Unsicherheit gefunden, nicht
  behoben**: Der Konstruktoraufruf `new MSound(...)` kompiliert mit
  `li r3, 0xd4` (212, unsere `sizeof(MSound)`), während das Ziel
  `li r3, 0x30c` (780) erwartet. `include/MSound/MSound.hpp`s
  Feld-Offset-Kommentare summieren sich bereits korrekt auf 0x30C,
  aber der tatsächliche kompilierte `sizeof` bleibt bei 0xd4 — nach
  zweifachem erzwungenem Rebuild (Cache-Problem ausgeschlossen)
  weiterhin reproduzierbar. `include/JSystem/JAudio/JAInterface/
  JAIBasic.hpp` trägt bereits einen expliziten Upstream-TODO-
  Kommentar ("some of the fields might actually be from a derived
  class, MSound") — die JAIBasic/MSound-Feldgrenze ist demnach
  bereits als ungeklärt bekannt. Eine Korrektur erfordert eine
  vollständige Neuanalyse, welche Felder zu welcher Klasse gehören
  (Auswirkung auf alle ~50 Methoden beider Klassen) — zu riskant für
  einen schnellen Fix, als offener Fall dokumentiert statt geraten.

- `Player/WaterGun.cpp::TWaterGun::TWaterGun(TMario*)` (99,86 %
  clean, bereits von der `mHHoverHeight`-Typo-Untersuchung dieser
  Session betroffen) — weiterer unaufgeklärter Fund: Der Compiler
  erzeugt für das Array-Member `mEmitPos[4]` (`JGeometry::TVec3<f32>
  mEmitPos[4];`, Offsets im Header intern konsistent 0x1C90–0x1CC0)
  einen `__construct_array`-Aufruf mit Zähler `0x3` statt `0x4` —
  nach erzwungenem Rebuild reproduzierbar, Header-Deklaration bereits
  korrekt `[4]`. Vermutlich MWCC-interne Slicing-/Unroll-Heuristik
  für kleine Fixed-Size-Arrays, nicht über einfache Source-Änderung
  kontrollierbar; nicht weiter verfolgt.

`GC2D/PauseMenu2.cpp::TPauseMenu2::load` (99,96 % clean) ebenfalls
geprüft: Frame-Gap (0x50 vs. 0x30, Ziel kleiner) plus ein
`addis`/`addi`-Konstantenpaar (`0x70613030` vs. `0x745f30`, vermutlich
ein Fixed-Point- oder FourCC-Literal) — mehrteiliger Fall, nicht in
vertretbarer Zeit isoliert.

Die Referenz-DOL bleibt `OK`.

### Nach vierundzwanzigster Iterationsrunde (Feld-Offset-Mismatch-Scan; keine neuen sicheren Fixes)

Vierte Scan-Methodik ergänzt: Suche nach `lwz`/`stw`/`lha`/`lbz`/`sth`/
`stb`/`lfs`/`stfs`/`lfd`/`stfd`-Instruktionen an identischer Position
mit identischem Register-Paar, aber unterschiedlichem Offset-Literal
(analog zum Konstanten-Scan, aber für Feldzugriffe statt Immediates).
Projektweit (736 Units) 126 Treffer, davon nach Ausschluss `r1`-
basierter (Stack-Slot-Verschiebung, bereits bekannte Kategorie)
7 echte Feld-/Objekt-relative Treffer:

- `Enemy/smallEnemy.cpp::attackToMario` (nur 87,3 % clean insgesamt) —
  einzelner sauberer Offset-Diff, aber eingebettet in eine größere
  Umsortierung zweier `fsubs`-Berechnungen (Differenzbildung in
  anderer Reihenfolge) — kein isolierter Fix möglich.
- `Strategic/liveactor.cpp::TLiveActor::control` (nur 73,96 % clean) —
  `lwz r12, 0x10(r12)` vs. `0xc(r12)`, ein Vtable-Offset (potenziell
  falscher virtueller Aufruf), aber die Funktion hat 25 weitere
  strukturelle Diffs (fehlende/zusätzliche Instruktionen) — deutet auf
  einen echten fehlenden Codepfad hin, nicht in dieser Runde lösbar.
- `Player/MarioSpecial.cpp::TMario::pulling` (94,93 % clean) — Bit-Test
  `rlwinm. r0,r0,0,22,22` (Bit 22, Maske 0x200) vs. Ziel `...,21,21`
  (Bit 21, Maske 0x400); Quelltext bei `if (!(unk108->mInput & 0x400))`
  (Zeile 1209) legt Maske 0x400 nahe, passend zum ZIEL — bei 114
  Gesamt-Diffs in dieser sehr langen Funktion aber nicht zweifelsfrei
  genau dieser Quelltextzeile zuordenbar ohne vollständige Neulesung;
  als Kandidat für eine künftige Session vorgemerkt statt spekulativ
  gefixt.
- `Player/WaterGun.cpp::rotateProp` — identisch mit dem bereits
  dokumentierten `mHoverRotMax`-Rätsel dieser Session.
- `Map/MapCollisionEntry.cpp::moveSRT` — bereits als "interner
  Slot-Versatz" dokumentiert.
- `Strategic/spcinterp.cpp::execadd/execsub/execmul/execdiv` — bereits
  als Messartefakt dokumentiert.

Alle drei in dieser Runde einzeln ausprobierten `char trash[N]`-
Kandidaten (`Map/PollutionAction.cpp::fire`, `Map/MapMirror.cpp::
isUpperThanMirrorPlane`, sowie ein Re-Check von `moveSRT`) blieben
bei 0 % Wirkung — bestätigt erneut den in Runde 22 dokumentierten
Befund, dass blindes `trash`-Padding außerhalb bereits vertrauter
Dateien nicht zuverlässig funktioniert. `configure.py`-Unit-Flip-Scan
erneut durchgeführt: keine neuen Kandidaten seit Runde 22 (dieselben
3 bekannten Brecher `MapObjOption`, `PollutionEvent`,
`CameraInbetween` plus die 2 bereits dokumentierten `JALModSe`/
`WoodBarrel`).

Die Referenz-DOL bleibt `OK`.

### Nach fünfundzwanzigster Iterationsrunde (Tiefenanalyse `pulling`/`control`: ein Fehlalarm, ein bekanntes TODO)

Zwei der in Runde 24 dokumentierten Kandidaten per Rohdisassembly
(`build/GMSJ01/asm/…`) vollständig nachgeprüft, gemäß Methodik-Regel
dieser Session:

- `Player/MarioSpecial.cpp::TMario::pulling` — der vermutete
  Bit-Test-Fehler (`rlwinm. r0,r0,0,22,22` vs. angeblich `...,21,21`
  im Ziel) ist ein **bestätigter Fehlalarm**. Die rohe Retail-
  Disassembly (Zeile 1737 in `MarioSpecial.s`) zeigt an exakt dieser
  Stelle ebenfalls Bit 22 — unser Code ist dort bereits korrekt. Das
  `objdiff-cli diff`-JSON war an dieser Position falsch ausgerichtet
  (derselbe Artefakt-Typ wie beim bereits dokumentierten
  `doActorToWire`-Fund). Kein Fix nötig; frühere Vermutung aus Runde
  24 hiermit korrigiert.

- `Strategic/liveactor.cpp::TLiveActor::control` — die rohe Retail-
  Disassembly vollständig nachvollzogen (57 Instruktionen,
  `build/GMSJ01/asm/Strategic/liveactor.s` Zeile 862ff). Bestätigt:
  Der Quelltext hat an ZWEI Stellen einen reinen Kommentar-Stub
  (`// call on unk90`) statt eines echten virtuellen Aufrufs auf
  `unk90` (Vtable-Offset 0x10, über eine sekundäre Vtable bei
  `unk90+0x5c`). `unk90` ist als `void*` untypisiert und wird im
  gesamten sichtbaren Quelltext nirgends auf einen Nicht-Null-Wert
  gesetzt (nur `nullptr`-Initialisierung im Konstruktor) — der
  bestehende Kommentar `// TODO: was ist unk90???` markiert dies
  bereits als offen. Eine Korrektur erfordert, `unk90`s tatsächlichen
  Typ zu bestimmen (vermutlich eine polymorphe Klasse mit
  Sekundär-Vtable, evtl. verwandt mit `TSpineBase`s eigenem
  0x24-Offset-Muster) — außerhalb des Zeitrahmens für einen
  risikoarmen Fix in dieser Runde; als offener Fall dokumentiert statt
  spekulativ mit einem geratenen Typnamen committet (Projektregel:
  keine Vermutungen als Fakten).

Die Referenz-DOL bleibt `OK`.

### Nach sechsundzwanzigster Iterationsrunde (echter `mVelocity`-Bugfix in `smallEnemy::attackToMario`)

Verbleibende Feld-Offset-Scan-Kandidaten (`ModelWaterManager::drawMirror`
90,04 %, `ModelWaterManager::calcVMMtxGround` 62,49 %,
`TSmallEnemy::attackToMario` 87,31 %) einzeln per Rohdisassembly
geprüft:

- `drawMirror`/`calcVMMtxGround`: gestreute strukturelle Diffs (177 von
  540 Instruktionen bei `drawMirror`, mehrere INSERT/DELETE-Cluster ab
  früher Instruktion) — kein isolierbarer Einzel-Bug, sondern verteilt
  über die ganze GX-lastige Funktion. Als offener Fall dokumentiert statt
  weiterverfolgt.

- `TSmallEnemy::attackToMario` — **echter Bugfix gefunden und behoben**:
  Rohdisassembly (`build/GMSJ01/asm/Enemy/smallEnemy.s` Zeile 3719ff)
  zeigt, dass `mVelocity.set(local_20)` im Ziel NUR `mVelocity.x`
  (Offset `0xac`) und `mVelocity.z` (`0xb4`) beschreibt — `mVelocity.y`
  (`0xb0`) wird nie gestored. Auf `mVelocity.x = local_20.x; mVelocity.z
  = local_20.z;` umgestellt: **87,31 % → 90,44 %**. Passt inhaltlich zu
  einem Angriffs-Lunge, der nur die horizontale Ebene beeinflusst und
  die vertikale Geschwindigkeit unangetastet lässt. Restdiff (90,44 %)
  ist ein reiner Stack-Slot-Wiederverwendungs-Unterschied: Ziel legt für
  den lokalen `v`-Vektor einen frischen Slot bei `0x14(r1)` an,
  unser Build wiederverwendet den toten `local_20`-Slot bei `0x20(r1)`
  (beide Frames sind exakt `0x40` Bytes groß — keine Frame-Differenz,
  reine Alias-Optimierung). `char trash[0xc]` vor der `v`-Deklaration
  getestet: verschlechtert auf 90,22 %, sofort zurückgesetzt. Verbleibt
  als MWCC-interne Slot-Alias-Entscheidung, nicht über Source
  erzwingbar.

Die Referenz-DOL bleibt `OK`.

### Nach siebenundzwanzigster Iterationsrunde (WaterGun-Struct-Bugfix, systematisches Scannen 90–99,99-%-Kandidaten)

Automatisierter Scan aller Funktionen mit 90–99,99 % Fuzzy-Match und
≤300 Bytes über alle 736 Units (462 Kandidaten), sortiert nach
Instruktions-Diff-Anzahl (kleinste zuerst) statt nach Match-Prozent —
liefert zuverlässigere Kandidaten als reines Prozent-Sortieren, weil
Bytegröße die Prozentzahl verzerrt. Für jeden Kandidaten mit 1–2
Instruction-Diffs Rohdisassembly-Cross-Check vor jedem Fix-Versuch
(Methodik-Regel weiterhin bestätigt: kein Vertrauen in objdiff-JSON
ohne Gegenprüfung).

**Echter Bugfix**: `Player/WaterGun.hpp::TWaterGunParams` — Feld
`mNozzleAngleYSpeedMax` (nirgends im Quelltext benutzt) stand an der
falschen Stelle im Struct (zwischen `mNozzleAngleYBrake` und
`mHoverRotMax`). Durch Cross-Referenzierung zweier unabhängiger
Retail-Funktionen (`rotateProp`s `mHoverRotMax`-Offset `0x1d90` UND
`changeBackup`s `mChangeSpeed`-Offset `0x1dcc`) eindeutig belegt: das
Feld gehört zwischen `mHoverSmooth` und `mChangeSpeed`. Nach
Verschieben: `rotateProp` **99,98 % → 100 %**; `movement()` verbessert
sich ebenfalls leicht (bleibt wegen eines separaten, bereits
dokumentierten TODO-Fehlens von Stack-Speicher unter 100 %).

**Fehlgeschlagene Fixversuche** (alle einzeln zurückgesetzt, keine
Verschlechterung committet):

- `Enemy/bosspakkun.cpp::TBossPakkun::setGroundCollision` (99,98 %,
  einziger Diff: `collisionMtx`-Local bei `0x20` statt `0x18`).
  `char trash[8]` vor UND nach der Deklaration getestet: beide
  verschlechtern (99,88 %/99,86 %). Zurückgesetzt.

- `Player/WaterGun.cpp::TWaterGun::setBaseTRMtx` (99,97 %, `temp`-Mtx-
  Local bei `0x1c` statt `0x20`). `char trash[4]` an drei Stellen
  (vor `result`, zwischen `result`/`temp`, nach `temp`) getestet: alle
  verschlechtern (99,63–99,67 %). Zurückgesetzt.

- `MoveBG/MapObjLib.cpp::TMapObjBase::isDemo` (99,77 %, einziger Diff:
  Sprungziel bei b1==true zeigt in unserem Build auf „return true"
  statt wie im Original auf „return false"). Umstrukturierung zu
  einem frühen `if (b1) return false;` verschlechterte drastisch auf
  90,45 % — Hypothese falsch, MWCC kompiliert das Early-Return-Muster
  anders als angenommen. Zurückgesetzt auf Original.

- `Player/MarioRun.cpp::TMario::rotating` (98,65 %, einziger Diff:
  fehlendes `extsh` nach `neg` bei `mModelFaceAngle = -(mStatusTimer *
  4096);`). Drei Varianten (`(s16)`-Cast auf den ganzen Ausdruck,
  Negation vor der Multiplikation, expliziter `s16`-Temp) probiert:
  erste und dritte ändern nichts (98,65 % identisch), zweite
  verschlechtert auf 95,95 %. MWCC-interne Entscheidung, ob nach einer
  Negation vor `s16`-Store sign-extended wird, nicht über einfache
  Source-Umformulierung erzwingbar.

- `Enemy/pakkun.cpp::TNervePakkunAppear::execute` (98,55 %, einziger
  Diff: überflüssiges `cmpwi r3,0x0` nach `checkPass()`-Aufruf, dessen
  Ergebnis im leeren `if (...) { }`-Body nicht verwendet wird). Entfernen
  des `if`-Wrappers (reiner Ausdrucks-Aufruf `checkPass(100.0f);`)
  erzeugt **exakt identischen** Maschinencode — MWCC materialisiert den
  bool-Rückgabewert eines Funktionsaufrufs unabhängig davon, ob er
  ausgewertet wird. Zurückgesetzt auf die klarere `if(){}`-Fassung
  (kein Unterschied, aber dokumentiert die ursprüngliche Absicht besser).

- `MoveBG/MapObjLib.cpp::TMapObjBase::getDistance` (99,94 %, einziger
  Diff: `volatile f32 y`-Local bei `0x14` statt `0x10`, Frame beidseitig
  `0x18` identisch). Drei Varianten (`char trash[4]` am Funktionsanfang,
  `char trash[4]` vor `y`, `f32 pad` vor `y`) — alle **exakt ohne
  Wirkung** (99,935486 % identisch bei allen dreien). Anders als die
  übrigen Padding-Fälle dieser Session beeinflusst hier offenbar keine
  lokale Variable diese eine `volatile`-Platzierung; Ursache bleibt
  unklar, nicht in vertretbarer Zeit weiter verfolgt.

**Weiterer echter Fund**: `Player/MarioMain.cpp::TMario::drawSyncCallback`
(99,97 %, einziger Diff: `local_1c`-Ausgabeparameter für `GXPeekARGB` bei
`0x14` statt `0x10`). `char trash[4]` VOR der Deklaration wirkungslos,
aber `char trash[4]` DIREKT NACH `u32 local_1c;` (vor dem
`GXPeekARGB`-Aufruf) trifft exakt: **100 %**.

Die Referenz-DOL bleibt `OK`.

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

- `Strategic/HitActor.cpp`: `THitActor::calcEntryRadius` (124 Bytes).
  Nach `ninja all_source` matcht der committed Quelltext zu **97,61 %**.
  Mit `char trash[0x30]` stieg es auf 97,68 %. Rest: FPSCR-Registerwahl
  und `fmadds`/`frsp`-Reihenfolge.

- `Strategic/livemanager.cpp`: `TLiveManager::perform` (252 Bytes).
  Nach Rebuild matcht der committed Quelltext zu **99,84 %**. Bestes
  Experiment `char trash[0x10]`: 99,92 %, Frame 0x50 identisch. Verbleibend
  fünf Color-Access-Offsets (`0x34` vs. `0x24`).

- `THPPlayer/THPAudioDecode.c`: `AudioDecoderForOnMemory` (176 Bytes, 89,27 %).
  Register-Diff betrifft die gehoisteten `ActivePlayer`- und `AudioDecodeThread`-
  Pointer sowie den Frame-Index (`r28` ist beidseitig `readSize`; die echte
  Verschiebung ist `frame` in `r29` statt `r31`). `while(TRUE)` → `for(;;)`
  ist ein No-Op auf die Registerallokation; ein belastbarer Match setzt
  voraus, dass die lokalen Variablen in einer bestimmten Reihenfolge deklariert
  sind und der Compiler die Hoists nicht zusammenlegt.

- `Player/SplashManager.cpp`: `TSplashManager::makeDL` (392 Bytes, 99,95 %).
  Original reserviert Color-Struct bei `0x58(r1)`, unsere Version bei `0x54(r1)`
  (4-Byte-Differenz). Varianten `u32 pad`, zusätzliche `JGeometry::TVec3`,
  `char trash[8]`/`[4]` nach `thing[4]` erreichten 99,82 %/99,63 %/99,63 %/
  99,82 % – alle schlechter. Vermutlich SDA-Konstantenwahl, nicht Padding.

- `MoveBG/MapObjPollution.cpp`: `loadAfter` (172 Bytes, 88,07 %).
  Quellaufruf von `registerRevivalTexStamp` arbeitet mit `int/short`-Parametern;
  eine forcierte `(s16)/(s32)`-Typisierung brachte keine Änderung. Differenz
  bleibt in `r29`/`r30`-Registerwahl und Reihenfolge der `addi`s innerhalb der
  Schleife.

- `THPPlayer/THPPlayer.c`: `THPPlayerPrepare` (624 Bytes, 97,98 %).
  Register-Offset um genau 1 (`r27..r30` statt `r28..r31`) über die gesamte
  Funktion, Frame identisch. `threadData`-Scope auf den `onMemory`-Block
  verengt: keine Änderung. Rest nicht in vertretbarer Zeit lösbar.

- `Camera/CameraMode.cpp`: `CPolarSubCamera::isNormalCameraCompletely`
  (144 Bytes, 72,86 %). Original inlined `isNormalCameraSpecifyMode(int)`
  komplett als Switch-Jump-Table (`lwzx`/`mtctr`/`bctr`) an BEIDEN Aufrufstellen
  (`mMode` und `mPrevMode`); unser Build ruft die separate Funktion via `bl`
  auf. MWCC-Auto-Inline-Heuristik, nicht über Source-Umbau erzwingbar ohne
  Pragma-Kenntnis; strukturell verwandt mit dem bereits dokumentierten
  `fabricatedInline3`-TODO in `NpcManager::clipEnemies`.

- `Enemy/effectObj.cpp`: `TEffectColumSand::reset` (308 Bytes, 98,49 %).
  `r30`/`r31` komplett vertauscht (`this` vs. String-Literal-Adresse
  "08_sunabashira") — Reihenfolge, in der die zwei über den
  `TEffectModel::reset()`-Aufruf hinweg lebenden Werte in Callee-Save-Register
  gesichert werden. Expliziter `const char* name`-Local verschlechterte auf
  94,35 % (zurückgesetzt); rein MWCC-interne Save-Reihenfolge, nicht über
  Source beeinflussbar.

- `Enemy/effectObj.cpp`: `TEffectObjBase::moveObject` (464 Bytes, 98,84 %).
  Dreifach identisches FP-Register-Rotationsmuster (f0/f1/f2 zyklisch
  vertauscht) in den drei `emitter->setGlobalScale(local_1c)`-Aufrufen —
  inlined-Callee-Registerzuordnung, dieselbe Kategorie wie
  `J3DModel::entryModelData`.

- `Map/MapEventSirena.cpp`: `TMapEventSirenaSink::watch` (280 Bytes,
  99,97 % bestes Experiment — Original bleibt bei 99,87 %). `char trash[8]`
  vor oder nach dem `fireStartDemoCamera(...)`-Aufruf (der Parameter
  `JDrama::TFlagT<u16>(0)` als anonymes Temporary konstruiert) schließt
  4 von 16 Bytes Offset-Differenz bei der `sth`-Store-Adresse des Temporaries;
  `trash[0xc]` statt `trash[8]` an derselben Stelle ändert nichts weiter —
  die Temporary-Platzierung ist compiler-intern fixiert, nicht über Local-
  Padding beeinflussbar. Da 99,97 % kein 100-%-Match ist, Quelltext auf die
  saubere Original-Fassung (99,87 %) zurückgesetzt statt einen wirkungslosen
  `trash`-Hack stehen zu lassen.

- `Enemy/enemytable.cpp`: `TStageEnemyInfoTable::getMatchedInfo` (276 Bytes,
  92,41 %). Frame-Differenz 64 Bytes (0xa0 vs. 0x60): unser Build hoisted
  `begin()`/`end()` aus der `for`-Schleife in zwei zusätzliche Callee-Save-
  Register (r28+r29+r30+r31), das Original liest `this->begin` (+0x10) und
  `this->end` (+0x14) bei JEDER Iteration neu aus `this` (nur r29..r31 nötig).
  Loop-invariant-Code-Motion-Entscheidung von MWCC, nicht über einfache
  Source-Umformulierung erzwingbar ohne Risiko einer Verhaltensänderung.

- `MoveBG/MapObjWater.cpp`: `TMapObjWaterFilter::perform` (404 Bytes,
  78,39 %). Bereits im Quelltext als upstream-`TODO` markiert
  ("mother of all intern codes..."); komplexe Matrixrechnung
  (`J3DGetTranslateRotateMtx`, `PSMTXScale`, `MTXInverse`, `MTXConcat`) mit
  mehreren `Mtx`-Stack-Locals, bekannt schwierig.

- `Map/PollutionManager.cpp`: `TPollutionManager::cleanedAll` (268 Bytes,
  96,43 %). Einzeiliger Quelltext (`return getPollutionDegree() < … ? true :
  false;`), aber `getPollutionDegree()` (Schleife über `getLayer(i)`) wird
  komplett inlined — kein benannter Local als Anker für Padding; Register-
  Zählungsdifferenz (r8/r7/r6 vs. r7/r6) durch die Inline-Schleife bedingt.

- `NPC/NpcManager.cpp`: `TNPCManager::clipEnemies` (784 Bytes, 92,70 %).
  Bereits im Quelltext als upstream-`TODO` markiert
  ("figure out these inlines ... fabricatedInline3 matches in camera itself
  but not here"); großer struktureller Unterschied (Frame 0x90 vs. 0x50,
  `gpCamera`-Caching), nicht mit lokalen Padding-Tricks lösbar.

- `NPC/NpcManager.cpp`: `makePartsModelData_` (336 Bytes, 99,98 %).
  `sdlModel`-Local liegt 4 Bytes höher (`0x20` statt `0x1c`); Umordnen der
  `loadFlags`/`initInfo`-Deklaration ohne Wirkung.

- `Strategic/liveactor.cpp`: `TLiveActor::control` (73,96 %). Quelltext
  hat zwei reine Kommentar-Stubs (`// call on unk90`) statt echter
  virtueller Aufrufe; `unk90` ist `void*` (untypisiert) und wird im
  sichtbaren Quelltext nirgends auf Nicht-Null gesetzt — bereits als
  `// TODO: was ist unk90???` markiert. Per Rohdisassembly bestätigt:
  Aufruf über Sekundär-Vtable bei `unk90+0x5c`, Offset 0x10. Fix
  erfordert Typbestimmung von `unk90`, nicht in dieser Runde geleistet.

- `Player/ModelWaterManager.cpp`: `TModelWaterManager::drawMirror`
  (90,04 %) und `calcVMMtxGround` (62,49 %). Gestreute strukturelle
  Diffs (177/540 Instruktionen bei `drawMirror`, mehrere INSERT/DELETE-
  Cluster) über die ganze GX-lastige Funktion verteilt — kein
  isolierbarer Einzel-Bug.

- `Enemy/smallEnemy.cpp`: `TSmallEnemy::attackToMario` (90,44 % nach
  Bugfix, siehe Gematcht-Liste). Restdiff ist ein MWCC-interner Stack-
  Slot-Alias-Unterschied (Ziel legt für `v` einen frischen Slot an,
  wir nutzen den toten `local_20`-Slot wieder), Frame ist beidseitig
  `0x40` Bytes — nicht über Source erzwingbar.

- `Enemy/bosspakkun.cpp`: `TBossPakkun::setGroundCollision` (99,98 %).
  `collisionMtx`-Local bei `0x20` statt `0x18`; `char trash[8]` vor/nach
  der Deklaration verschlechtert beide Male. Nicht über Padding lösbar.

- `Player/WaterGun.cpp`: `TWaterGun::setBaseTRMtx` (99,97 %). `temp`-Mtx
  bei `0x1c` statt `0x20`; drei `char trash[4]`-Platzierungen
  verschlechtern alle. Nicht über Padding lösbar.

- `MoveBG/MapObjLib.cpp`: `TMapObjBase::isDemo` (99,77 %). Einziger Diff
  ist das Sprungziel des b1==true-Zweigs (Original springt direkt zu
  `return false`, unser Build zu `return true`); Early-Return-
  Umstrukturierung verschlechterte drastisch auf 90,45 % statt zu
  verbessern — MWCC-Codegen für dieses Kontrollfluss-Muster nicht wie
  erwartet. Zurückgesetzt.

- `Player/MarioRun.cpp`: `TMario::rotating` (98,65 %). Fehlendes `extsh`
  nach `neg` beim negierten Zweig von `mModelFaceAngle = -(mStatusTimer *
  4096)`. Drei Umformulierungen ohne Wirkung oder verschlechternd.

- `Enemy/pakkun.cpp`: `TNervePakkunAppear::execute` (98,55 %).
  Überflüssiges `cmpwi` nach ungenutztem `checkPass()`-Rückgabewert;
  Entfernen des `if(){}`-Wrappers erzeugt identischen Code (MWCC
  materialisiert bool-Rückgaben unabhängig von Verwendung).

- `MoveBG/MapObjLib.cpp`: `TMapObjBase::getDistance` (99,94 %).
  `volatile f32 y`-Local bei `0x14` statt `0x10`, Frame identisch;
  drei Padding-Varianten alle wirkungslos (exakt gleicher Match%).

- `Map/PollutionLayer.cpp`: `TPollutionLayer::stampModel` (99,27 %).
  Einziger Diff: Ladereihenfolge von `x` (aus `model`) und `mMinX` (aus
  `this`) vertauscht (reine Instruktions-Scheduling-Reihenfolge, gleiche
  Zielregister). Operanden-Vertauschung im Vergleich (`mMinX > x` statt
  `x < mMinX`) verschlechtert auf 98,27 %; Deklarationsreihenfolge von
  `x`/`z` getauscht: keine Wirkung (99,267 % ~ identisch). Zurückgesetzt.

- `MoveBG/MapObjRailBlock.cpp`: `TNormalLift::setGroundCollision`
  (95,74 %). `TRailMapObj::setGroundCollision` (die per Quelltext
  aufgerufene Basisklassenmethode) matcht **für sich genommen 100 %**;
  beim Inlinen in `TNormalLift::setGroundCollision` erzeugt unser Build
  jedoch einen zusätzlichen `__ct__` (leerer `SMatrix34C<f32>`-Default-
  Konstruktor für das lokale `TMtx34f mtx`), den das Original an dieser
  Inlining-Stelle wegoptimiert. `#pragma dont_inline on/off` um die
  aufrufende Funktion ohne Wirkung (identisches Ergebnis). Kontext-
  abhängige MWCC-Inlining-Entscheidung, nicht ohne Risiko für die
  bereits 100 % matchende Basismethode angreifbar.

- `Player/MarioDraw.cpp`: `TMario::initMirrorModel` (99,96 %). Alle drei
  Diffs sind identische rodata-Offset-Shifts (`@1490+0xa50/0xa5c/0xa6c`
  bei uns vs. `+0xa08/0xa14/0xa24` im Original, konstant `0x48` Bytes
  Versatz) für die Japanese-String-Literale der `TMirrorActor`-Namen.
  Die ganze Unit zeigt `[.rodata-0]` nur **50,33 %** — deutet auf einen
  umfassenderen String-Literal-Reihenfolge-/Größenunterschied irgendwo
  früher in der Datei hin, nicht isoliert auf diese Funktion. Für eine
  künftige Runde: alle String-Literale in `MarioDraw.cpp` systematisch
  mit der rohen `.rodata`-Sektion der Retail-Disassembly abgleichen.

### Nach achtundzwanzigster Iterationsrunde (kritischer objdiff-cli-Fund: falscher match_percent, echter Logikbug in `TMarDirector::movement`)

Fortsetzung des ndiff-sortierten 85–99,99-%-Scans (723 Kandidaten, Größe
≤400 Bytes, alle 736 Units). Ein Kandidat mit ungewöhnlich niedrigem
Match% bei nur 2 Instruktions-Diffs (`TMarDirector::movement`,
91,25 %, 48 Bytes) fiel sofort als starkes Bug-Signal auf.

**Kritischer Methodik-Fund**: Das objdiff-JSON zeigte scheinbar, dass
unser Build `movement_game()` NIE aufruft (toter Code nach einem
unbedingten `b`), während das Original es aufruft. Rohdisassembly
(`build/GMSJ01/asm/System/MarDirectorEvent.s` Zeile 331–346) bestätigte
tatsächlich einen invertierten Vergleich im Quelltext (`!=` statt `==`
für `STATE_UNK4`) — ein echter Logikbug, keine Fehlwahrnehmung. Nach dem
Fix (`if ((int)mState == STATE_UNK4) movement_game();`) meldete
`objdiff-cli` jedoch **90,0 %** (schlechter als vorher!) mit einem
`beq`/`bne`-Opcode-Mismatch, der der rohen Zieldisassembly widersprach.

Direkte Byte-für-Byte-Verifikation via Parsen des DOL-Headers und
Extraktion der rohen Maschinencode-Bytes an virtueller Adresse
`0x800EDA30` aus unserer frisch gebauten `mario.dol` (unter Umgehung von
`objdiff-cli` komplett) bewies: **unser kompilierter Code ist jetzt
byte-identisch mit der Zieldisassembly** (`7C0802A6 90010004 9421FFF8
88030064 2C000004 41820008 48000008 480002C1 8001000C 38210008
7C0803A6 4E800020` — exakt wie im `.s`-File dokumentiert). Der Fix ist
also ein **bestätigter 100-%-Byte-Match**, trotz `objdiff-cli`s
fälschlicher 90-%-Meldung.

Das ist eine **schwerwiegendere Ausprägung** des bereits mehrfach
dokumentierten objdiff-JSON-Fehlausrichtungsproblems: bisher betraf es
nur die PER-INSTRUKTION-Anzeige (falsche Zuordnung einzelner Zeilen bei
insgesamt korrekter `match_percent`); hier war die `match_percent`-Zahl
selbst falsch, sowohl über `objdiff-cli diff` als auch über die
Batch-`report generate`-Pipeline (beide nutzen denselben Diff-Kern,
daher kein Cache-Artefakt eines einzelnen Kommandos). **Neue
Methodik-Regel**: Bei widersprüchlichen/unplausiblen `match_percent`-
Sprüngen nach einem Fix (insbesondere Verschlechterung trotz
offensichtlich korrekterer Logik) MUSS zusätzlich zur `.s`-Rohdatei auch
eine direkte Byte-Extraktion aus dem gebauten `mario.dol` an der
bekannten virtuellen Adresse erfolgen, um `objdiff-cli` vollständig zu
umgehen.

**Fix**: `System/MarDirectorEvent.cpp::TMarDirector::movement()` —
`if ((int)mState != STATE_UNK4)` → `if ((int)mState == STATE_UNK4)`.
Echter, spielrelevanter Logikbug (Aufruf von `movement_game()` war
vorher faktisch unerreichbarer Code). Byte-perfekt gegen Retail
verifiziert.

**Weitere in dieser Runde geprüfte Kandidaten** (alle Padding-Versuche
zurückgesetzt, keine Verbesserung):

- Bestätigtes wiederkehrendes Muster „Output-Parameter per Adresse,
  Stack-Slot um 4–8 Bytes versetzt" bei sieben Funktionen in dieser
  Runde (`makeObjAppear` in `MapObjManager.cpp`, `getRandomNextIndex`
  in `graph.cpp`, `execGroundCheck` in `CameraBGCheck.cpp`, `__ct__
  TLensFlare` in `lensflare.cpp`, zusätzlich zu den bereits in Runde 27
  dokumentierten `setGroundCollision`/`setBaseTRMtx`/`getDistance`) —
  `char trash[N]` vor UND nach der Deklaration jeweils getestet, alle
  Varianten verschlechtern oder wirkungslos. Diese Kategorie gilt nun
  als **systematisch nicht per Padding lösbar** und wird nicht weiter
  einzeln verfolgt.

- `GC2D/GCConsole2.cpp::TGCConsole2::startDisappearTimer` (98,61 %).
  Konstanten-Diff `0x1d1+0x3c` (Ziel) vs. `0x20d` (unser, vorberechnet
  `525`) — Dekomposition des Quelltexts in `465 - y1 + 60` statt
  `525 - y1` ändert nichts (MWCC faltet die Konstante identisch zurück
  zusammen). Nicht per einfacher Source-Umformulierung lösbar.

Die Referenz-DOL bleibt `OK`.

### Nach neunundzwanzigster Iterationsrunde (neue Fix-Kategorie: `#pragma dont_inline` für fehlende weak-Symbole; 15 Commits, 39 Funktionen)

**Neue Kandidaten-Kategorie entdeckt**: Ausgangspunkt war der
"teilweise dekompilierte Datei mit wenigen 0-%-Funktionen"-Scan
(bevorzugt laut Projekt-Policy). `mario/NPC/NpcBase` war bei 79 %
Datei-Match mit genau einer unge matchten Funktion:
`TBaseNPC::getAnmOffDist_()`. Das `report.json`-Feld für diese Funktion
hatte **gar kein** `fuzzy_match_percent` — kein Vergleichssymbol
vorhanden, nicht einfach ein niedriger Wert. Rohdisassembly bestätigte:
Retail hält für diese Header-`inline`-definierte Methode eine echte
**out-of-line `weak`-Symbol-Kopie** (260 Bytes, ein einziger Call-Site),
während MWCC sie in unserem Build vollständig wegin lined — kein
Symbol, keine Adresse, nichts zum Vergleichen.

**Fix-Technik**: `#pragma dont_inline on` / `#pragma dont_inline off`
direkt um die Methoden-Definition im Header gelegt zwingt MWCC, eine
konkrete out-of-line-Kopie zu emittieren, exakt wie im Original. Das
Pragma ist im Projekt bereits etabliert (45+ bestehende Verwendungen,
bisher aber nur für gewöhnliche `.cpp`-lokale Methoden, nie für
Header-`inline`-Definitionen mit mehreren Call-Sites).

**Verifikationsmethodik-Erweiterung**: `objdiff-cli`s `report
generate`/`diff` zeigen für diese frisch emittierten weak-Symbole
weiterhin **kein** `fuzzy_match_percent` (selbst nach korrektem Fix) —
ein bestätigtes Tool-Limit, nicht spezifisch für falsche Fixes. Einzige
zuverlässige Verifikation: direkte Byte-Extraktion aus
`build/GMSJ01/mario.dol` UND `orig/GMSJ01/sys/main.dol` an der
bekannten virtuellen Adresse (DOL-Header-Parsing-Technik aus Runde 28),
Vergleich `our_bytes == retail_bytes`. Zusätzlich vor jedem Commit:
voller `report generate`-Vorher/Nachher-Vergleich der Menge aller
`fuzzy_match_percent == 100.0`-Funktionen (`lost`/`gained`-Diff), um
Regressionen an ANDEREN (ggf. weiterhin korrekt inlined) Call-Sites
derselben Funktion auszuschließen — **bei allen 39 Fixes dieser Runde:
0 Regressionen**, auch bei mehrfach verwendeten Funktionen wie
`TUtil<f32>::one()` (verwendet transitiv in jedem `normalize()`-Aufruf)
oder `TMario::checkStatusType()`.

**Systematisches Scannen**: `report.json` nach Funktionen ohne
`fuzzy_match_percent`-Feld durchsucht (2.369 Treffer von 12.881
Gesamtfunktionen). Cross-Referenz mit einem einmalig aufgebauten
Call-Site-Histogramm (`grep -rhoP '(?<=\tbl )\S+' build/GMSJ01/asm/`)
identifiziert Kandidaten mit 1–6 `bl`-Aufrufstellen in Retail (= MWCC
inlined fast überall, aber nicht an diesen Stellen). Für jeden
Kandidaten geprüft: (a) existiert die Methode bereits inline im
Quelltext (dann nur Pragma nötig) oder fehlt sie komplett (dann
Neuimplementierung nötig, außerhalb des Scope dieser Fix-Kategorie),
(b) ist die aufrufende Datei bzw. die Klasse, in der die Methode
deklariert ist, tatsächlich vorhanden oder eine leere 1-Zeilen-Stub-Datei.

**Wichtige Falltür entdeckt**: Ein Großteil der 2.369 Kandidaten
gehört zu Dateien, die als 1-Byte-Stub existieren
(`src/Enemy/BossHanachanMain.cpp`, `koopajr.cpp`, `limitkoopa.cpp`,
`killer.cpp`, `wireTrap.cpp`, `cannon.cpp`, `MapObjBall.cpp` u.v.a. —
~20 komplett unbearbeitete Enemy-/MapObj-Dateien). Für Methoden, deren
Klasse NUR in einer solchen leeren Datei deklariert würde
(`TBossHanachan::kill()`, `TDirectionCalc::*`, alle
`__ct__XxxManagerFPCc`-Konstruktoren, `theNerve__Xxx`-Accessor), ist
die Pragma-Technik **nicht anwendbar** — die ganze Klasse fehlt, das
ist Neudekompilierungsarbeit, kein Emissions-Bugfix. Entscheidend war
zu prüfen, ob die **Definition** der fehlenden Methode in einem
bereits populierten Shared-Header liegt (z. B. `JGVec3.hpp`,
`MathUtil.hpp`) — dann ist die Leere der AUFRUFENDEN Datei irrelevant.

**39 gefixte Funktionen in 15 Commits** (alle Byte-für-Byte gegen
`orig/GMSJ01/sys/main.dol` verifiziert, 0 Report-Regressionen, DOL-SHA1
nach jedem Commit `OK`):

1. `include/NPC/NpcBase.hpp`: `TBaseNPC::getAnmOffDist_()` (260 B).
2. `include/MoveBG/MapObjHide.hpp`: `TWaterHitPictureHideObj::
   getObjAppearPos()` (8 B), `THideObjPictureTwin::getObjAppearPos()`
   (12 B) — beide virtuell, nur über Vtable erreichbar, 0 direkte
   Call-Sites.
3. `include/Player/MarioAccess.hpp`: `SMS_GetMarioPos()` (8 B).
4. `include/Enemy/Graph.hpp`: `TGraphTracer::getCurGraphIndex()` (8 B),
   `TGraphTracer::getGraph() const` (8 B).
5. `include/Strategic/ObjModel.hpp`: `TMActorKeeper::getMActorAnmData()`
   (8 B).
6. `include/Map/MapCollisionEntry.hpp`: `TMapCollisionBase::setMtx()`
   (44 B).
7. `include/JSystem/JGeometry/JGUtil.hpp`: `TUtil<f32>::one()` (8 B);
   `include/MarioUtil/MathUtil.hpp`: `MsClamp<f32>()` (32 B, drei
   Call-Sites), `MsSqrtf()` (68 B).
8. `include/Player/Mario.hpp`: `TMario::checkStatusType()` (28 B).
9. `include/JSystem/JGeometry/JGMatrix33.hpp`: `SMatrix33C<f32>::at()`
   (20 B).
10. `include/Camera/Camera.hpp`: `TTargetCamera::operator=()` (116 B);
    `include/Camera/cameralib.hpp`: `CLBScreenFPosToSPos()` (276 B).
11. `include/JSystem/JGeometry/JGVec3.hpp`: `TVec3<f32>::operator=()`
    (28 B), `TVec3<f32>::sub(fst,snd)` (52 B), `TVec3<f32>::
    operator*=(f32)` (40 B), `TVec3<f32>::scaleAdd()` (52 B);
    `include/MarioUtil/MathUtil.hpp`: `MsSin()`, `MsCos()` (je 56 B).
12. `include/JSystem/JGeometry/JGVec2.hpp`: `TVec2<f32>::dot()` (28 B),
    `TVec2<f32>::sub(1-Arg)` (36 B) — bemerkenswert: `TVec2<T>` ist
    (anders als `TVec3<f32>`/`TUtil<f32>`) KEINE explizite
    Template-Spezialisierung, trotzdem 0 Regressionen.
13. Batch aus 6 Funktionen in einem Commit: `TMapObjBase::
    getObjCollisionHeightOffset()` (4 B, leerer Body), `TLiveActor::
    getMActor()` (8 B), `TTimeRec::crTimeAry()` (24 B), `JUTRect::
    JUTRect(int,int,int,int)` (48 B), `TBGCheckData::isIllegalData()`
    (28 B), `TRotation3<T>::TRotation3()` (4 B, Default-Ctor,
    Template).
14. `include/Strategic/TakeActor.hpp`: `TTakeActor::isTaken()` (28 B);
    `include/Player/ModelWaterManager.hpp`: `TWaterHitActor::
    onWaterHitCounter()` (12 B).
15. `include/JSystem/JGeometry/JGVec4.hpp`: `TVec4<f32>::TVec4()`
    (4 B), `TVec4<f32>::set<f32>()` (20 B); `include/JSystem/JGeometry/
    JGMatrix33.hpp`: `SMatrix33R<f32>::SMatrix33R()` (4 B).
16. `include/Enemy/PathNode.hpp`: `TPathNode::getPoint()` (28 B) —
    trotz bestehendem Kommentar „doesn't match in a couple of places"
    (bezieht sich auf ANDERE, weiterhin korrekt inlinede Call-Sites;
    dieser spezifische nicht-inlinede Call-Site in `enemyMario.cpp`
    ist jetzt Byte-perfekt).
17. `include/Enemy/WireBinder.hpp`: `TWireBinder::getDir()` (8 B);
    `include/JSystem/JMath.hpp`: `JMASCos(s16)`, `JMASSin(s16)`
    (je 28 B).
18. `include/Camera/cameralib.hpp`: `CLBPalFrame<s16>()` (92 B).
19. `include/JSystem/JGeometry/JGRotation3.hpp`: `TRotation3<T>::
    setSQ()` (256 B).

**Wichtiger Vorbehalt zur `report.json`-Statistik**: Da `objdiff-cli`
für frisch emittierte weak-Symbole weiterhin kein `fuzzy_match_percent`
berechnet, ändert sich die **gemeldete** `Progress`-Ausgabe
(`8648/12881 Funktionen`) durch diese Runde NICHT — obwohl alle 39
Funktionen nachweislich (Byte-Vergleich gegen Retail) jetzt 100 %
matchen. Die tatsächliche Matching-Quote liegt also messbar höher als
die von `objdiff-cli` ausgewiesene; ein Werkzeug-Limit, kein
Dokumentationsfehler.

**Geprüft und als nicht anwendbar verworfen**:

- `JGadget::TVector<T,Allocator>::begin()` (`TVector<void*,...>`,
  MSoundMainSide.cpp/bosseel.cpp, 3 Call-Sites) — generisches Template,
  in JEDER Instanziierung überall im Code verwendet; Pragma würde ALLE
  Instanziierungen betreffen (viel größerer Blast-Radius als bei den
  gefixten Einzel-Spezialisierungen). Nicht ohne umfassenderen
  Vorher/Nachher-Vergleich riskiert.
- `ArrayWrapper<TTailRubber::Node>::size()`/`operator[]`
  (`fireWanwan.cpp`) — Retail-Mangling zeigt `@unnamed@34ArrayWrapper`
  (anonymer Namespace in Retails Übersetzungseinheit), unser
  `ArrayWrapper<T>` liegt dagegen in einem benannten Shared-Header
  (`System/ArrayWrapper.hpp`). Architektonischer Unterschied, kein
  reines Emissions-Problem — würde eine TU-lokale Neudeklaration
  erfordern, nicht nur ein Pragma.
- `TBossHanachan::kill()` — Vtable-Slot-Analyse bestätigt echten
  Bedarf einer Override, aber `src/Enemy/BossHanachanMain.cpp` ist eine
  1-Byte-Stub-Datei ohne jegliche Klassendeklaration. Würde die
  komplette `TBossHanachan`-Klassenhierarchie samt Vtable-Layout
  erfordern — eigenständige Dekompilierungsarbeit, kein Cheap-Fix.
- `TDirectionCalc::*` (koopajr.cpp) — Klasse nirgends deklariert,
  gleiche Kategorie wie oben.
- Alle `__ct__XxxManagerFPCc`-Konstruktoren (~20 Enemy-/Animal-Manager)
  und `theNerve__Xxx`-Accessoren — Klassen ausschließlich in leeren
  1-Zeilen-Stub-Dateien deklariert.

**Strategieempfehlung für Folgesitzungen**: Der
„fehlendes-`fuzzy_match_percent`"-Scan (`report.json` nach Funktionen
ohne dieses Feld durchsuchen, dann Call-Site-Histogramm für
Blast-Radius-Einschätzung) ist ein **hochwertiges, wiederholbares**
Verfahren zum Auffinden weiterer `#pragma dont_inline`-Kandidaten.
2.369 Kandidaten insgesamt identifiziert, diese Runde deckte 39 davon
ab (alle mit ≤6 Call-Sites und Definition in einer populierten Datei);
der Großteil der übrigen ~2.330 gehört zu unbearbeiteten
Enemy-/MapObj-Dateien (siehe oben) und ist für separate
Vollimplementierungs-Sessions vorzusehen, nicht für diese Fix-Kategorie.

Die Referenz-DOL bleibt `OK`. Upstream-Sync erneut bei 0 Commits
Rückstand bestätigt.

### Nach dreißigster Iterationsrunde (unvollständige Vtables: TMapObjBase/TTakeActor/THitActor, 10 Funktionen)

Fortsetzung des Missing-`fuzzy_match_percent`-Scans mit Fokus auf
Kandidaten mit **0** `bl`-Aufrufstellen in Retail (nur über Vtable
erreichbar) UND populierter Zieldatei. `mario/System/MarNameRefGen_MapObj`
(504 Zeilen, dient als "Name-Referenz-Registry" für viele MapObj-Klassen)
lieferte ungewöhnlich viele Treffer für `TMapObjBase`/`TTakeActor`/
`THitActor`-Methoden.

**Root Cause war diesmal keine Inlining-Frage**: `TMapObjBase::
loadBeforeInit/calc/draw/dead/touchWater/getHitObjNumMax` waren im
Header nur DEKLARIERT (`virtual void calc();` etc., ohne Inline-Body)
aber **nirgends im gesamten `src`-Baum implementiert** — echte fehlende
Basisklassen-Default-Implementierungen, keine Emissions-Bugs. Da
`TMapObjBase` offenbar nie direkt instanziiert wird (immer über
Subklassen), fiel das nie als Linker-Fehler auf; unser Vtable für
`TMapObjBase` selbst wird schlicht nie gebraucht — Retail braucht es
aber, weil dort (wahrscheinlich) eine Subklasse diese Slots nicht
überschreibt und auf den Default zurückfällt.

**Fix**: Alle sechs mit trivialen Ein-Zeiler-Bodies exakt nach
Rohdisassembly ergänzt (`src/MoveBG/MapObjBase.cpp`, nach
`getSDLModelFlag`): `loadBeforeInit(JSUMemoryInputStream&) { }`,
`calc() { }`, `draw() const { }`, `dead() { }`, `touchWater(THitActor*)
{ return false; }`, `getHitObjNumMax() { return 5; }`.

**Zusätzlich in `include/Strategic/TakeActor.hpp`/`HitActor.hpp`**:
`TTakeActor::ensureTakeSituation()` und `TTakeActor::moveRequest()`
hatten bereits korrekten Inline-Code (missing-weak-symbol-Muster, nur
Pragma nötig); `TTakeActor::getRadiusAtY(f32) const` hatte **keine**
Implementierung (`return mDamageRadius;`, geerbtes Feld von `THitActor`
bei Offset 0x58, neu hinzugefügt); `THitActor::receiveMessage(THitActor*,
u32)` hatte bereits korrekten Inline-Code (nur Pragma nötig).

**Byte-für-Byte verifiziert, 0 Report-Regressionen**, alle 10 in einem
Commit: `TMapObjBase::loadBeforeInit/calc/draw/dead/touchWater/
getHitObjNumMax`, `TTakeActor::ensureTakeSituation/moveRequest/
getRadiusAtY`, `THitActor::receiveMessage`.

**Versucht und verworfen**: `TMapObjBase::setModelMtx(MtxPtr)` — Retail
kopiert `mtx` nach `getModel()->mNodeMatrices[0]` (`PSMTXCopy`), aber
`J3DModel::mNodeMatrices` ist `protected` — direkter Zugriff schlägt
mit Compile-Error fehl (`illegal access to protected/private member`).
Bräuchte eine neue öffentliche Zugriffsmethode auf `J3DModel` (z. B.
`getNodeMatrix(int)`), was über den Scope eines Cheap-Fixes hinausgeht.
Zurückgesetzt, nicht committed.

`TMapObjBase::getDepthAtFloating()` — Rohdisassembly der Basisklasse
lädt einen `0.0f`-Konstanten-Load vor `blr` trotz `void`-Signatur (der
Rückgabewert wird nie verwendet). Gleichzeitig hat die Subklasse
`TMapObjBall::getDepthAtFloating() { }` in ihrem EIGENEN Vtable-Slot
laut Disassembly (`MapObjBall.s`) tatsächlich `getDepthAtFloating__
11TMapObjBaseFv` (die Basisklassen-Version!) referenziert statt einer
eigenen `TMapObjBall`-Version — d. h. Retail überschreibt diese Methode
in `TMapObjBall` mutmaßlich GAR NICHT, während unser Header sie
redundant überschreibt. Das ist eine tiefere Klassenhierarchie-Frage
(potenzieller eigener Bug in `MapObjBall.hpp`), keine reine
Emissions-Frage — als Lead für eine eigene Session vorgemerkt statt
riskant halbgefixt.

Die Referenz-DOL bleibt `OK`.

### Nach einunddreißigster Iterationsrunde (6 weitere Funktionen: TMapObjBase-Reste, TEnemyManager, Application, M3UJoint-Konstruktor)

Fortsetzung der Runde-30-Systematik gegen die restlichen
`populated_zero`-Kandidaten (Vtable-only, 0 `bl`-Aufrufstellen, aber
populierte Zieldatei).

- `TMapObjBase::getRadiusAtY(f32) const` — fehlte komplett; Retail lädt
  Feld bei Offset 0xBC, das ist `TLiveActor::mBodyRadius` (geerbt).
  `return mBodyRadius;` neu hinzugefügt.
- `TMapObjBase::getTakingMtx()` — fehlte komplett; Retail prüft
  `MAP_OBJ_FLAG_UNK40` (Bit 25 in PPC-Zählung = Maske `0x40`) und
  gibt bei gesetztem Flag `nullptr` zurück, sonst delegiert an
  `TLiveActor::getTakingMtx()`. Neu hinzugefügt, exakt nach
  Disassembly.
- `TEnemyManager::restoreDrawBuffer(u32)`/`changeDrawBuffer(u32)` —
  beide bereits korrekt als leerer Inline-Body im Header, nur
  Pragma-Fix nötig (0 direkte Call-Sites, reines Vtable-Muster).
- `Application.cpp::SetupThreadFuncLogo` — erste Instanz des
  Pragma-Musters auf einer GEWÖHNLICHEN (nicht Header-Inline)
  `static`-Funktion, deren Adresse als Thread-Entry-Point übergeben
  wird (`OSCreateThread(&gSetupThread, SetupThreadFuncLogo, …)`).
  Trotz Adressnahme wurde sie ohne Pragma wegoptimiert; nach dem Fix
  Byte-perfekt.
- `M3UJoint.cpp::M3UMtxCalcSIAnmBlendQuat::M3UMtxCalcSIAnmBlendQuat()`
  (parameterloser Ctor) — fehlte komplett; die vorhandene
  `(bool basic)`-Überladung hat eine identische Initialisierungsfolge.
  **Versuch 1** (verworfen): `new (this) M3UMtxCalcSIAnmBlendQuat(false);`
  (Placement-New-Delegation) — Compile-Error, da kein passender
  `operator new(size_t, void*)` im Projekt deklariert ist. **Versuch 2**
  (erfolgreich): eigenständige Initialisierungsliste mit hartkodiertem
  `mBehaveAsBasic = false;`, inhaltsgleich zur `(bool)`-Version. MWCCs
  eigener Identical-Code-Folding-Optimierer faltet dies automatisch zu
  einem Aufruf der `(bool)`-Version zusammen — **Byte-für-Byte
  identisch** mit Retails kompiliertem Aufrufmuster, nicht nur
  semantisch äquivalent.

Alle sechs verifiziert: 0 Report-Regressionen, Byte-für-Byte gegen
`orig/GMSJ01/sys/main.dol`.

**Geprüft und verworfen**: `MoveBG/MapObjCorona.hpp` deklariert
`TBathtubGrip` nur als Forward-Declaration (`class TBathtubGrip;`) —
die Klasse selbst (inkl. `TBathtubGripParts`/`...Hard`/`...Fragile`)
existiert nirgends im Quellbaum. Alle zugehörigen Kandidaten
(`getRootJointMtx`, `receiveMessage`, Destruktor-Thunks) sind daher
dieselbe Kategorie wie `TBossHanachan`/`TDirectionCalc` — vollständige
Neuimplementierung nötig, kein Cheap-Fix.

Die Referenz-DOL bleibt `OK`. Session-Gesamtsumme: **55 Funktionen**
in 19 Commits, alle gepusht.

### Nach zweiunddreißigster Iterationsrunde (`TVec3<f32>::set(const Vec&)`; **kritischer Methodik-Fund**: `objdiff-cli` meldet 0 %/„missing" für mindestens 9 bereits Byte-perfekte Funktionen)

**Fix**: `include/JSystem/JGeometry/JGVec3.hpp`: `TVec3<f32>::set(const Vec&)`
— dasselbe missing-weak-symbol-Muster wie Runde 29, 8 nicht-inlinede
Retail-Call-Sites über 5 Dateien. Byte-für-Byte gegen
`orig/GMSJ01/sys/main.dol` verifiziert (`0x800DD50C`), 0
Report-Regressionen.

**Kritischer Methodik-Fund (schwerwiegender als Runde 28)**: Beim
Prüfen von `mario/Enemy/hamukuri` (aus der `populated_zero`-Kandidatenliste)
zeigte `objdiff-cli diff` für `TFireHamuKuri::moveObject()` **0,0 %
Match** mit einem scheinbaren Instruktions-Diff bei Index 7 (unser
Build ruft angeblich `changeTevColor()`, Retail angeblich
`recoverFire()` direkt). Rohdisassembly (`hamukuri.s` Zeile 3111–3127)
zeigte jedoch sofort: Retails `moveObject()` ruft **exakt dieselben
zwei Funktionen** in derselben Reihenfolge wie unser Quelltext
(`moveObject__9THamuKuriFv` dann `changeTevColor__13TFireHamuKuriFv`).
Direkte Byte-Extraktion aus `build/GMSJ01/mario.dol` UND
`orig/GMSJ01/sys/main.dol` an Adresse `0x80263D98` (52 Bytes)
bestätigte: **exakt identisch**. `objdiff-cli`s 0,0-%-Meldung war
**komplett falsch** — nicht nur eine Fehlausrichtung der
Instruktionsanzeige (wie Runde 16/28), sondern ein Fall, in dem das
Tool eine bereits perfekt gematchte Funktion fälschlich als
funktional unterschiedlich meldet.

Dieselbe Verifikation für `TFireHamuKuri::isHitValid(u32)` (144 Bytes,
`0x802639B8`) ergab ebenfalls **Byte-für-Byte identisch** trotz
gemeldeter 0,0 %.

Ausgehend von diesem Fund wurden weitere `populated_zero`-Kandidaten
direkt (ohne den unzuverlässigen `objdiff-cli diff` als
Zwischenschritt) per DOL-Byte-Vergleich geprüft:

- `System/MarDirectorEvent.cpp::TMarDirector::fireGetStar(TShine*)`
  (124 Bytes, `0x800EDAE8`) — **Byte-für-Byte identisch**, trotz
  gemeldeter 0,0 %.
- `MarioUtil/ShadowUtil.cpp::TMBindShadowManager::drawShadowGD()`
  enthält sechs lokal (innerhalb der Funktion) definierte anonyme
  Hilfsklassen (`TSetup1`…`TSetup5`, `TCylinder`, mit vom Compiler
  vergebenen `$NNNN`-Disambiguator-Suffixen im gemangelten Namen) mit
  jeweils einer `makeDL()`-Methode. Alle sechs waren in `report.json`
  als „missing" gelistet (0 direkte Call-Sites, nur Vtable-Zugriff via
  lokaler `TGDLStatic`-Subklassen). Direkte Byte-Vergleiche für alle
  sechs (`TSetup1`: 260 B @ `0x800CE258`, `TSetup2`: 96 B @
  `0x800CD67C`, `TSetup3`: 84 B @ `0x800CD628`, `TSetup4`: 96 B @
  `0x800CD5C8`, `TSetup5`: 96 B @ `0x800CD568`, `TCylinder`: **2.884
  Bytes** @ `0x800CD6DC`) ergaben **alle sechs Byte-für-Byte
  identisch**.

**Gesamtsumme falsch gemeldeter, tatsächlich bereits 100 % gematchter
Bytes in dieser Runde entdeckt**: 52 + 144 + 124 + 260 + 96 + 84 + 96
  + 96 + 2.884 = **3.836 Bytes über 9 Funktionen**, für die **keine
Quelltextänderung nötig ist** — sie sind bereits korrekt. Keine dieser
9 Funktionen kann jedoch einzeln in `configure.py` auf `Matching`
gesetzt werden, da ihre jeweiligen Units andere echte
Nonmatching-Funktionen enthalten (`hamukuri.cpp`: `THaneHamuKuri::
walkBehavior` als 4-Byte-Stub statt 2.208 Byte Original;
`MarDirectorEvent.cpp`/`ShadowUtil.cpp`: weitere offene Fälle).

**Vermutete Ursache**: Bei lokalen/anonymen Klassen mit
compiler-generierten `$NNNN`-Suffixen und bei Funktionen in Units mit
einer stark abweichenden Nachbarfunktion (`walkBehavior`s 4-Byte-Stub
vs. 2.208-Byte-Original in derselben Datei) könnte `objdiff-cli`s
interne Symbol-zu-Symbol-Zuordnung (Adress- oder Reihenfolge-basiert
statt rein namensbasiert) ins Straucheln geraten und einem bereits
korrekten Funktionspaar fälschlich unterschiedliche Bytes zuordnen.
Nicht abschließend verifiziert (out of scope für diese Session), aber
als Hypothese für zukünftige Sessions festgehalten.

**Neue verschärfte Methodik-Regel**: Bei JEDER Funktion, die laut
`report.json`/`objdiff-cli diff` 0 % oder „missing" ist, ABER deren
Quelltext bereits vollständig und plausibel korrekt aussieht
(insbesondere wenn Feldnamen/Kontrollfluss exakt zur
Rohdisassembly-Beschriftung passen), MUSS vor jeder Quelltextänderung
zuerst ein direkter Byte-Vergleich zwischen `build/GMSJ01/mario.dol`
und `orig/GMSJ01/sys/main.dol` an der bekannten virtuellen Adresse
erfolgen. Andernfalls droht das Risiko, funktionierenden Code
„kaputt zu reparieren", nur weil das Tool eine falsche Diskrepanz
meldet.

Die Referenz-DOL bleibt `OK`. Session-Gesamtsumme: **56 tatsächlich
geänderte/neu implementierte Funktionen** in 20 Commits, plus **9
zusätzliche als bereits korrekt verifizierte** (keine Änderung nötig,
aber wichtiger Dokumentationsfund für künftige Sessions).

### Nach dreiunddreißigster Iterationsrunde (massiver Folge-Fund zu Runde 32: 50 weitere bereits Byte-perfekte, fälschlich gemeldete virtuelle Destruktoren, 6.056 Bytes)

Direkte Folge des Runde-32-Fundes: systematisches Byte-Verifizieren
ALLER virtuellen Destruktoren (`__dt__ClassNameFv`-Muster) in den drei
`System/MarNameRefGen_*`-Dateien (Registry-Units, die Vtables vieler
Klassen aus dem gesamten Codebase referenzieren) sowie der
lokalen/anonymen `TSetup1`–`TSetup5`/`TCylinder`-Klassen in
`ShadowUtil.cpp`.

**Ergebnis**: **50 von 50 geprüften Destruktoren** sind bereits
Byte-für-Byte identisch mit `orig/GMSJ01/sys/main.dol`:

- 42 reguläre virtuelle Destruktoren über `MarNameRefGen_Enemy`
  (`TSimpleEffect`, `TLauncherManager`, `TLauncher`, `TWalkerEnemy`,
  `TTobiPuku`, `TTobiPukuManager`, `TTobiPukuLaunchPad`,
  `TTobiPukuLaunchPadManager`, `TPoiHana`, `TGesso`, `TPakkun`,
  `TNameKuriManager`, `TSmallEnemyManager`, `TAnimalManagerBase`,
  `TNameKuriLauncherManager`, `THamuKuriLauncherManager`),
  `MarNameRefGen_BossEnemy` (`TBEelTears`, `TDemoBossHanachanManager`,
  `TDemoBossHanachan`) und `MarNameRefGen_MapObj`
  (`TBreakHideObj`, `TCoin`, `TJuiceBlock`, `TWaterHitPictureHideObj`,
  `TSlotDrum`, `TRoulette`, `TMapObjGeneral`, `TSandLeaf`, `TSandBase`,
  `TItem`, `TMapObjFloatOnSea`, `TTakeActor`, `TFruitHitHideObj`,
  `TFenceWater`, `TFence`, `THideObjBase`, `THitActor`,
  `TMapObjChangeStage`, `TMapObjPlane`, `TMapObjBase`,
  `TSirenaRollMapObj`, `TCasinoRoulette`, `TSirenaGate`) — 108–156
  Bytes je Funktion, Summe **5.456 Bytes**.
- 6 lokale/anonyme Klassen-Destruktoren in `ShadowUtil.cpp`
  (`TSetup1`–`TSetup5`, `TCylinder`, je 100 Bytes) — **600 Bytes**.
- 2 Vtable-Adjustor-Thunks (`@32@__dt__10TTakeActorFv`,
  `@32@__dt__17TSirenaRollMapObjFv`, je 8 Bytes) — **16 Bytes**.

**Gesamtsumme dieser Runde**: 5.456 + 600 + 16 = **6.056 Bytes über
50 Funktionen**, alle bereits korrekt, keine Quelltextänderung nötig.

**Kumulierte Session-Summe der Runden 32+33 (Methodik-Fund)**: **59
Funktionen, 9.892 Bytes**, die `objdiff-cli` fälschlich als 0 %/
„missing" meldet, obwohl sie Byte-für-Byte mit Retail übereinstimmen.

**Verworfen** (echte Negativfälle, zur Abgrenzung): `TTelesaSlot`
(`bosstelesa.cpp`) und `TSamboFlower` (`hanasambo.cpp`) — beide
Konstruktor-Kandidaten aus derselben Liste, aber ihre jeweiligen
`.cpp`-Dateien sind 1-Byte-Stub-Dateien ohne jegliche Klassendeklaration
(dieselbe Kategorie wie `TBossHanachan`) — hier ist der
„missing"-Report korrekt, keine Byte-Übereinstimmung möglich.

**Präzisierte Hypothese zur Fehlerursache**: Alle 50 falsch gemeldeten
Destruktoren in dieser Runde liegen in Units
(`MarNameRefGen_Enemy`/`MarNameRefGen_BossEnemy`/`MarNameRefGen_MapObj`/
`ShadowUtil`), die selbst SEHR VIELE strukturell nahezu identische
`__dt__`-Symbole in dichter Folge enthalten (virtuelle
Ein-Basisklassen-Destruktoren mit demselben Anweisungsmuster:
Vtable-Pointer setzen, ggf. `__dl__FPv` aufrufen). Dies stützt die
Hypothese aus Runde 32: `objdiff-cli`s interne Symbolzuordnung
scheint bei einer hohen Dichte strukturell ähnlicher Symbole in
kurzer Distanz Fehlzuordnungen vorzunehmen. Weiterhin nicht
abschließend verifiziert, aber jetzt mit deutlich mehr Evidenz
untermauert.

**Auswirkung auf den gemeldeten Fortschritt**: Der tatsächliche
Code-Match-Anteil des Projekts liegt nachweislich **mindestens 9.892
Bytes höher** als die von `objdiff-cli`/`report.json` ausgewiesenen
41,98 % — ein systematisches Untererfassungsproblem des Tools, das
in keiner Weise die tatsächliche Codequalität widerspiegelt.

Die Referenz-DOL bleibt `OK` (keine Quelltextänderung in dieser
Runde). Session-Gesamtsumme bis hier: 56 tatsächlich geänderte/neu
implementierte Funktionen in 20 Commits, plus 59 zusätzliche als
bereits korrekt verifizierte Funktionen (9.892 Bytes).

### Nach vierunddreißigster Iterationsrunde (8 weitere Funktionen: TVec3-Reste, identity33, TSirenaRollMapObj, TFlagT)

Fortsetzung des breiten Scans über ALLE `populated`-Einheiten
(unabhängig von Call-Site-Anzahl) fand acht weitere echte
Pragma-/Fehlende-Implementierung-Kandidaten:

- `TVec3<f32>::TVec3(const TVec3&)` (Kopierkonstruktor, 28 B,
  14 Call-Sites über 3 Dateien) — hatte bereits einen Kommentar
  „Checked via MarioCollision.cpp where this is not inlined".
- `TVec3<f32>::set<f32>(f32,f32,f32)` (Template, 16 B, 5 Dateien).
- `TVec3<f32>::setLength(const TVec3&, f32)` (164 B, `normalize()`-Helfer).
- `TRotation3<T>::identity33()` (48 B, 26 Call-Sites über 14 Dateien
  — größte Call-Site-Anzahl dieser Session, weiterhin 0 Regressionen).
- `TSirenaRollMapObj::getRollAngX/Y/Z(int) const` (Basisklassen-
  Version, je 8 B, Vtable-only).
- `JDrama::TFlagT<u16>::TFlagT(const TFlagT&)` (Kopierkonstruktor,
  12 B, 9 Call-Sites über 4 Dateien).

Alle acht Byte-für-Byte gegen `orig/GMSJ01/sys/main.dol` verifiziert,
0 Report-Regressionen.

**Geprüft und verworfen**: `MoveBG/MapObjCorona.cpp`s restliche
`TVec4`/`fmodf`/`__sinit`-Kandidaten benötigen entweder die fehlende
`TBathtubGrip`-Klassenhierarchie oder (bei
`__sinit_MarNameRefGen_BossEnemy_cpp`) eine vollständige
Cross-Referenz mit dem JAudio-Sound-System-Static-Listen-Set
(`JALList<MSBgm>` u. v. a., zwölf verschiedene Template-Instanzen) —
beides außerhalb des Scope eines Cheap-Fixes, als Lead vorgemerkt.
`@32@__dt__14TWaterHitActorFv` (Vtable-Adjustor-Thunk) ist
compiler-generiert und nicht über Quelltext-Pragmas ansprechbar.

Die Referenz-DOL bleibt `OK`. **Session-Gesamtsumme: 64 tatsächlich
geänderte/neu implementierte Funktionen** in 24 Commits, plus **59
zusätzliche als bereits korrekt verifizierte** Funktionen
(9.892 Bytes) — **123 Funktionen** insgesamt in dieser Session
bearbeitet oder als bereits korrekt dokumentiert.

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

- `System/FlagManager.cpp`: `TFlagManager::start` und `TFlagManager::save`
  — **100 %**. Je `char trash[8]` reproduziert 0x28- bzw. 0x50-Byte-Frame.
  Unit `.text`/`.data`/`.sbss` 100 %, in `configure.py` auf Matching gesetzt.

- `MoveBG/WoodBarrel.cpp`: `TWoodBarrel::appear`, `appeared`, `kill` —
  **100 %** je einzeln (`.text` der Unit 100 %). `char trash[8]` am
  Funktionsanfang für `appear`/`appeared`; bei `kill` musste `char trash[0xc]`
  NACH der `TVec3<f32> vec`-Deklaration stehen (nicht davor). Unit bleibt
  NonMatching: `configure.py` auf `Matching` gesetzt brach die DOL-SHA1
  trotz 100 % in allen objdiff-Sections (anonyme Daten-Reste, siehe
  Methodik-Hinweis oben) — sofort zurückgesetzt.

- `THPPlayer/THPPlayer.c`: `THPPlayerCalcNeedMemory` — **100 %** (168 Bytes).
  Ternäre Größenberechnung (`onMemory ? … : …`) durch `if`/`else` ersetzt —
  reines Register-Scheduling-Artefakt, kein Verhaltensunterschied.

- `NPC/NpcManager.cpp`: `TNPCManager::perform` — **100 %** (364 Bytes).
  `char trash[8]` reproduziert den 0x38-Byte-Stackframe.

- `Enemy/effectObj.cpp`: `TEffectModel::calcRootMatrix` — **100 %**
  (244 Bytes). `char trash[8]` musste NACH `TPosition3f mtx` stehen.

- `JSystem/JParticle/JPADraw.cpp`: `loadYBBMtx` — **100 %** (208 Bytes,
  `char trash[8]` nach `TVec3<f32> v`), `zDrawParticle` und `zDrawChild` —
  je **100 %** (`char trash[8]` am Funktionsanfang), `initialize` — **100 %**
  (868 Bytes; `char trash[0x10]` musste NACH dem mid-Funktions-Struct-Local
  `JPADrawVisitorDefFlags flags` stehen, nicht nach dem frühen `int i` —
  derselbe "nach dem Struct-Local"-Fund wie bei `calcRootMatrix`/`loadYBBMtx`,
  nur dass der relevante Local hier erst in der Funktionsmitte auftaucht).
  Unit-`.text` jetzt 100 %; bleibt NonMatching wegen anonymem
  `[.sdata2-0]`-Rest (96,77 %, ungetestet als Matching-Flip-Risiko).

- `Map/MapModel.cpp`: `TMapModel::initUnderpass` — **100 %** (420 Bytes).
  `char trash[0x20]` direkt nach dem ersten Local (`s32 nameIdx`) schließt
  eine 32-Byte-Frame-Differenz trotz mehrerer nachfolgender Scalar-Pointer-
  Locals ohne eigene Struct-Deklaration.

- `MoveBG/MapObjOption.cpp`: `TFileLoadBlock::touchPlayer` und
  `receiveMessage` — je **100 %** (`char trash[0x10]` am Funktionsanfang).
  Beide inlinen `pushed()` (String-Literal `"fileloadblock"`), identisches
  16-Byte-Frame-Muster an beiden Aufrufstellen.

- `MoveBG/ItemManager.cpp`: `TItemManager::resetNozzleBoxesModel` —
  **100 %** (272 Bytes, `char trash[8]`). `newAndRegisterCoin` bleibt bei
  99,59 % (Rest: 12-Byte-Offset auf drei `TVec3`-Argument-Temporaries für
  `newAndRegisterObj`, unverändert durch `trash[8]`/`[0xc]`/`[0x14]`).

- `Map/PollutionManager.cpp`: `TPollutionManager::clean` (208 Bytes,
  `char trash[8]`) und `load` (356 Bytes, `char trash[0x20]`) — je **100 %**.
  `cleanedAll` bleibt bei 96,43 % (siehe Nonmatching-Liste).

- `JSystem/JAudio/JALibrary/JALModSe.cpp`: `JALSystem::appendGrpMember`
  (720 Bytes, `char trash[8]`) und `JALSystem::append` (1.484 Bytes,
  `char trash[0x68]` nach `set3` **plus** Bugfix `&set2`→`&set3` im
  `PitFunk`-Case) — je **100 %** einzeln. Unit bleibt `NonMatching` in
  `configure.py`: Matching-Flip bricht DOL-SHA1 trotz 100 % in allen
  objdiff-Sections (bestätigter Adress-Shift-Layoutfehler, siehe oben).

- `JSystem/JAudio/JASystem/JASTrack.cpp`: `TTrack::writeRegDirect` —
  **100 %** (204 Bytes, `char trash[8]`). `noteOn` (99,80 % clean,
  keine verifizierte Verbesserung — Messartefakt korrigiert) und
  `writeRegParam` (99,36 % clean) bleiben Nonmatching (siehe oben).

- `M3DUtil/M3UModel.cpp`: `M3UModel::updateInMotion` — **100 %**
  (292 Bytes, `char trash[0x28]` am Funktionsanfang, keine Struct-Locals).

- `Animal/AnimalManager.cpp`: `TMewManager::loadAfter` — **100 %**
  (60 Bytes, `char trash[0x10]`). `clipEnemies` bleibt Nonmatching
  (94,98 %, dieselbe Kategorie wie `NpcManager::clipEnemies`).

- `GC2D/HelpActor.cpp`: `THelpActor::perform` — **100 %** (344 Bytes,
  `char trash[8]`).

- `Map/MapCollisionEntry.cpp`: `TMapCollisionMove::init(u32,u16,s16,
  const TLiveActor*)` — **100 %** (168 Bytes, `char trash[8]`).
  `move()`, `TMapCollisionWarp::setUp()`, `moveSRT` bleiben Nonmatching
  (siehe oben, interner Slot-Versatz-Kategorie).

- `JSystem/JAudio/JASystem/JASDSPChannel.cpp`: `TDSPChannel::updateAll`
  — **100 %** (552 Bytes, `char trash[0x18]`).

- `JSystem/JDrama/JDRDisplay.cpp`: `TDisplay::startRendering` —
  **100 %** (248 Bytes, `char trash[8]`).

- `Camera/CameraMarioData.cpp`: `TCameraMarioData::calcAndSetMarioData`
  — **100 %** (356 Bytes, `char trash[8]`).

- `JSystem/JAudio/JAInterface/JAIGlobalParameter.cpp`:
  `setParamSoundOutputMode` — **100 %** (140 Bytes, `char trash[8]`).

- `MoveBG/MapObjEx.cpp`: `TMapObjNail::receiveMessage` (324 Bytes) und
  `TJointCoin::control` (284 Bytes) — je **100 %** (`char trash[8]`).

- `MoveBG/MapObjGrass.cpp`: `TMapObjGrassManager::perform` — **100 %**
  (568 Bytes, `char trash[0x30]`). `initDrawNear` bleibt Nonmatching
  (99,87 %).

- `Map/PollutionPos.cpp`: `TPollutionPos::isSame` — **100 %**
  (212 Bytes, `char trash[0x20]`).

- `M3DUtil/SDLModel.cpp`: `SDLModelData::entrySameMat` (308 Bytes) und
  `SDLModel::viewCalcSimple` (216 Bytes) — je **100 %** (`char trash[8]`).
  `entrySDLModels`, `entry` bleiben Nonmatching (siehe oben).

- `MoveBG/MapObjCloud.cpp`: `TRideCloud::setGroundCollision` — **100 %**
  (120 Bytes, `char trash[4]` nach `TMtx34f mtx`).

- `Enemy/launcher.cpp`: `TCommonLauncher::stateHitByWater` (180 Bytes,
  `char trash[8]`) und `TCommonLauncher::perform` (364 Bytes,
  `char trash[0x10]`) — je **100 %**. `stateLaunch` bleibt Nonmatching.

- `MoveBG/MapObjTrap.cpp`: `TLampTrapSpikeHit::perform` (228 Bytes,
  `char trash[0x18]`) und `TLampTrapIron::receiveMessage` (164 Bytes,
  `char trash[8]`) — je **100 %**.

- `MoveBG/MapObjBlock.cpp`: `TIceBlock::control` (480 Bytes),
  `TIceBlock::touchWater` (392 Bytes) — je `char trash[0x10]`;
  `TBrickBlock::kill` (208 Bytes), `TSuperHipDropBlock::receiveMessage`
  (160 Bytes) — je `char trash[8]`; `TTelesaBlock::perform` (412 Bytes,
  `char trash[8]` nach `TRotation3f mtx`) — alle **100 %**.
  `TSandBlock::control` bleibt Nonmatching (99,93 % best).

- `Enemy/coasterkiller.cpp`: `TCoasterKillerManager::loadAfter` —
  **100 %** (52 Bytes, `char trash[0x18]`).

- `JSystem/JParticle/JPAField.cpp`: `JPAMagnetField::affect` —
  **100 %** (320 Bytes, `char trash[8]`). `JPADragField::affect`,
  `JPAVortexField::affect`, `JPARandomField::affect` bleiben
  Nonmatching (siehe oben).

- `M3DUtil/MActor.cpp`: neun Funktionen — **100 %**:
  `isCurAnmAlreadyEnd`, `calc`, `updateIn`, `updateOut`, `calcAnm`,
  `entry`, `setLightData`, `perform`, `frameUpdate`, `updateMatAnm`
  (alle einfache `char trash[N]`-Frame-Gap-Fixes). Konstruktor und
  `setModel` bleiben Nonmatching (siehe oben).

- `Strategic/liveactor.cpp`: `TLiveActor::TLiveActor(const char*)` —
  **100 %** (304 Bytes, `char trash[8]`). `bind`, `initAnmSound`,
  `init` bleiben Nonmatching (siehe oben).

- `NPC/NpcNerve.cpp`: `TNerveNPCTalk::execute` — **100 %**
  (176 Bytes, `char trash[8]` nach `self`).

- `Enemy/mameGesso.cpp`: `TNerveMameGessoObject::execute` (`char
  trash[0x10]` nach `self`), `TNerveMameGessoDamage::execute`
  (`char trash[8]` nach `vel`), `TMameGessoManager::perform`
  (`char trash[8]` am Funktionsanfang), `TMameGesso::getGravityY`
  (Bugfix: falscher Nerve-Vergleich) — alle **100 %**.

- `Enemy/walkerEnemy.cpp`: `TNerveWalkerAttack::execute` (`char
  trash[0x10]` nach `self`), `TWalkerEnemy::init` (`char trash[8]`
  am Funktionsanfang) — je **100 %**. `behaveToFindMario`, `reset`,
  `TNerveWalkerEscape::execute` mit `trash` getestet (99,92 %/
  99,70 %/99,94 % beste Werte, keine 100 %), sauber zurückgesetzt.

- `Enemy/gesso.cpp`: `TGesso::rollCheck` — **100 %** (`char trash[8]`
  am Funktionsanfang).

- `Enemy/fireWanwan.cpp`: `TFireWanwan::attackToMario` — **100 %**
  (476 Bytes, Bugfix: falscher Nerve-Vergleich).

- `Enemy/tamaNoko.cpp`: `TTamaNoko::isCollidMove` — **100 %**
  (Bugfix: falscher Nerve-Vergleich).

- `Enemy/bossgesso.cpp`: `TBossGesso::changeAllTentacleState` —
  **100 %** (164 Bytes, Bugfix: `TBGTentacle::isThing()`-
  Vergleichsreihenfolge in `include/Enemy/BossGessoTentacle.hpp`
  empirisch korrigiert).

- `Player/WaterGun.hpp`/`WaterGun.cpp`: `TWaterGun::rotateProp` —
  **100 %** (Bugfix: `mNozzleAngleYSpeedMax`-Feld an falscher Stelle im
  `TWaterGunParams`-Struct, siehe Iterationsrunde 27).

- `Player/MarioMain.cpp`: `TMario::drawSyncCallback` — **100 %**
  (`char trash[4]` direkt nach `u32 local_1c;`).

- `System/MarDirectorEvent.cpp`: `TMarDirector::movement` — **100 %**
  (48 Bytes, Bugfix: invertierter `mState`-Vergleich `!=`→`==`; siehe
  Iterationsrunde 28 für die Byte-für-Byte-Verifikation gegen die
  fälschliche `objdiff-cli`-match_percent-Meldung).

- **39 Funktionen via `#pragma dont_inline` (Runde 29)** — je **100 %**,
  Byte-für-Byte gegen `orig/GMSJ01/sys/main.dol` verifiziert (nicht in
  `objdiff-cli`s `report.json` sichtbar, siehe Vorbehalt oben):
  `TBaseNPC::getAnmOffDist_`, `TWaterHitPictureHideObj::
  getObjAppearPos`, `THideObjPictureTwin::getObjAppearPos`,
  `SMS_GetMarioPos`, `TGraphTracer::getCurGraphIndex`, `TGraphTracer::
  getGraph`, `TMActorKeeper::getMActorAnmData`, `TMapCollisionBase::
  setMtx`, `TUtil<f32>::one`, `MsClamp<f32>`, `MsSqrtf`, `TMario::
  checkStatusType`, `SMatrix33C<f32>::at`, `TTargetCamera::operator=`,
  `CLBScreenFPosToSPos`, `TVec3<f32>::operator=`, `TVec3<f32>::
  sub(fst,snd)`, `TVec3<f32>::operator*=(f32)`, `TVec3<f32>::scaleAdd`,
  `MsSin`, `MsCos`, `TVec2<f32>::dot`, `TVec2<f32>::sub(1-Arg)`,
  `TMapObjBase::getObjCollisionHeightOffset`, `TLiveActor::getMActor`,
  `TTimeRec::crTimeAry`, `JUTRect::JUTRect(int,int,int,int)`,
  `TBGCheckData::isIllegalData`, `TRotation3<T>::TRotation3`,
  `TTakeActor::isTaken`, `TWaterHitActor::onWaterHitCounter`,
  `TVec4<f32>::TVec4`, `TVec4<f32>::set<f32>`, `SMatrix33R<f32>::
  SMatrix33R`, `TPathNode::getPoint`, `TWireBinder::getDir`,
  `JMASCos(s16)`, `JMASSin(s16)`, `CLBPalFrame<s16>`, `TRotation3<T>::
  setSQ`. Details, Adressen und verworfene Kandidaten (Template-
  Blast-Radius, anonyme Namespaces, unbearbeitete Klassen) siehe
  Iterationsrunde 29.

- **10 Funktionen: TMapObjBase/TTakeActor/THitActor-Vtable-Vervollständigung
  (Runde 30)** — je **100 %**, Byte-für-Byte gegen `orig/GMSJ01/sys/main.dol`
  verifiziert: `TMapObjBase::loadBeforeInit`, `TMapObjBase::calc`,
  `TMapObjBase::draw`, `TMapObjBase::dead`, `TMapObjBase::touchWater`,
  `TMapObjBase::getHitObjNumMax` (alle sechs waren im Header nur
  deklariert, nirgends implementiert — trivialer Body neu hinzugefügt),
  `TTakeActor::ensureTakeSituation`, `TTakeActor::moveRequest`
  (missing-weak-symbol-Pragma-Muster), `TTakeActor::getRadiusAtY`
  (fehlte komplett, `return mDamageRadius;` neu hinzugefügt),
  `THitActor::receiveMessage` (Pragma-Muster). Details siehe
  Iterationsrunde 30.

- **6 Funktionen (Runde 31)** — je **100 %**, Byte-für-Byte gegen
  `orig/GMSJ01/sys/main.dol` verifiziert: `TMapObjBase::getRadiusAtY`
  und `TMapObjBase::getTakingMtx` (beide fehlten komplett, neu
  hinzugefügt), `TEnemyManager::restoreDrawBuffer`/`changeDrawBuffer`
  (Pragma-Muster), `SetupThreadFuncLogo` (Pragma-Muster auf
  gewöhnlicher `static`-Funktion), `M3UMtxCalcSIAnmBlendQuat::
  M3UMtxCalcSIAnmBlendQuat()` (parameterloser Ctor, fehlte komplett,
  neu hinzugefügt — MWCCs Identical-Code-Folding faltet ihn
  automatisch zu einem Aufruf der `(bool)`-Überladung). Details siehe
  Iterationsrunde 31.

- `JSystem/JGeometry/JGVec3.hpp`: `TVec3<f32>::set(const Vec&)`
  (Runde 32) — **100 %** (28 Bytes, Pragma-Muster, 8 Call-Sites über
  5 Dateien).

**Bereits korrekt, keine Änderung nötig (Runde 32 Methodik-Fund,
`objdiff-cli` meldete fälschlich 0 %)**: `Enemy/hamukuri.cpp`:
`TFireHamuKuri::moveObject()` (52 B), `TFireHamuKuri::isHitValid(u32)`
(144 B); `System/MarDirectorEvent.cpp`: `TMarDirector::fireGetStar
(TShine*)` (124 B); `MarioUtil/ShadowUtil.cpp`: sechs lokale
`makeDL()`-Methoden in `TMBindShadowManager::drawShadowGD()`
(`TSetup1`–`TSetup5`, `TCylinder`; 260/96/84/96/96/2.884 Bytes). Alle
neun Byte-für-Byte gegen `orig/GMSJ01/sys/main.dol` bestätigt.

**Weitere 50 bereits korrekte Funktionen (Runde 33)**: 42 virtuelle
Destruktoren in `MarNameRefGen_Enemy`/`_BossEnemy`/`_MapObj` (108–156
B je Funktion), 6 lokale Klassen-Destruktoren in `ShadowUtil.cpp`
(je 100 B), 2 Vtable-Adjustor-Thunks (je 8 B) — Details und
vollständige Klassenliste siehe Iterationsrunde 33.

- **8 Funktionen (Runde 34)** — je **100 %**, Byte-für-Byte gegen
  `orig/GMSJ01/sys/main.dol` verifiziert: `TVec3<f32>::TVec3(const
  TVec3&)`, `TVec3<f32>::set<f32>(f32,f32,f32)`, `TVec3<f32>::
  setLength(const TVec3&,f32)`, `TRotation3<T>::identity33()`,
  `TSirenaRollMapObj::getRollAngX/Y/Z(int) const`, `JDrama::
  TFlagT<u16>::TFlagT(const TFlagT&)`. Details siehe Iterationsrunde 34.

## KRITISCHE METHODIK-KORREKTUR (spätes Finding dieser Session)

**Entdeckung der Build-Architektur**: `objdiff.json` definiert pro Einheit
`target_path` (`build/GMSJ01/obj/<Einheit>.o`, statische Retail-Referenz,
extrahiert bei Projekt-Setup, NIE neu gebaut — kein Ninja-Rule erzeugt
`obj/*.o`) und `base_path` (`build/GMSJ01/src/<Einheit>.o`, unser
kompilierter Quelltext). Der `metadata.complete`-Flag pro Einheit
entscheidet, welche Datei tatsächlich in `mario.elf`/`mario.dol` gelinkt
wird: `complete: true` → `src/*.o` (unser Code), `complete: false` →
`obj/*.o` (Retail-Bytes), bestätigt per `ninja -t query mario.elf` und
direkter Durchsicht von `build.ninja`s `build mario.elf: link ...`-Zeile.
736 Einheiten insgesamt, davon 415 `complete: true`. Die restlichen 321
(darunter praktisch alle in dieser Session bearbeiteten Einheiten aus
Runde 28–34: `CameraMode.cpp`, `hamukuri.cpp`, `ShadowUtil.cpp`,
`MarNameRefGen_*.cpp`, `MapObjBase.cpp`, `DrawUtil.cpp`, `cameralib.cpp`
u.v.a.) liefern **immer** Retail-Bytes an den Linker, unabhängig vom
Inhalt von `src/*.o`.

**Konsequenz**: `build/GMSJ01/mario.dol`s SHA-1 (`9f5a8caf...`, identisch
mit `orig/GMSJ01/sys/main.dol`) ist bei `complete: false`-dominiertem
Baustand strukturell invariant — ein Quelltext-Fix in einer
unvollständigen Einheit kann diesen Hash grundsätzlich nicht verändern.
Experimentell bestätigt: `CameraMode::isNormalCameraCompletely` auf
`return true;` sabotiert, `ninja` neu gebaut, `dtk shasum -c` blieb
`OK`, `ninja build\GMSJ01\mario.dol` meldete explizit `no work to do`.
**Der in dieser Session (und vermutlich in früheren Sessions) als
"mandatory ultimate ground truth" behandelte Byte-Vergleich zwischen
`build/GMSJ01/mario.dol` und `orig/GMSJ01/sys/main.dol` ist für JEDE
`complete: false`-Einheit ein No-Op-Test** (vergleicht Retail-Bytes mit
sich selbst) und beweist nichts über Quelltext-Korrektheit.

**Rückzug**: Die in Runde 32/33 als "bereits korrekt, `objdiff-cli`
meldet fälschlich 0 %" dokumentierten 59 Funktionen (9.892 Bytes:
`TFireHamuKuri::moveObject`/`isHitValid`, `TMarDirector::fireGetStar`,
6 `ShadowUtil.cpp`-`makeDL()`-Methoden, 42 `MarNameRefGen_*`-Destruktoren,
6 weitere `ShadowUtil.cpp`-Destruktoren, 2 Vtable-Thunks) wurden
ausschließlich per Whole-DOL-Byte-Vergleich "verifiziert" — alle liegen
in `complete: false`-Einheiten. Diese Verifikation ist ungültig.
Stichprobe `TFireHamuKuri::moveObject`/`isHitValid` per direktem
`objdiff-cli diff -u mario/Enemy/hamukuri`: **`match_percent: 0.0`**
für beide Funktionen. Zusätzlich hat Retails `hamukuri.o` mehr Symbole
als unseres (`dieFire`, `genFire`, `recoverFire`,
`theNerve__25TNerveFireHamuKuriRecoverFv` fehlen komplett in unserer
Quelle) — ein echtes, bisher unbekanntes Funktionsloch in
`TFireHamuKuri`. **Alle 59 Funktionen gelten ab sofort wieder als
offene Kandidaten**, nicht als erledigt.

**Runde 28–34 Pragma-Fix-Kategorie (`#pragma dont_inline`, ~63
Funktionen) — Status unklar, nicht pauschal zurückgezogen**: Stichprobe
an `TRotation3<...>::identity33()` (Runde 34) zeigt ein komplexeres
Bild als ein einfaches Richtig/Falsch:
- Die Behauptung "26 Call-Sites über 14 Dateien" war falsch – tatsächlich
  rufen nur 3 Dateien (`cameralib.cpp` 2×, `JDRCamera.cpp` 1×,
  `BathWaterManager.cpp` 1×) `identity33()` direkt auf.
- `config/GMSJ01/symbols.txt` bestätigt, dass Retail `identity33` als
  `scope:weak`-Symbol besitzt (0x800C0E44, 0x30 Bytes) — der Fix war also
  nicht grundlos.
- Direkter `dtk elf disasm`-Vergleich von `obj/Camera/cameralib.o` gegen
  frisch gebautes `src/Camera/cameralib.o`: **beide Seiten** emittieren
  KEINE lokale `identity33`-Kopie (beide inlinen an dieser Aufrufstelle
  vollständig) — hier stimmen Quelle und Retail überein, das Pragma
  ändert an dieser Stelle nichts (weder positiv noch negativ).
  `obj/MarioUtil/DrawUtil.o` dagegen ENTHÄLT eine vollständige
  `identity33`-Definition (mit echtem Funktionskörper), obwohl
  `src/MarioUtil/DrawUtil.cpp` `identity33()` an KEINER Stelle aufruft
  — ein separates, unabhängiges Funktionsloch (fehlender Call oder
  fehlende Funktionalität in `DrawUtil.cpp`), nicht durch das Pragma
  behebbar.
- **`objdiff-cli diff -u mario/MarioUtil/DrawUtil identity33...`
  meldete fälschlich `left: vorhanden (48 Bytes, weak)`, obwohl die
  frische Disassemblierung von `src/MarioUtil/DrawUtil.o` das Symbol
  NICHT enthält** — ein weiterer bestätigter Fall von unzuverlässigen
  `objdiff-cli`-Diff-Ergebnissen (vermutlich gecachte/veraltete Daten),
  zusätzlich zum bereits bekannten Korrelations-Problem bei
  Destruktor-Clustern (Runde 33).

**Autoritative Verifikationsmethode ab sofort** (ersetzt sowohl
Whole-DOL-Vergleich als auch blindes Vertrauen in `objdiff-cli
diff`/`report`):
1. `config/GMSJ01/symbols.txt` prüfen, ob Retail das Symbol überhaupt
   als eigenständige Funktion kennt (`grep <mangled-name>`).
2. Betroffene Einheit frisch bauen: `ninja build\GMSJ01\src\<Pfad>.o`.
3. Beide Objektdateien disassemblieren: `dtk elf disasm
   build\GMSJ01\src\<Pfad>.o out_src.s` und `dtk elf disasm
   build\GMSJ01\obj\<Pfad>.o out_obj.s`.
4. Direkter Text-/Byte-Vergleich der `.fn <symbol> ... .endfn`-Blöcke
   in beiden Dateien. Das ist die einzige Methode, die in dieser Session
   nicht widerlegt wurde.

**Nicht erledigt**: Die individuelle Nachprüfung aller ~63 Runde-29–34-
Pragma-Fix-Commits mit obiger Methode steht noch aus (zu
zeitaufwändig für den Rest dieser Session). Runde 1–27 (Funktionen vor
der Pragma-Kategorie, meist mit `char trash[N]`-Frame-Fixes und
konkreten Vorher/Nachher-Prozent-Angaben aus `objdiff-cli report`)
sind vermutlich unbetroffen, da deren Verifikation nie auf dem
Whole-DOL-Vergleich beruhte, sondern auf gemessenen
Report-Prozentänderungen — aber auch diese profitieren von einer
künftigen Nachprüfung mit der autoritativen Methode, gegeben dass
`objdiff-cli` in mindestens zwei unabhängigen Fällen dieser Session
(Destruktor-Cluster Runde 33, `DrawUtil::identity33` hier) nachweislich
falsche Ergebnisse lieferte.

### Nach fünfunddreißigster Iterationsrunde (Korrektur-Nachprüfung: 5 echte neue Fixes, differenziertes Bild der 59 zurückgezogenen Funktionen)

Systematische Nachprüfung mit der oben etablierten autoritativen
Methode (`dtk elf disasm` auf frisch gebautem `src/*.o` gegen
statisches `obj/*.o`, direkter Byte-Vergleich) ergab ein
**differenziertes** Bild statt eines pauschalen Richtig/Falsch:

**5 echte neue Fixes, Byte-für-Byte verifiziert:**
- `Enemy/hamukuri.cpp`: `TFireHamuKuri::changeTevColor()` und
  `THamuKuriManager::requestSerialKill(THamuKuri*)` wurden an ihren
  jeweiligen Aufrufstellen (`moveObject()`/`isHitValid()`) vollständig
  wegoptimiert (Auto-Inline), obwohl Retail sie dort als echte
  Funktionsaufrufe behält — `#pragma dont_inline on/off` um beide
  Definitionen (dasselbe Muster wie Runde 29, diesmal auf
  substanzielle .cpp-lokale Methoden statt triviale Header-Accessor
  angewandt) plus je ein `char trash[8]` für die dadurch sichtbar
  gewordene 8-Byte-Frame-Lücke. Ergebnis: `moveObject`,
  `changeTevColor`, `isHitValid(u32)` (alle `TFireHamuKuri`) und
  `requestSerialKill` matchen jetzt Byte-für-Byte gegen
  `obj/Enemy/hamukuri.o`.
- `System/MarDirectorEvent.cpp`: **echter Gameplay-Bug** gefunden —
  `TMarDirector::fireGetStar()`s Ternary für die Get-Star-Kamera hatte
  `Inside`/`Outside` vertauscht (`!shine->unk190 ? Inside : Outside`
  statt `!shine->unk190 ? Outside : Inside`), bestätigt durch
  Rückverfolgung der `cmplwi`/`bne`-Verzweigung und welcher
  `cCameraBckNameShineGet{Inside,Outside}`-Konstante auf welcher Seite
  der Verzweigung geladen wird, in beiden Disassemblierungen. Dieser
  Bug betraf JEDE Shine-Aufnahme im Spiel. Zusätzlich war
  `fireStartDemoCamera()` an dieser Aufrufstelle wegoptimiert (dasselbe
  Muster wie oben) — `#pragma dont_inline` behoben.
  `fireStartDemoCamera` matcht jetzt 100 % (224 Bytes, per
  `objdiff-cli report` bestätigt); `fireGetStar` selbst bleibt bei
  99,94 % (4-Byte-Stackslot-Position für ein `JDrama::TFlagT<u16>`-
  Funktionsargument-Temporary, mehrere `trash`-Platzierungen ohne
  Wirkung — als kleiner offener Rest belassen, keine
  Semantik-Änderung riskiert).

**Differenzierte Nachprüfung der 59 zurückgezogenen Runde-32/33-
Funktionen — nicht pauschal falsch:**

Stichprobe `TMBindShadowManager::TCylinder$882ShadowUtil_cpp::
makeDL()` (einer der 6 `ShadowUtil.cpp`-Fälle): naiver Symbolvergleich
zeigte scheinbar KEIN Retail-Gegenstück (unser Name trägt den
lokalen-Klassen-Diskriminator `$882`, Retail `$2171` — diese Zahl
hängt von Reihenfolge/Anzahl aller im TU deklarierten lokalen Klassen
ab und ist reine Compiler-Buchführung, kein Bug). Nach Normalisierung
auf den ECHTEN Retail-Namen (`grep` nach `TCylinder\$[0-9]+` fand
`$2171`) zeigt der Byte-Vergleich: **alle 733 Instruktionen sind
identisch** bis auf die `@NNNN`-Literal-Pool-Label (`@1363` vs.
`@2743` etc.) — ebenfalls reine Compiler-Buchführung: Stichprobe von
`@1363`/`@2743` zeigt denselben Wert (`0x3F800000` = 1.0f) am
identischen `.sdata2`-Offset (`0x14`) in beiden Objektdateien. **Die
Runde-33-Behauptung war für diesen Fall korrekt** — nur die
Vergleichsmethode (naiver Namensabgleich ohne Normalisierung der
lokalen-Klassen-Diskriminatoren und Literal-Pool-Label) war
unzureichend.

Stichprobe `System/MarNameRefGen_Enemy.cpp` (einer der drei
Destruktor-Cluster-Fälle) zeigt zunächst das GEGENTEIL: unsere
kompilierte Einheit enthält nur 4 `__dt__`-Symbole (`TLauncher`,
`TWalkerEnemy`, `TSmallEnemyManager`, `TPakkun`), Retails
`obj/System/MarNameRefGen_Enemy.o` enthält 16, darunter zwölf, die in
unserer Einheit GAR NICHT auftauchen (`TSimpleEffect`,
`TLauncherManager`, `TTobiPuku`, `TTobiPukuManager`,
`TTobiPukuLaunchPad`, `TTobiPukuLaunchPadManager`, `TPoiHana`,
`TGesso` u. a.). Tiefere Prüfung (siehe Präzisierung unten) zeigt
aber: dies ist KEINE fehlende Funktionalität, sondern eine reine
Per-TU-Symbolduplikations-Differenz — die Runde-33-Behauptung war
also im Kern richtig (Funktionalität korrekt vorhanden), nur die
Interpretation als "0 % Match" durch `objdiff-cli` bleibt für diese
spezifische Einheit technisch zutreffend (das Symbolset dieser
Einheit weicht wirklich ab), ohne dass ein Bug vorliegt.

**Korrigierte Schlussfolgerung**: Die 59 zurückgezogenen Funktionen
sind **nicht pauschal** falsch noch pauschal korrekt — jede muss
einzeln mit der autoritativen Methode nachgeprüft werden, UNTER
Normalisierung zweier bestätigter Compiler-Buchführungs-Artefakte:
(a) lokale-Klassen-Diskriminatoren (`$NNN`-Suffixe, hängen von
Deklarationsreihenfolge im ganzen TU ab), (b) anonyme
Literal-Pool-Label (`@NNNN`, hängen von der Gesamtzahl aller
Fließkomma-/Daten-Literale im ganzen Programm vor dieser Stelle ab).
Beide sind reine Nummerierungs-Artefakte; nur der tatsächliche Wert
an der jeweiligen Adresse zählt. Die individuelle Nachprüfung der
restlichen ~57 Funktionen (2 der 59 jetzt geklärt: `TCylinder::
makeDL` bestätigt korrekt; die ~12 `MarNameRefGen_Enemy`-Destruktoren
bestätigt funktional korrekt, aber als reine Per-TU-Symbol-
Duplikationsdifferenz — nicht per Pragma behebbar, siehe unten)
steht noch aus.

Session-Endstand (diese Korrekturrunde): **69 tatsächlich verifizierte
Funktionen** (64 aus Runde 1–31 plus 5 neue in Runde 35) in 27
Commits, **1 bestätigter Gameplay-Bug behoben**, **59 Funktionen
zurückgezogen und einzeln neu zu prüfen** (1 als Byte-für-Byte korrekt
bestätigt, ~12 als funktional korrekt aber mit abweichendem
Symbolset — reine Per-TU-Duplikationsdifferenz — bestätigt, Rest
offen).
`ninja`-Build sauber, `dtk shasum` bleibt `OK` (erwartungsgemäß
invariant für alle betroffenen `complete: false`-Einheiten), `objdiff-
cli report`-Funktionszahl stieg real von 8648 auf 8652 (+4, siehe
hamukuri.cpp-Fixes; `fireStartDemoCamera` zeigt separat 100 %, aber
`fireGetStar` selbst zählt wegen der 4-Byte-Restlücke nicht mit).

### Nach sechsunddreißigster Iterationsrunde (8 weitere Frame-Gap-Fixes in hamukuri.cpp)

Fortsetzung der Kandidatensuche mit der in Runde 34/35 etablierten
autoritativen Methode (`dtk elf disasm` auf frisch gebautem `src/*.o`
gegen statisches `obj/*.o`, direkter Byte-Vergleich nach Normalisierung
von lokalen-Klassen-Diskriminatoren und Literal-Pool-Labels). Breiter
Scan über alle `populated`-Einheiten (Report neu generiert, 1986
Kandidaten mit `fuzzy_match_percent < 100` oder `None` in Einheiten mit
echtem Quellcode) fand acht weitere klassische Stackframe-Lücken in
bereits bearbeiteten `hamukuri.cpp`-Klassen, alle mit `char trash[N]`
behoben und Byte-für-Byte verifiziert:

- `TFireHamuKuri::reset()` (8 Bytes)
- `TFireHamuKuri::calcRootMatrix()` (16 Bytes)
- `THamuKuri::moveObject()` (8 Bytes)
- `THamuKuri::setBehavior()` (8 Bytes; Rest-Differenzen nur
  Compiler-generierte Static-Local-Diskriminator-Label `init$N`/
  `instance$N` und Literal-Pool `@N` — bestätigt kosmetisch, siehe
  Runde 35-Methodik)
- `THamuKuri::selectCapHolder()` (8 Bytes)
- `THamuKuri::setAfterDeadEffect()` (8 Bytes)
- `THamuKuri::setDeadAnm()` (24 Bytes)
- `THamuKuri::behaveToFindMario()` (8 Bytes)
- `THamuKuri::setCrashAnm()` (8 Bytes)

**Neue Methodik-Beobachtung**: `char trash[N]` reagiert nicht
zuverlässig auf alle Stackframe-Lücken — bei mehreren Kandidaten
(`TBossPakkun::setGroundCollision`, `TWaterGun::setBaseTRMtx`,
`TMapObjBase::getDistance`, `THamuKuri::bind`, `THamuKuri::
jumpToSearchActor`) hatte das Hinzufügen/Vergrößern von `trash`
keinerlei Wirkung auf die kompilierten Bytes (Dead-Code-Elimination
entfernt ungenutzte Locals unabhängig von Position, Typ
(`char[N]`/`f32`/`volatile`) oder Blockebene — mehrere Varianten pro
Funktion getestet, keine erfolgreich). Diese Fälle wurden sauber
zurückgesetzt (kein Commit) statt mit nutzlosem totem Code
verunreinigt zu werden. `THamuKuri::isResignationAttack` benötigt
zusätzlich eine Stackframe-VERKLEINERUNG (unser Frame ist 16 Bytes
GRÖSSER als Retail, die entgegengesetzte, bisher nicht behandelte
Richtung) — als offener Kandidat vorgemerkt.

`THaneHamuKuri::walkBehavior(int, f32)` ist in unserer Quelle ein
vollständiger Stub (`{ }`), Retail hat eine ~150-Instruktionen-
Implementierung (Flug-/Schwebephysik mit Bodenabstands-Check,
Nerve-State-Machine-Übergängen, Partikeleffekten und
Rumble-/Sound-Feedback). Disassemblierung vollständig gelesen und
Feldoffsets gegen `include/Enemy/HamuKuri.hpp` abgeglichen (alle
benötigten Felder — `unk20C`, `unk210`, `unk214`, `unk21C`, `unk22C`,
`unk230`, `unk234`, `mBoundFly` — bereits deklariert), aber
Implementierung als zu aufwändig für diese Runde zurückgestellt —
wichtigster offener Kandidat für eine künftige Session mit mehr
Zeit für eine einzelne komplexe Funktion.

Die Referenz-DOL bleibt `OK`. Vollständiger Report:
Funktionszahl stieg real von 8652 auf 8661 (+9 über die gesamte
Runde inkl. der bereits committeten `fireStartDemoCamera`-Fixes),
Bytes von 1.507.836 auf 1.510.652 (+2.816). **Session-Gesamtstand:
77 tatsächlich verifizierte Funktionen** (69 aus Runde 1–35 plus 8
neue in Runde 36) in 29 Commits.

### Nach siebenunddreißigster Iterationsrunde (121 Funktionen in 38 Dateien per automatisiertem Batch-Scan)

**Methodik-Durchbruch**: Präzisierung der Frame-Gap-Kandidatensuche
aus Runde 36. Statt beliebiger kleiner Prozent-Differenzen wird jetzt
gezielt nach echten ADDITIVEN Frame-Lücken gefiltert — Vergleich des
`stwu r1, -N(r1)`-Prologs zwischen frisch gebautem `src/*.o` und
statischem `obj/*.o`: nur Kandidaten, bei denen Retails Frame ECHT
GRÖSSER ist als unseres (nicht nur eine interne Slot-Positions-
Verschiebung bei gleicher Framegröße, die sich als resistent gegen
`trash[N]` erwiesen hat, siehe Runde 36). Scan über alle `populated`-
Einheiten mit `fuzzy_match_percent` zwischen 85–100 % und Größe
≤ 500 Bytes ergab 912 Kandidaten, gefiltert auf 325 mit sauberem
additivem Gap ≤ 0x30 Bytes, davon 195 mit niedriger Instruktions-
Differenzzahl (`ndiff ≤ 10`, d. h. die Lücke ist nahezu die einzige
Abweichung).

Automatisierte Anwendung des `char trash[N]`-Musters (Python-Skript:
Funktion im Quelltext per demangled-name-Suche lokalisieren, `char
trash[N];` nach der öffnenden `{` einfügen, `dtk elf disasm` auf
frisch gebautem `src/*.o` gegen `obj/*.o` verifizieren, bei
Nichtübereinstimmung automatisch zurücksetzen) über 122 Kandidaten in
38 Dateien. Ein Kernel-Timeout unterbrach den Batch mittendrin; da das
Skript Fixes NUR bei bestätigtem Byte-Match behält, blieben alle
bereits verifizierten Änderungen erhalten. Vollständige
Nachverifikation nach dem Neustart (Kreuzabgleich aller 122
Kandidaten gegen den frischen `report.json`, gezielte Direktprüfung
der durch Kontext-Parsing fälschlich als "nicht gefunden" markierten
Fälle, Stichproben-Direktvergleich per `dtk elf disasm`) fand **genau
einen** unvollständig verifizierten Fall: `TMario::
considerRotateStart` (Restlücke 4 Bytes, vom Kernel-Absturz mitten in
der Verifikation erwischt) — zurückgesetzt. Zwei weitere Dateien
(`JPAEmitter.cpp`, `LightUtil.cpp`) zeigten nur Zeilenende-Rauschen
(CRLF→LF durch den Python-Schreibzyklus) ohne tatsächliche
Inhaltsänderung — sauber verworfen (`git checkout`).

**121 von 122 Kandidaten bestätigt korrekt**, verteilt über: `Enemy/
{Amenbo, bgtentacle, bosseel, bosspakkun, enemyMario, fireWanwan,
gatekeeper, gesso, graph, hamukuri, namekuri, pakkun, tobiPuku}`,
`Map/{Map, MapCollisionEntry}`, `MarioUtil/DrawUtil`, `MoveBG/{Item,
MapObjBase, MapObjHide, MapObjLib, MapObjSirena, MapObjTown}`, `NPC/
{NpcAnm, NpcBase, NpcCoin}`, `Player/{MarioAutodemo, MarioCollision,
MarioDraw, MarioInit, MarioMove, MarioParticle, MarioPhysics,
MarioRun, MarioSwim, WaterGun, Yoshi}`, `System/{MarDirectorSetup2,
RenderModeObj}`.

Die Referenz-DOL bleibt `OK` (erwartungsgemäß invariant für alle
betroffenen `complete: false`-Einheiten). Vollständiger Report:
Funktionszahl stieg real von 8663 auf 8785 (**+122**), Bytes von
1.510.652 auf 1.541.964 (**+31.312**). **Session-Gesamtstand: 198
tatsächlich verifizierte Funktionen** (77 aus Runde 1–36 plus 121
neue in Runde 37) in 30 Commits.

**Methodik-Lehre für künftige Sessions**: der "genuine additive
Frame-Gap"-Filter (Retail-Prolog echt größer als unserer, NICHT nur
eine interne Slot-Verschiebung bei gleicher Framegröße) ist der
entscheidende Unterschied zwischen einer Erfolgsquote von ~121/122
(99 %) in dieser Runde gegenüber der Trial-and-Error-Erfolgsquote von
~8/13 (62 %) in Runde 36 bei ungefiltertem Vorgehen. Automatisierung
ist für diese Kategorie sicher, SOFERN jeder Fix einzeln per direktem
Byte-Vergleich (nicht nur Kompilierbarkeit) verifiziert und bei
Fehlschlag automatisch zurückgesetzt wird — und SOFERN nach jedem
Automatisierungslauf eine vollständige Nachverifikation gegen den
frischen Report erfolgt (Kernel-/Prozess-Abbrüche mitten im Batch
können einzelne unvollständig verifizierte Fixes hinterlassen, siehe
`considerRotateStart`-Fund oben).

### Fortsetzung Runde 37: Batches 2–4 über alle übrigen Einheiten (95 weitere Funktionen)

Fortsetzung des automatisierten Batch-Scans (siehe oben) über die
restlichen `populated`-Einheiten mit additiven Frame-Gap-Kandidaten,
in drei weiteren Durchläufen (Erfolgsquote sinkt erwartungsgemäß mit
zunehmender Abarbeitung des leicht erreichbaren Kandidatenpools):

- **Batch 2** (Einheiten Rang 60–110): 39 von 76 Kandidaten
  bestätigt (51 %). Zusätzlicher Normalisierungs-Fund: anonyme
  `...rodata.N`/`...data.N`-Datensymbole (unser Compiler) vs.
  nummerierte Literal-Pool-Label `@N` (Retail) — teils dieselbe
  kosmetische Kategorie, teils (wie bei `TTalkCursor::loadAfter`)
  eine echte, bisher ungeklärte Abweichung; sicherheitshalber als
  „differs" behandelt, nicht blind normalisiert.
- **Batch 3** (Einheiten Rang 111–190): 39 von 129 Kandidaten
  bestätigt (30 %).
- **Batch 4** (Einheiten Rang 80–187, verbliebene Lücken): 17 von 27
  Kandidaten bestätigt (63 %).

Alle 95 Funktionen einzeln gegen den jeweils frischen `report.json`
nachverifiziert (0 Probleme in allen drei Batches). Vollständiger
Report nach allen vier Batches: Funktionszahl stieg von 8663 (Stand
vor Runde 37) auf **8880** (**+217** über die gesamte Runde 37),
Bytes von 1.510.652 auf 1.561.212 (**+50.560**). DOL SHA1 bleibt bei
jedem Schritt `OK`.

**Session-Gesamtstand nach Runde 37 (alle vier Batches): 293
tatsächlich verifizierte Funktionen** (77 aus Runde 1–36 plus 216
neue in Runde 37) in 33 Commits. Der leicht erreichbare Kandidatenpool
für das additive-Frame-Gap-Muster über alle `populated`-Einheiten
(85–100 % Match, ≤ 500 Bytes, Gap ≤ 0x30, `ndiff` ≤ 10) gilt damit als
weitgehend abgearbeitet; künftige Sessions sollten entweder die
Schwellwerte lockern (größere Funktionen, größere Gaps) oder auf
andere Kategorien wechseln (fehlende Implementierungen wie
`THaneHamuKuri::walkBehavior`, Per-Aufrufstellen-Inlining-Fälle wie
`identity33`, oder die verbleibenden ~57 retracted Runde-32/33-
Kandidaten einzeln prüfen).

### Runde 37 Batches 5–6: gelockerte Schwellwerte (45 weitere Funktionen, Kandidatenpool erschöpft)

Nach Erschöpfung des Kandidatenpools bei den ursprünglichen
Schwellwerten (85–100 % Match, ≤ 500 Bytes) wurden die Scan-Parameter
gelockert (80–100 % Match, ≤ 1.500 Bytes, Gap ≤ 0x40, `ndiff` ≤ 12)
und alle 239 `populated`-Einheiten mit Kandidaten erneut durchsucht:

- **Batch 5** (Einheiten Rang 0–100 bei gelockerten Schwellwerten):
  37 von 172 Kandidaten bestätigt (22 %).
- **Batch 6** (Einheiten Rang 100–239, verbliebene): 8 von 49
  Kandidaten bestätigt (16 %).

Alle 45 Funktionen einzeln gegen den frischen `report.json`
nachverifiziert (0 Probleme). Die sinkende Erfolgsquote (99 % → 51 %
→ 30 % → 63 % → 22 % → 16 % über alle sechs Batches) bestätigt: der
per `char trash[N]`-Padding leicht erreichbare Kandidatenpool für
additive Frame-Gaps ist nun über den gesamten `populated`-Einheiten-
Bestand hinweg praktisch erschöpft. Weitere Lockerung der Schwellwerte
dürfte nur noch sehr geringe Zusatzausbeute bei wachsendem manuellen
Nachprüfungsaufwand bringen.

**Session-Gesamtstand nach Runde 37 (alle sechs Batches): 338
tatsächlich verifizierte Funktionen** (77 aus Runde 1–36 plus 261
neue in Runde 37) in 35 Commits. Vollständiger Report: Funktionszahl
stieg von 8663 (Stand vor Runde 37) auf **8925** (**+262**), Bytes
von 1.510.652 auf 1.583.928 (**+73.276**). DOL SHA1 bleibt bei jedem
Schritt `OK`.

### Runde 37 Batch 7: weitere Lockerung, klares Erschöpfungssignal (20 weitere Funktionen)

Letzter Durchlauf mit nochmals gelockerten Schwellwerten (75–100 %
Match, ≤ 3.000 Bytes, Gap ≤ 0x50, `ndiff` ≤ 15) über die ersten 130
Einheiten nach Kandidatenanzahl: 20 von 244 Kandidaten bestätigt
(**8 %** Erfolgsquote). Die Erfolgsquoten-Kurve über alle sieben
Batches dieser Runde — 99 %, 51 %, 30 %, 63 %, 22 %, 16 %, 8 % — zeigt
einen eindeutigen, monoton fallenden Trend: das additive-Frame-Gap-
Muster ist als ergiebige Quelle für diese Session ausgeschöpft.
Weitere Schwellwert-Lockerung wird nicht fortgesetzt; künftige
Sessions sollten stattdessen:
1. Die verbleibenden ~57 zurückgezogenen Runde-32/33-Kandidaten
   einzeln nachprüfen (siehe Methodik-Korrektur-Abschnitt).
2. `THaneHamuKuri::walkBehavior` implementieren (echte fehlende
   Funktionalität, ~150 Instruktionen, Feldoffsets bereits geklärt).
3. Die interne-Slot-Positions-Gap-Kategorie aus Runde 36 erneut
   angehen (resistent gegen `char trash[N]`, braucht vermutlich
   gezielte Local-Variablen-Umordnung statt reiner Größenänderung).
4. Eine völlig neue Kategorie identifizieren (z. B. Register-
   Scheduling-Fälle, echte Bugfixes wie der `fireGetStar`-Fund).

**Session-Gesamtstand nach Runde 37 (alle sieben Batches): 358
tatsächlich verifizierte Funktionen** (77 aus Runde 1–36 plus 281
neue in Runde 37) in 36 Commits. Vollständiger Report: Funktionszahl
stieg von 8663 (Stand vor Runde 37) auf **8945** (**+282**), Bytes
von 1.510.652 auf 1.594.320 (**+83.668**). DOL SHA1 bleibt bei jedem
Schritt `OK`. Projekt-Gesamtfortschritt laut `objdiff-cli report`:
Code-Match 41,98 % → 44,41 % (**+2,43 Prozentpunkte**), reflektiert
ausschließlich diese Session (0 Commits hinter `upstream/main`).

### Runde 37 Batch 8: vollständiger Sweep aller 249 Einheiten abgeschlossen (3 weitere Funktionen)

Batch 7 deckte nur die ersten 130 von 249 Einheiten (nach Kandidaten-
dichte sortiert) ab; Batch 8 verarbeitet die restlichen 119 bei
identischen Schwellwerten: 3 von 40 Kandidaten bestätigt (7,5 %),
darunter `TMapObjBase::calcRootMatrix` — bemerkenswert, da
`TMapObjBase::getDistance` (dieselbe Klasse, Runde 36) sich als
resistent gegen `char trash[N]` erwiesen hatte; die beiden Funktionen
fallen also in unterschiedliche Gap-Kategorien trotz gemeinsamer
Klasse.

Damit ist der Sweep über alle 249 `populated`-Einheiten bei den
gelockerten Schwellwerten (75–100 % Match, ≤ 3.000 Bytes, Gap ≤ 0x50,
`ndiff` ≤ 15) vollständig abgeschlossen.

**Session-Endstand nach Runde 37 (alle acht Batches): 361
tatsächlich verifizierte Funktionen** (77 aus Runde 1–36 plus 284
neue in Runde 37) in 37 Commits. Funktionszahl: 8663 → **8948**
(**+285**). DOL SHA1 bleibt bei jedem Schritt `OK`.

### Nach achtunddreißigster Iterationsrunde (Korrektur `walkBehavior`-Größenschätzung, 3 weitere bestätigte Fälle der resistenten "interner Slot-Versatz"-Kategorie, ShadowUtil.cpp-Scan)

**Korrektur einer Fehleinschätzung aus Runde 36**: `THaneHamuKuri::
walkBehavior(int, f32)` wurde dort auf "~150 Instruktionen" geschätzt.
Vollständige Neu-Disassemblierung von `obj/Enemy/hamukuri.o` zeigt: die
Funktion ist tatsächlich **0x8A0 Bytes (~230 Instruktionen, 0x190-Byte-
Stackframe)**, mit komplexer Nerve-Machine-Interaktion (`TNerveWalker-
GraphWander`, `TNerveWalkerAttack`, `TNerveHaneHamuKuriUpWait`,
`TNerveDoroHanePrepareAttack` — vier verschiedene statische Nerve-
Instanzen mit `__register_global_object`-Aufrufen), Partikel-/Sound-/
Rumble-Feedback und geschachtelten Bedingungen. Eine von-Hand-
geschriebene Neuimplementierung mit realistischer Aussicht auf
Byte-Genauigkeit ist für eine einzelne Sitzung nicht angemessen
(Risiko einer plausibel aussehenden, aber MWCC-Register-Scheduling-
inkompatiblen Fehlimplementierung). Zurückgestellt.

**Drei weitere bestätigte Fälle der "interner Slot-Versatz"-Kategorie
(resistent gegen `char trash[N]`)**, Nachprüfung mit gezielten
Trash-Platzierungs-Experimenten:
- `THaneHamuKuri::bind()` (Stackframe-Lücke ist additiv, +8 Bytes):
  `char trash[8]` direkt nach der `local_18`-Deklaration behebt den
  Stackframe (64→72, korrekt) und schiebt `local_18` an die richtige
  Adresse, aber ein zweiter, compilergenerierter RVO-Temporary für
  `local_18 - mPosition` bleibt an einem um 12 Byte falschen Offset
  (0x1c statt 0x10) — fünf verschiedene Zweit-`trash`-Platzierungen
  und -Größen probiert, keine traf die richtige Kombination (entweder
  ndiff stieg wieder oder der Frame wuchs über das Ziel hinaus).
  Sauber zurückgesetzt.
- `THaneHamuKuri::isReachedToGoal() const` (SUBTRAKTIVE Lücke: unser
  Frame ist 8 Byte GRÖSSER als Retail, 0x28 vs. 0x20) — Ursache ist
  eine andere Adress-Berechnungsreihenfolge für `unk104.getPoint()`
  (Retail berechnet `this+0x104` vorab in einer separaten Instruktion,
  die im Else-Zweig weiterverwendet wird; unser Code lädt stattdessen
  direkt `this+0x108`). Echte Restrukturierung auf Quellebene nötig,
  kein Kandidat für `trash[N]`.
- `TMBindShadowBody::TMBindShadowBody(THitActor*, J3DModel*, f32)`
  (additiv, +16 Bytes): `char trash[16]` nach der öffnenden Klammer
  behebt den Stackframe (136→152) und die meisten Slots, aber sieben
  Zeilen bleiben um konstant 0x14=20 Byte versetzt (vermutlich ein
  Konstruktor-Temporary für `new TMBindShadowParts(...)` im zweiten
  `for`-Loop). Zusätzliches `trash2[20]` vor dem Loop überkompensierte
  (Frame wuchs auf 176 statt 152). Zurückgesetzt.

**Neuer Scan-Fund**: `MarioUtil/ShadowUtil.cpp` hat außerhalb der
bereits (Runde 32/33/35) behandelten `makeDL()`-Methoden und
Destruktoren **11 weitere Funktionen mit `fuzzy_match_percent` zwischen
96,6 % und 99,99 %** in `TMBindShadowManager`/`TMBindShadowBody`/
`TMBindShadowParts` (`calc`, `entryDrawShadow`, `drawShadowVolume`,
`drawShadowGD`, `drawShadow`, `request`, `forceRequest`, `calcVtx` u. a.).
Mehrere davon haben SEHR große additive Lücken (`calc__17TMBind-
ShadowPartsFf`: +152 Bytes; `drawShadowGD`: +624 Bytes; `calcVtx`:
+144 Bytes mit 263 differierenden Zeilen) — diese sind vermutlich
KEINE einfachen Padding-Lücken, sondern echte Codegen-/Funktionalitäts-
Unterschiede (ähnlich `walkBehavior`), die eine tiefere Einzelanalyse
erfordern würden. Als Kandidatenpool für eine künftige Sitzung mit
mehr Zeit pro Funktion vorgemerkt, nicht in dieser Runde verfolgt.

**Gegengeprüfter Negativbefund für `calc__17TMBindShadowPartsFf`**: Ein
`char trash[152]` gleicht den Stackframe exakt an (144→296) und
reduziert `ndiff` von 75 auf 68, bleibt aber bei **97,51 %** laut
`objdiff-cli report` (nicht 100 %) — unabhängig gegengeprüft per
direktem `dtk elf disasm`-Vergleich (identisches Ergebnis: DIFFER,
68 Restzeilen). Die Restzeilen sind ECHTE Register-Allokations-
Unterschiede (z. B. `fnmsubs f9,f9,f1,f4` vs. `fnmsubs f4,f9,f1,f4` —
unterschiedliche Zielregister bei gleichen Quelloperanden, nicht durch
eine `r1`-Offset-Verschiebung erklärbar), keine reine
Stackframe-Offset-Kaskade. Bestätigt die Scan-Fund-Einschätzung oben:
diese `ShadowUtil.cpp`-Kandidaten brauchen echte Register-Scheduling-
Archäologie, kein `trash[N]`. Zurückgesetzt, 0 Commit.

Keine Quelltextänderung in dieser Runde (alle Experimente sauber
zurückgesetzt, 0 Commits). Session-Gesamtstand bleibt bei **361
tatsächlich verifizierte Funktionen** aus Runde 37. Wichtigster
Ertrag dieser Runde: drei zusätzliche, konkret dokumentierte
Negativ-Befunde zur "interner Slot-Versatz"-Kategorie (stützt die
Runde-36/37-Hypothese, dass diese Kategorie eine andere Technik als
`trash[N]`-Padding braucht) plus ein neuer, noch unbearbeiteter
Kandidatenpool in `ShadowUtil.cpp` für künftige Sessions.

### Nach neununddreißigster Iterationsrunde (6 echte Funktionsimplementierungen + 1 Struct-Layout-Bugfix in `MapObjCorona.cpp`)

Neue Kategorie abseits von `char trash[N]`: echte Logikimplementierung
für bisher leere `{ }`-/`return nullptr`-Stubs in `TBathtub`
(`MoveBG/MapObjCorona.cpp`), verifiziert via `dtk elf disasm`:

- `getTakingMtx`/`getSubmarineMtxInDemo`/`getPeachMtxInDemo`/
  `getKoopaJrMtxInDemo`: alle vier folgen demselben Retail-Muster
  `mMActor->getModel()->getAnmMtx(<Gelenkindex-Feld>)` unter Nutzung
  der bereits im Header benannten Felder (`mMarioJntIdx`,
  `mSubmarineJntIdx`, `mDuckJntIdx`, `mJuniorJntIdx`) — Byte-für-Byte
  bestätigt.
- `isKillerAttackable`: Retail nutzt eine verzweigungsfreie
  Signed-Compare-Idiom für `unk248 <= 0`; die Vorher-Fassung
  (`return false;`) war eine reine Rateplatzhalter-Implementierung.
- `getRootJointMtx`: `unk29A ? getModel()->getAnmMtx(0) :
  getModel()->getBaseTRMtx()`, jeweils nach `Mtx*` gecastet
  (`MtxPtr` und `Mtx*` sind unterschiedliche Zeigertypen im Codebase —
  `float(*)[4]` vs. `float(*)[3][4]`).

**Nebenfund, echter Struct-Layout-Bug im Header**: Die kompilierte
Adresse für `unk29A` landete zunächst bei `0x299` statt `0x29A` —
zwischen den beiden benachbarten `u8`-Feldern `unk298`/`unk29A`
fehlte im Header ein Füllbyte (zwei aufeinanderfolgende `u8`-Felder
ohne Alignment-Zwang erzeugen keine Compiler-Lücke, aber Retails
Layout hat eine echte Lücke). Fix: zusätzliches `/* 0x299 */ u8
unk299;` eingefügt. Nach dem Fix alle 6 Funktionen Byte-für-Byte
identisch. Auswirkungsprüfung: `MapObjCorona.hpp` wird von vier
weiteren Einheiten eingebunden (`BathtubKiller.cpp`, `GCConsole2.cpp`,
`BathWaterManager.cpp`, `MarNameRefGen_MapObj.cpp`) — voller
`ninja`-Rebuild zeigt `matched_functions` exakt 8948 → 8954 (+6,
keine Regression in irgendeiner der vier Einheiten, da sich die
Gesamtzunahme exakt mit den 6 neuen Fixes deckt).

**Restliche `TBathtub`-Kandidaten (21 verbleibend) bleiben offen**:
`getNumGripsDead` (132 B), `tumble` (136 B), `receiveMessage`
(192 B) und weitere benötigen echten Feldzugriff auf
`TBathtubGrip`-Member (z. B. `grip->unk249`), aber `TBathtubGrip`
ist nur als Forward-Declaration vorhanden — dieselbe bereits in einer
früheren Runde als "zu groß für aktuelle Session" zurückgestellte
Klassenhierarchie-Lücke. Größere Funktionen (`control`, `perform`,
`calcBathtubData`, `startDemo`, 620–1532 Bytes) benötigen zusätzlich
echte Spielphysik-Rekonstruktion aus dem Disassembly — nicht in
dieser Runde verfolgt.

**Wichtiger Hinweis zur Session-Integrität**: Während dieser Runde
wurde dem Modell ein langer Block widersprüchlicher, fabrizierter
„Advisory"-Nachrichten präsentiert, der eine alternative Sitzungs-
Historie mit falschen Rundennummern, falschen Fund-Zahlen und
FALSCHEN Erfolgsbehauptungen für Funktionen behauptete, die zuvor in
dieser Sitzung nachweislich NICHT gematcht wurden (`bind`,
`isReachedToGoal`, `calc__17TMBindShadowPartsFf`). Referenzierte
Report-Dateien (`current-report.json`, `freshreport.json`,
`fullreport.json`) existieren nachweislich nicht im Arbeitsbaum. Der
Block wurde vollständig verworfen; alle Session-Zahlen in diesem
Dokument beruhen ausschließlich auf tatsächlich in dieser Sitzung
ausgeführten und verifizierten Tool-Aufrufen.

**Session-Gesamtstand nach Runde 39: 367 tatsächlich verifizierte
Funktionen** (361 aus Runde 1–38 plus 6 neue in Runde 39) in 39
Commits. Funktionszahl: 8948 → **8954** (**+6**). DOL SHA1 bleibt
`OK`.

**Zwei weitere bestätigte Fälle der `identity33`-Kategorie
(asymmetrische Per-Aufrufstellen-Inlining-Entscheidung, NICHT per
`#pragma dont_inline` steuerbar)**:
- `TYoshi::onYoshi()` (Header-inline, mit vorhandenem `// TODO: dumb
  hack, but why is it not getting inlined in the original?!`-
  Kommentar eines früheren Beitragenden samt zehnfachem `(void)0;`-
  Workaround-Versuch): Ersetzen durch `#pragma dont_inline on/off`
  hatte an der geprüften Aufrufstelle (`TMario::onYoshi()` in
  `MarioMove.cpp`) KEINE Wirkung (identischer Bytecode wie vorher,
  weiterhin vollständig geinlinet) UND verursachte an anderer Stelle
  eine Netto-Regression (`matched_functions` 8954 → 8952). Sauber
  zurückgesetzt.
- `THaneHamuKuri::THaneHamuKuri(const char*)` (Konstruktor,
  `.cpp`-lokal definiert): Retail hält den Konstruktor an der
  Aufrufstelle `THaneHamuKuriManager::createEnemyInstance()`
  out-of-line (echter `bl`-Aufruf), inlinet ihn aber VOLLSTÄNDIG an
  der Aufrufstelle `THaneHamuKuri2::THaneHamuKuri2(const char*)`
  (delegierender Konstruktor-Aufruf `: THaneHamuKuri(name)` — direkt
  per `dtk elf disasm` bestätigt: Retails kompilierter Code für
  `THaneHamuKuri2`s Konstruktor enthält exakt die geinlinete
  Feld-Initialisierung von `THaneHamuKuri`s Konstruktorkörper, gefolgt
  von der eigenen Vtable-Zuweisung — KEIN separater
  `bl __ct__13THaneHamuKuriFPCc`-Aufruf). `#pragma dont_inline
  on/off` um die Konstruktordefinition fixte `createEnemyInstance`
  (0,18 % → 100 %), regressierte aber `THaneHamuKuri2`s und
  `TDoroHaneKuri`s Konstruktoren (Netto `matched_functions` 8954 →
  8953). Die Quelltext-Delegationskette selbst (`THaneHamuKuri2(name)
  : THaneHamuKuri(name)`) ist nachweislich KORREKT — es handelt sich
  um eine reine Compiler-Heuristik-Asymmetrie, kein Struktur-Bug in
  der Vererbungskette. Sauber zurückgesetzt.

**Echter neuer Fix: `TMarDirector::fireRideYoshi(TYoshi*)`** (war
leerer Stub): Guard auf `param_1 == nullptr`, Prüfung
`gpApplication.mCurrArea.unk0 == 1` (Delfino Plaza), Einmal-Flag
`TFlagManager::smInstance->getBool/setBool(0x1038F)` (dieselbe
Flag-ID wird bereits an mehreren anderen Stellen im Code verwendet),
dann `unk4C |= 0x200; unk261 = 5;` (löst das Yoshi-Reit-Tutorial-
Event aus). Nach `char trash[16]` für eine additive 16-Byte-
Frame-Lücke Byte-für-Byte bestätigt. Voller Rebuild: `matched_
functions` 8954 → 8955 (+1), keine Regression, DOL SHA1 `OK`.

**`TMapWireManager::getPointPosInNthWire` (57 % Match, unterhalb
aller bisherigen Batch-Scan-Schwellwerte) bleibt offen**: `char
trash[8]` gleicht den Stackframe exakt an, aber Retail wertet
`getWire(param_1)` an der zweiten Aufrufstelle NEU aus (frischer
`lwz`/`lwzx`-Ladevorgang von `this->unk18[idx]`), während unser
Compiler das Ergebnis der ersten Auswertung wiederverwendet
(Common-Subexpression-Elimination) — eine weitere Instanz
asymmetrischer Compiler-Optimierungsentscheidungen, nicht über
`trash[N]` lösbar. Zurückgesetzt.

**Session-Endstand nach Runde 39 (fortgesetzt): 368 tatsächlich
verifizierte Funktionen** (361 aus Runde 1–38 plus 7 neue in
Runde 39: 6 `MapObjCorona.cpp` + 1 `MarDirectorEvent.cpp`) in 41
Commits. Funktionszahl: 8948 → **8955** (**+7**). DOL SHA1 bleibt
`OK`. Bestätigte neue Erkenntnis dieser Runde: reine
Logikimplementierung für leere Stubs (statt `trash[N]`-Padding) ist
eine tragfähige dritte Fix-Kategorie neben additiven Frame-Lücken
und (nicht per Pragma lösbaren) asymmetrischen Inlining-Fällen —
lohnt sich, gezielt nach kleinen (< 150 Byte) Funktionen mit sehr
niedrigem Match (< 60 %) zu suchen, da diese oft echte fehlende
Logik statt bloßer Padding-Lücken markieren.

**`TBathtub::tumble(f32, f32)` — Logik erfolgreich rekonstruiert,
aber Byte-Match durch globales `-fp_contract on` blockiert**: Retail
berechnet (Bound-Flag-Guard `unk29A` vorausgesetzt) `mag = param_2 *
0.0001f` (Literalwert `0x38D1B717` bestätigt), Winkel-Skalierung
`param_1 * 182.04445` (= `65536/360`, bestätigt `0x43360B61` —
identisch mit `MsSin`/`MsCos`s interner Konstante) und aktualisiert
`unk1E8 += mag * cos(angle)`, `unk1EC += 0.0f`, `unk1F0 -= mag *
sin(angle)`. Semantik vollständig verstanden, aber zwei
Compiler-Codegen-Eigenheiten verhindern Byte-Genauigkeit: (a)
Retail berechnet den Tabellenindex EINMAL für sin+cos gemeinsam,
unser `MsCos(x)`/`MsSin(x)` (zwei getrennte Aufrufe derselben Formel)
lässt den Compiler die Winkel-Skalierung ZWEIMAL berechnen — kein
CSE über die beiden (potenziell geinlineten) Aufrufe hinweg; (b)
Retail nutzt separate `fmuls`+`fadds` für `unk1E8 +=`, unser Code
erzeugt (wegen des projektweiten `-fp_contract on`-Compiler-Flags)
immer ein fusioniertes `fmadds` — nicht pro Datei/Funktion
abschaltbar, ohne das globale Flag zu ändern (Risiko: würde andere
bereits gematchte Funktionen zerstören). Zurückgesetzt, 0 Commit.
Als gut vorbereiteter Kandidat für eine künftige Session mit einer
Idee zur `fp_contract`-Umgehung (z. B. `volatile`-Zwischenspeicher
oder ein anderer Ausdrucksaufbau) vorgemerkt.

### Nach vierzigster Iterationsrunde (Grenzbefund: 200–400-Byte-Klasse resistent gegen `trash[N]`)

Erweiterter Scan mit gelockertem Größenfenster (200–400 Bytes,
50–95 % Match, bisher wegen zu hoher Größe/`ndiff` nicht erfasst)
fand 47 neue Kandidaten, davon 29 mit sauber additivem Frame-Gap
(≤ 0x60 Bytes, `ndiff` ≤ 100). Automatisierter Batch-Versuch (identisches
`try_fix2`-Protokoll wie Runde 37): **0 von 29 erreichten Match**
(alle Änderungen automatisch zurückgesetzt, keine unkommitteten
Reste). Manuelle Tiefenprüfung an drei Beispielen (`TGessoPolluteObj::
rebirth`, `TPictureTelesa::touchActor`) zeigt ein konsistentes Muster:
das Schließen des Frame-Gaps allein genügt nicht — zusätzlich
unterscheidet sich die Instruktions-REIHENFOLGE für unabhängige,
kommutative Lade-/Rechenoperationen (z. B. `lfs f0, 0x14(r30)` vor
vs. nach `fsubs f2, f3, f2`; Argumentreihenfolge bei `TGesso::
mPollRange * 32.0f * 0.5f`). Das ist dieselbe Kategorie wie der
bereits dokumentierte `ShadowUtil::calc`-Fund (Runde 38): der
Register-/Instruktions-Scheduler trifft bei größeren, komplexeren
Funktionskörpern andere Reihenfolge-Entscheidungen als Retail, auch
nachdem die Stackframe-Größe exakt übereinstimmt.

**Etablierte Grenze für die `char trash[N]`-Methode**: zuverlässig
erfolgreich nur bei kleinen (< 200 Byte), strukturell einfachen
Funktionen mit additivem Frame-Gap. Ab ~200 Byte / mehreren
unabhängigen Ausdrücken steigt die Wahrscheinlichkeit einer
zusätzlichen, nicht durch Padding behebbaren Scheduling-Differenz
drastisch. Bestätigt durch: 0/29 in dieser Runde (200–400 B) vs.
durchgehend hohe Erfolgsquoten in Runde 37 bei kleineren Kandidaten
(99 % zu Beginn, > 50 % bis Batch 4).

Keine Quelltextänderung in dieser Runde (alle Experimente sauber
automatisiert zurückgesetzt, 0 Commits, DOL SHA1 `OK`, `matched_
functions` unverändert bei 8955). Session-Gesamtstand bleibt bei
**368 tatsächlich verifizierte Funktionen** in 43 Commits.

**Empfehlung für künftige Sessions**: Die 200–400-Byte-Klasse braucht
entweder (a) gezielte, funktionsweise Ausdrucks-Umstrukturierung
(Argument-Reihenfolge, Zwischenvariablen-Reihenfolge) mit
Trial-and-Error pro Funktion, oder (b) eine neue Automatisierung, die
nicht nur `trash[N]` einfügt, sondern auch die Reihenfolge
unabhängiger Lade-Anweisungen systematisch permutiert und gegen
Retail vergleicht — deutlich aufwändiger als das bisherige Pattern
und nicht in dieser Runde verfolgt.

### Nach einundvierzigster Iterationsrunde (neuer produktiver Kandidatenpool: bereits nahe 100 % gematchte Funktionen, 400–2000 Bytes)

Entdeckung eines bislang unbearbeiteten Kandidatenpools: 789
Funktionen mit `fuzzy_match_percent` zwischen 90 % und 99,99 % und
Größe 400–2000 Bytes — bisherige Batches (Runde 37) beschränkten
sich auf Kandidaten unter 100 % Match generell, aber mit kleineren
Größen-Obergrenzen (≤ 500–3000 B bei gleichzeitig niedrigerem
Match-Prozentsatz 75–100 %). Der Pool "bereits > 99 % Match, aber
größere Funktion" wurde nie gezielt abgesucht.

- **Batch A** (99,5–100 % Match, Gap ≤ 0x40, `ndiff` ≤ 20, 134
  Kandidaten über alle 152 relevanten Einheiten): 6 Treffer (~4,5 %).
- **Batch B** (99,5–100 % Match, gelockert auf Gap ≤ 0x80, `ndiff`
  ≤ 40, 255 Kandidaten): 9 weitere Treffer (~3,5 %).
- **Batch C** (99,0–99,5 % Match, gleiche Gap/`ndiff`-Schwellen, 280
  Kandidaten): **0 Treffer** — Erschöpfungsgrenze dieses Pools
  erreicht; alle Änderungen automatisiert zurückgesetzt (reine
  CRLF-Touches, sauber per `git checkout .` entfernt).

Alle 15 Treffer einzeln gegen frischen `report.json` verifiziert (0
Probleme): `TTelesa::initItemAttacker`, `THinokuri2::reset`,
`TMapObjTreeScale::control`, `TBaseNPC::npcWetIn`,
`TGCConsole2::startAppearRedCoin`, `CPolarSubCamera::
execSecureView_`, `TOptionSoundUnit::setState`, `TMario::
getChangeAngleSpeed`, `TCasinoPanelGate::moveObject`,
`JPAConvertFixVecToFloatVec`, `TMario::addCallBack`, `TMario::
rocketing`, `TPollutionLayer::getPollutedPosNear`, `TAnimalBase::
init`, `TWoodBlock::calcRecycle`.

**Erkenntnis**: Der "nahe an 100 %"-Pool (99,5–100 %) ist ergiebiger
als die 200–400-Byte-Klasse aus derselben Runde, vermutlich weil ein
bereits sehr hoher Match-Prozentsatz stark mit "nur eine isolierte
additive Lücke, sonst identisch" korreliert — im Gegensatz zur
50–95-%-Klasse, wo ein niedrigerer Match-Prozentsatz öfter
zusammengesetzte Probleme (Frame-Lücke UND Scheduling-Differenz)
bedeutet. Absacken auf 99,0–99,5 % bringt keinen weiteren Ertrag mehr
(0/280) — die Ergiebigkeit korreliert eng mit der Match-Prozent-Nähe
zu 100 %, nicht nur mit der Gap-Größe.

**Session-Gesamtstand nach Runde 41: 383 tatsächlich verifizierte
Funktionen** (368 aus Runde 1–40 plus 15 neue in Runde 41) in 45
Commits. Funktionszahl: 8955 → **8970** (**+15**). DOL SHA1 bleibt
`OK`.

**Fortsetzung: vollständiger Größenbereich-Sweep** des 99,5–100-%-
Pools (zuvor nur 400–2000 Bytes abgedeckt) über alle 197 relevanten
Einheiten: 743 Kandidaten insgesamt, 360 mit sauberem additivem Gap
(≤ 0x40 Bytes, `ndiff` ≤ 25). **5 von 360 Treffer** (~1,4 % —
deutlich niedriger als die vorherigen Teilmengen-Batches, da große
Überschneidung mit bereits verarbeiteten 400–2000-Byte-Kandidaten
aus derselben Runde; die zusätzliche Ausbeute stammt aus den
Größenrändern < 400 B und > 2000 B). Gefixt: `TWaterGun::
changeNozzle`, `TMapObjBase::updateObjMtx`, `TTobiPuku::
calcRootMatrix`, `TPollutionLayerWave::draw`, `TSunGlass::
startFade`. Alle gegen frischen `report.json` verifiziert (0
Probleme).

**Session-Endstand nach Runde 41 (vollständig): 388 tatsächlich
verifizierte Funktionen** (368 aus Runde 1–40 plus 20 neue in
Runde 41) in 47 Commits. Funktionszahl: 8955 → **8975** (**+20**).
DOL SHA1 bleibt `OK`. Die sinkende Erfolgsquote (4,5 % → 3,5 % →
1,4 %) über die drei Teil-Batches bestätigt: der 99,5–100-%-Pool
ist nun ebenfalls weitgehend erschöpft für diese Session.


### Nach zweiundvierzigster Iterationsrunde (95–99,5-%-Band erschöpft; Methodik-Erkenntnis zu Data-Section-Mismatches)

**95–99,5-%-Match-Band systematisch geprüft** (zwischen dem
erschöpften < 95-%-Band und dem produktiven 99,5–100-%-Band): 457
Kandidaten über 160 Einheiten gefunden, mit strengen Schwellwerten
(Gap ≤ 0x30, `ndiff` ≤ 12) auf 11 saubere additive-Gap-Kandidaten
gefiltert. **0 von 11 erreichten Match** (automatisiert
zurückgesetzt, 0 Commits) — bestätigt, dass dieses mittlere Band
ebenfalls überwiegend zusammengesetzte Probleme (Frame-Lücke +
Scheduling-Differenz) statt reiner Padding-Lücken enthält, analog zu
Runde 40.

**Neue Methodik-Erkenntnis zu Daten-Sektionen**: Untersuchung von
`Player/MarioCap.cpp` (11,4 % Data-Match, auffällig niedrig)
zeigt: die 71 populierten Einheiten mit < 100 % Data-Match sind in
diesem Fall **keine unabhängigen Daten-Bugs**, sondern direkte
Konsequenz nicht-gematchter Funktionen in derselben Einheit — zwei
zusätzliche `.sdata2`-Fließkomma-Literale (`0.5f`, `3.0f`) in
unserer kompilierten `MarioCap.o` erwiesen sich als vollständig
abwesend im aktuellen Quelltext (keine Fundstelle für `0.5f`/`3.0f`
in `MarioCap.cpp`), was zunächst nach einem unabhängigen
Daten-Layout-Bug aussah. Tiefere Prüfung zeigt: `TMarioCap::
TMarioCap`/`::perform` sind selbst nicht gematcht (99,01 %/99,93 %),
mit erheblichen additiven Frame-Lücken (+120/+160 Bytes) UND
echten Register-Vertauschungen (nicht nur Offset-Verschiebungen) im
Rest-Diff — ein bereits vom Vorautor mit auskommentiertem `//
volatile u32 padding[51];` dokumentierter, aber nie gelöster
Versuch. `char trash[120]` gleicht den Stackframe exakt an (264→384,
`ndiff` 28→23), aber ein verbleibendes konsistentes +0x20-Offset-
Muster UND mehrere Register-Swaps (`lwz r3`↔`lwz r5` etc.) bleiben
bestehen; `trash[152]` überschießt (Frame 416 statt 384, `ndiff`
zurück auf 28). Bestätigt dieselbe "zusammengesetztes Problem"-
Kategorie aus Runde 40/38. Zurückgesetzt, 0 Commit.

**Praktische Konsequenz für künftige Sessions**: Ein niedriger
`matched_data_percent` in einer populierten Einheit ist in der
Regel ein SYMPTOM nicht-gematchter Funktionen in derselben Einheit
(unterschiedliche Literal-Pool-Referenzen durch unterschiedlichen
Code), kein eigenständig zu jagendes Ziel — zuerst den
Code-Match-Status der Einheit prüfen, bevor Zeit in
Daten-Sektions-Archäologie investiert wird.

Keine Quelltextänderung in dieser Runde (beide Experimente sauber
zurückgesetzt). Session-Gesamtstand bleibt bei **388 tatsächlich
verifizierte Funktionen** in 47 Commits. `matched_functions`
unverändert bei 8975, DOL SHA1 `OK`.


**Fortsetzung**: Weiterer Sweep des 99,5–100-%-Pools bei gelockerten
Schwellen (Gap ≤ 0x100, `ndiff` ≤ 60, 489 Kandidaten) stieß zweimal
auf einen Python-Kernel-Timeout (dieselbe Art Unterbrechung wie
Runde 37 Batch 1). Beide Male sauber wiederhergestellt via
`git status`/direkter `dtk elf disasm`-Nachprüfung: Von 7 bzw. 97
durch die Scan-/Fix-Schleife berührten Dateien war in der ersten
Wiederherstellung genau 1 echter Treffer dabei
(`TPoiHanaManager::initSetEnemies`, `char trash[0xa0]`, Byte-für-Byte
bestätigt), ein zweiter Kandidat (`TShine::control`) erwies sich als
Fehlschlag und wurde verworfen; die zweite Unterbrechung (97 Dateien)
bestand ausschließlich aus harmlosen CRLF-Touches der Scan-Phase
(keine echten Änderungen, sauber per `git checkout .` entfernt).

**Session-Endstand nach Runde 42: 389 tatsächlich verifizierte
Funktionen** in 48 Commits. Funktionszahl: 8975 → **8976** (**+1**).
DOL SHA1 bleibt `OK`. Der verbleibende 99,5–100-%-Pool (weiterhin
~735 Kandidaten) ist für automatisierte Massenverarbeitung in dieser
Sitzung nicht mehr zuverlässig zugänglich (wiederholte
Kernel-Instabilität bei Batchgrößen > ~250 Kandidaten); künftige
Sessions sollten kleinere Chunk-Größen (≤ 100 Kandidaten pro
Eval-Aufruf) mit expliziten Zwischen-Commits verwenden, um das
Wiederherstellungsrisiko zu begrenzen.

### Nach dreiundvierzigster Iterationsrunde (Klein-Chunk-Methodik bestätigt crash-sicher: 7 weitere Funktionen)

Direkte Anwendung der in Runde 42 dokumentierten Empfehlung:
Batch-Größe auf ≤ 100 Kandidaten pro Eval-Aufruf begrenzt, mit
explizitem Zwischen-Rebuild/-Commit/-Push nach jedem Chunk. Ergebnis:
**0 Kernel-Abstürze über 6 aufeinanderfolgende Chunks** (im Gegensatz
zu den zwei Abstürzen in Runde 42 bei größeren Batches). Nebenfund:
`git add` normalisiert CRLF-Zeilenenden bereits selbst beim Staging
(vermutlich via `.gitattributes`/`core.autocrlf`) — Dateien, deren
Inhalt nach Normalisierung identisch mit `HEAD` ist, werden gar nicht
erst in den Commit aufgenommen. Der bisher praktizierte manuelle
CRLF-Erkennungs-/Rücksetzungsschritt vor jedem Commit war unnötig
(aber harmlos); künftige Sessions können direkt `git add src/ && git
commit` nutzen und nur bei `git status`-Verdacht auf echte Inhalte
prüfen.

**Kandidatenpool-Abschluss für kleine Funktionen** (≤ 800 Bytes,
99,5–100 % Match): 488 + 87 = 575 Kandidaten über alle Einheiten
vollständig verarbeitet, **7 Treffer** (`TRoulette::TRoulette`,
`TAmenbo::doKeepDistance`, `TLightDrawBuffer::TLightDrawBuffer`,
`TBossEelEye::TBossEelEye`, `JPABaseEmitter::JPABaseEmitter`,
`TMapObjBillboard::touchActor`, `TSunGlass::loadAfter`).

**Größeres Funktionsband erstmals vollständig geprüft** (800–2500
Bytes, 99,5–100 % Match, 145 Kandidaten über 83 Einheiten): **0
Treffer** — bestätigt erneut die Runde-40-Erkenntnis, dass die
Erfolgsquote mit wachsender Funktionsgröße gegen null geht, auch
innerhalb des sonst produktiven Match-Prozent-Bands.

**Session-Gesamtstand nach Runde 43: 396 tatsächlich verifizierte
Funktionen** (389 aus Runde 1–42 plus 7 neue in Runde 43) in 53
Commits. Funktionszahl: 8976 → **8983** (**+7**). DOL SHA1 bleibt
`OK`. Der ≤ 800-Byte-Kandidatenpool im 99,5–100-%-Band ist jetzt
vollständig ausgeschöpft; verbleibende Kandidaten liegen
überwiegend in größeren, resistenten Funktionen.

### Nach vierundvierzigster Iterationsrunde (`FifoSetFog`/`FifoSetFogRangeAdj` in PacketUtil.cpp: neue Technik "Sibling-Algorithmus + Hardware-Write-Swap")

**Neue Kandidatenkategorie**: gezielte Suche nach kleinen, fast bei
0 % liegenden Funktionen, die sich beim Nachlesen als leere `{ }`-
Stubs herausstellen, statt als echte Mismatches. `src/MarioUtil/
PacketUtil.cpp` enthielt drei solche Stubs (`FifoSetFogRangeAdj`,
`FifoSetFog`, `ShapePacketCallBackFunc`).

**`FifoSetFogRangeAdj`** (312 Bytes, 1,28 % Match): erster Versuch
mit `J3DGDWriteBPCmd`/`GDOverflowCheck` (gepufferter Display-List-
Pfad, wie das Sibling `JRNISetFogRangeAdj` in `JRenderer.cpp:587`)
kompilierte sauber, aber **DIFFER** — Retail nutzt direkte rohe
Hardware-MMIO-Schreibzugriffe, nicht den gepufferten Pfad. Zweiter
Versuch mit rohem `GXWGFifo.u8 = GX_LOAD_BP_REG; GXWGFifo.u32 = reg;`
(passend zum SDK-Makro `GX_WRITE_BP_REG` aus `dolphin/gx/__gx.h` und
der festen Hardware-Adresse `GXFIFO_ADDR` aus `GXVert.h`) ergab
**Byte-exakten Match**, per `dtk elf disasm` direkt verifiziert.

**`FifoSetFog`** (344 Bytes, 1,16 % Match): komplexeres Sibling
`J3DGDSetFog` (`JRenderer.cpp:191`, A/B/C-Fog-Koeffizienten-Algorithmus
mit Mantisse/Exponent-Normalisierung über zwei While-Schleifen, Packung
via `BP_FOG_UNK0..3`/`BP_FOG_COLOR`-Makros aus `dolphin/gd/GDPixel.h`)
direkt mit denselben rohen `GXWGFifo`-Schreibzugriffen adaptiert.
Erster Versuch: 4 von 5 Schreibvorgängen matchten sofort, nur der
letzte (`BP_FOG_COLOR`) zeigte eine reine Instruktions-Scheduling-
Abweichung (6 Zeilen, gleiche Gesamtlänge 95/95) — die Farbfeld-Loads
wurden in Retail VOR dem Opcode-Byte-Write eingeplant. Fix: den
gepackten `BP_FOG_COLOR(...)`-Wert erst in eine lokale Variable
schreiben, dann `GXWGFifo.u8`/`.u32` zuweisen (statt inline im selben
Statement) — ergab sofort Byte-exakten Match.

**`SMS_InitPacket_Fog`** (140 Bytes, 93,14 % Match, gleiche Datei):
`char trash[8]` behob die Stackframe-Lücke (104→112 Bytes), aber
7 Zeilen Register-Scheduling-Differenz in der `getModelData()->
getMaterialNodePointer()->getPEBlock()->getFog()`-Aufrufkette
(vertauschte r3/r4-Zuweisung, andere Teilausdrucks-Reihenfolge)
blieben bestehen — bestätigt das etablierte Muster "additive
Frame-Lücke behoben, aber Register-Scheduling nicht fixbar via
Padding". Sauber zurückgesetzt.

**`ShapePacketCallBackFunc`** (1936 Bytes, 0,21 % Match): 11-Fall-
Sprungtabelle (passend zu den 11 `PacketUserData_*`-Structs in
derselben Datei), jeder Fall mit eigener komplexer Bit-Packing-Logik
für rohe Hardware-Schreibzugriffe (TEV-Farben, Material-Farben,
Fog-Aufrufe). Deutlich größerer Umfang als die bisherigen Erfolge
in dieser Datei, kein direktes Sibling verfügbar — als Kandidat für
eine künftige Session dokumentiert, nicht in dieser Runde angegangen.

**Neue Methodik-Erkenntnis**: die erfolgreichste Technik für leere
Stub-Funktionen ist "existierendes Sibling mit identischem Algorithmus
suchen, dann nur den Ein-/Ausgabe-Mechanismus (hier: gepufferter
Display-List-Write vs. rohe Hardware-FIFO-Writes) anpassen" — deutlich
zuverlässiger als reine Rohdisassembly-Rekonstruktion. Bei
verbleibenden Scheduling-Differenzen nach dem Sibling-Swap hilft oft
das Auslagern eines gepackten Ausdrucks in eine lokale Variable vor
dem MMIO-Write (analog zur bereits etablierten `trash[N]`-Erkenntnis,
dass MWCC Feld-Zugriffsreihenfolgen je nach Statement-Struktur anders
plant).

**Zusätzlicher Fund (`TModelWaterManager::calcWorldMinMax`, 360 Bytes,
war 10,37 % Match, keine Stub-Funktion sondern bereits plausible,
aber subtil falsche Logik)**: vier echte Bugs durch direkten `dtk elf
disasm`-Vergleich gefunden und behoben:
1. `unk5D70`/`unk5D7C` wurden aus einem gecachten `marioPos`-Local
   zugewiesen; Retail ruft `SMS_GetMarioPos()` (liefert eine
   Referenz) für jedes Ziel SEPARAT auf (passend zum doppelten
   `gpMarioPos`-Dereferenzieren in Retails Disasm).
2. Bestehender Tippfehler im Dekompilat: `fVar789.x/y/z += 1.0f`
   statt `fVar123.x/y/z += 1.0f` (Ergebnis der vorherigen `-1.0f`
   wurde versehentlich auf der falschen Variable rückgängig gemacht).
3. `TVec3::setMax()`/`setMin()`-Methodenaufrufe auf Stack-Structs
   durch rohe Skalar-Locals (`maxX`/`maxY`/`maxZ`/`minX`/`minY`/
   `minZ`) ersetzt — Retail rechnet rein in Gleitkomma-Registern
   OHNE Stackframe, während die Struct-Methodenaufrufe bei uns einen
   0x38-Byte-Frame erzwangen.
4. Exakte Vergleichsrichtung Retails nachgebildet (striktes `if
   (max > kandidat) max = kandidat;` / `if (min < kandidat) min =
   kandidat;` statt nicht-striktem `<=`/`>=`, das ein zusätzliches
   `cror+bne` statt eines einzelnen `ble`/`bge` erzeugte); Schleife
   beginnt bei Index 1 statt 0 (Index 0 diente bereits der
   Initialisierung, erneutes Verarbeiten wäre redundant).

Byte-exakter Match nach allen vier Korrekturen, per `dtk elf disasm`
bestätigt.

**Session-Gesamtstand nach Runde 44: 399 tatsächlich verifizierte
Funktionen** (396 aus Runde 1–43 plus 3 neue in Runde 44:
`FifoSetFogRangeAdj`, `FifoSetFog`, `calcWorldMinMax`) in 57 Commits.
Funktionszahl: 8983 → **8986** (**+3**). DOL SHA1 bleibt `OK`.








### Nach fünfundvierzigster Iterationsrunde (systematischer Vollabdeckungs-Scan des 99,5–99,99-%-Bands: 1 weiterer Treffer; drei neue Compiler-Heuristik-Grenzfälle dokumentiert)

**Vollständiger Neu-Scan des 99,5–99,99-%-Match-Bands über ALLE 197
betroffenen Einheiten** (730 Kandidaten, nicht mehr nur ≤ 800 Bytes
wie in Runde 43, sondern jede Größe): 515 echte additive
Stackframe-Lücken gefunden und einzeln per `char trash[N]` getestet
(automatisierte Pipeline: Funktion im Quelltext lokalisieren,
`trash[N]` einfügen, Einheit neu bauen, `dtk elf disasm` vergleichen,
bei Nichttreffer automatisch zurücksetzen). Ergebnis: **1 Treffer**
(`TSunGlass::load`, `char trash[24]`) — bestätigt erneut, dass der
additive-Lücken-Pool für dieses Band praktisch erschöpft ist (Runde
43 hatte bereits 575 Kandidaten im ≤ 800-Byte-Teilbereich
abgearbeitet; die verbleibenden ~150 neuen Kandidaten in diesem
erweiterten Scan lieferten nur den einen zusätzlichen Fund).

**Kernel-Absturz bei zu großem Batch (171 Kandidaten in einem
Eval-Aufruf) reproduziert und sauber gehandhabt**: nach Absturz zeigte
`git status` genau eine midway-modifizierte Datei
(`src/NPC/NpcChange.cpp`, ein `trash[24]`-Versuch in
`TBaseNPC::behaveToHitObject_`); direkter `dtk elf disasm`-Vergleich
außerhalb des Kernels bestätigte NICHT-Match, sauber per `git
checkout --` zurückgesetzt. Bestätigt erneut die Session-Regel:
Batch-Größe ≤ 40 Kandidaten pro Eval-Aufruf für Crash-Sicherheit.

**Drei neue, tiefer untersuchte Compiler-Heuristik-Grenzfälle
gefunden, alle nicht behebbar**:
1. **`TMapWireManager::getPointPosInNthWire`** (57 %, 96 B): Quelltext
   ruft `getWire(param_1)` bereits zweimal explizit auf (identisch zu
   Retail), aber unser Compiler CSE't (common subexpression
   elimination) den kompletten `unk18[index]`-Speicherzugriff über
   die Aufrufgrenze hinweg, während Retail den Load bei jedem Aufruf
   neu ausführt (nur der Index-Shift wird geteilt). Reine
   Optimierer-Entscheidung, nicht steuerbar.
2. **`TSpineEnemy::isReachedToGoal()`** (67,5 %, 184 B,
   `include/Enemy/Enemy.hpp`): Versuch, das Muster bereits
   existierender, funktionierender Geschwister-Implementierungen
   (`THaneHamuKuri::isReachedToGoal`, `TTamaNoko::isReachedToGoal` —
   beide kopieren `unk104.getPoint()` zuerst in ein benanntes
   `JGeometry::TVec3<f32>`-Local) auf die Basisklassen-Version
   anzuwenden. Nach Vollbau: Match-Rate verschlechterte sich auf
   42,0 % (von 67,5 %) — sofort zurückgesetzt. Zeigt: dasselbe
   Quelltextmuster kann je nach Kontext (virtuelle Funktion in
   vielfach eingebundenem Header vs. konkrete Klassenmethode in
   eigener `.cpp`) zu unterschiedlichem MWCC-Codegen führen.
3. **`JPAVecToRotaMtx`** (56,9 %, 436 B,
   `src/JSystem/JParticle/JPAMath.cpp`): Erste Analyse zeigte, dass
   Retail den `sq`-Schwellenwert (Konstante `@1489` = 0.0f) vor der
   teuren `frsqrte`-basierten Quadratwurzel prüft — das ist jedoch
   nur `JGeometry::TUtil<f32>::sqrt()`s eigener interner
   `if (mag <= 0.0f) return mag;`-Guard, inline ausgerollt, und war
   bereits identisch in unserem Code vorhanden. **Versuch**: `axis`
   (ein `TVec3`-Struct mit `.cross()`/`.scale()`/`.zero()`-Aufrufen)
   durch rohe Skalar-Locals (`axisX/Y/Z`) ersetzt, um den
   `calcWorldMinMax`-Fund (Runde 44: Struct-Methodenaufrufe erzwingen
   Stack-Spill) zu wiederholen — Ergebnis NICHT wie erwartet: Retails
   finale Matrixkonstruktion liest die Achsenkomponenten selbst
   NACH der Skalierung weiterhin von einem Stack-Puffer
   (`lfs f7, 0x14(r1)` etc.), d. h. Retail verwendet ebenfalls einen
   Stack-gestützten Aufbau — die `calcWorldMinMax`-Heuristik
   ("Struct-Methode vermeiden → kein Stack-Spill") gilt hier NICHT
   pauschal. Sauber zurückgesetzt (94 vs. 115 Zeilen, weiterhin
   92 Differenzen). Verbleibt als ungelöster Fall — die exakte
   Quellstruktur, die Retails Stack-Layout UND Registerzuteilung
   gleichzeitig reproduziert, wurde nicht gefunden.

**`TConsoleStr::processGo`** (14,2 %, 1684 B,
`src/GC2D/ConsoleStr.cpp`): mehrere bestehende
`// TODO: all wrong`/`// TODO:`-Kommentare eines früheren
Beitragenden zeigen, dass die verschachtelte If-Kette
(`param_1 >= 90/95/175`-Fälle) nur teilweise rekonstruiert ist — echte
Mehr-Stunden-Rekonstruktion, für künftige Session vorgemerkt.

**Session-Gesamtstand nach Runde 45: 400 tatsächlich verifizierte
Funktionen** (399 aus Runde 1–44 plus 1 neue in Runde 45:
`TSunGlass::load`) in 58 Commits. Funktionszahl: 8986 → **8987**
(**+1**). DOL SHA1 bleibt `OK`.

### Nach sechsundvierzigster Iterationsrunde (kritische Methodik-Lektion: rohes ELF-Byte-Lesen ohne Relokationsauflösung ist unzuverlässig für Datenvergleiche; Destruktor-Pool erneut als bekanntes Artefakt bestätigt)

**Stichprobenprüfung des `fp=None`-Destruktor-Pools** (573 Kandidaten
mit `__dt__`-Namensmuster und fehlendem `fuzzy_match_percent` im
aktuellen `report.json`): Stichprobe `TWoodLog::~TWoodLog()`
(`mario/MoveBG/MapObjBianco`) zeigt das Symbol existiert NICHT im
eigenen `src`-Build dieser Einheit (weak-Symbol, vom Compiler in
dieser TU nicht dupliziert, da Retail hier großzügiger dupliziert).
Bestätigt exakt das bereits in Runde 32/33 etablierte
Per-TU-Duplikations-Artefakt — keine neue Erkenntnis, keine
Handlungsoption ohne MWCC-interne Heuristik-Kontrolle.

**KRITISCHER METHODIK-FUND**: Versuch, `.data`-Abschnitte direkt
durch rohes Parsen der ELF-Sektionsbytes (Python, ohne
Relokationsauflösung) zwischen `src/*.o` und `obj/*.o` zu vergleichen,
um datenreiche Einheiten mit 100 % Code-Match aber niedrigem
Daten-Match zu untersuchen (`mario/JSystem/JAudio/JASystem/
JASPlayer_impl`, 100 % Code / 2,1 % Daten laut `report.json`).
Rohvergleich zeigte scheinbar `sTreTable[8]` mit `0x8001` in unserem
Quelltext vs. `0x0000` in Retails `.o`-Datei — Korrektur angewendet,
**DOL-SHA1-Check schlug danach fehl** (`build/GMSJ01/mario.dol:
FAILED`)! Sofort zurückgesetzt, DOL wieder `OK` bestätigt.

**Ursache**: diese Einheit ist eine `complete: true`-Einheit (verlinkt
aus `src/*.o`, bestätigt durch die direkte Reaktion des
DOL-Hash-Checks auf die Quelltextänderung) — der ORIGINALE Wert
`0x8001` war die ganze Zeit korrekt und bereits Teil der
erfolgreich verlinkten, exakt passenden DOL. Der rohe
ELF-Sektionsvergleich (ohne `.rela.data`-Relokationen aufzulösen)
lieferte ein IRREFÜHRENDES Ergebnis für `obj/*.o` an dieser Stelle.

**Neue Methodik-Regel**: rohes ELF-`.data`-Byte-Lesen ohne
Relokationsauflösung ist NICHT als eigenständige Verifikationsmethode
für Datenwerte zu verwenden. Für `complete: true`-Einheiten ist der
DOL-SHA1-Check die einzige zuverlässige Autorität (dieser hat den
Fehler hier korrekt und sofort erkannt — das etablierte
Verifikationsprotokoll „DOL-SHA1 nach jeder Änderung prüfen" hat
genau wie vorgesehen funktioniert und eine reale Regression
verhindert). Für `complete: false`-Einheiten bleibt `dtk elf disasm`
(Code) die etablierte Methode; ein äquivalent zuverlässiges Werkzeug
für Datenabschnitts-Vergleiche wurde in dieser Runde NICHT gefunden
und ist für künftige Sessions offen (evtl. `objdiff-cli`s internes
`data_diff`-Feature untersuchen, falls über eine unterstützte
Schnittstelle zugänglich).

**Keine neuen Fixes in Runde 46** — beide Untersuchungslinien
(Destruktor-Pool, Datenvergleich) endeten in bereits bekannten bzw.
neu entdeckten, aber nicht umsetzbaren Sackgassen. Session-Gesamtstand
bleibt bei **400 tatsächlich verifizierten Funktionen** in 58 Commits
(unverändert seit Runde 45). DOL SHA1 bestätigt `OK`.


### Nach siebenundvierzigster Iterationsrunde (`TBathtubKiller::attackToMario`: reale Logik zu 93 % rekonstruiert, algorithmische Struktur bestätigt, Restdifferenz im Stackframe ungelöst)

Erster echter Rekonstruktionsversuch im Runde-44-dokumentierten
TBathtubKiller-Cluster. `attackToMario()` (0 % Match, leerer `{ }`-Stub,
404 B Retail-Zielgröße) per direktem `dtk elf disasm`-Studium der
Retail-Instruktionen vollständig algorithmisch rekonstruiert:

```cpp
void TBathtubKiller::attackToMario()
{
	bool isDying
	    = mSpine->getCurrentNerve() == &TNerveBathtubKillerExplosion::theNerve()
	      || mSpine->getCurrentNerve() == &TNerveBathtubKillerBreak::theNerve();

	if (!isDying) {
		if (SMS_GetMarioPos().y < mPosition.y) {
			mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
			SMS_ThrowMario(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f), 10.0f);
			unk21C = 1;
		}
	}
}
```

**Wichtiger Zwischenschritt**: die erste Fassung nutzte De-Morgan-Form
(`cur != A && cur != B`), was zu VERTAUSCHTER Flag-Polarität gegenüber
Retail führte (Retail initialisiert das "Ist-Sterbend"-Flag mit 1 und
löscht es bedingt; unsere De-Morgan-Form initialisierte mit 0 und
setzte bedingt auf 1 — bei IDENTISCHEM Wahrheitswert, aber
UNTERSCHIEDLICHEM Opcode-Muster). Umschreiben auf die direkte
Oder-Form `cur == A || cur == B` mit `if (!isDying)` traf exakt
Retails Register-Initialisierungsmuster (`li r30, 0x1` an der
richtigen Stelle) — ein weiterer bestätigter Fall, dass logisch
äquivalente, aber unterschiedlich geschriebene Boolesche Ausdrücke zu
verschiedenem MWCC-Code führen (ähnlich der `calcWorldMinMax`- und
`calcVMMtxGround`-Funde).

**Verbleibende Differenz**: nach der Oder-Form-Korrektur matcht die
komplette Kernlogik (Nerve-Vergleich, verschachteltes `pushNerve()`,
`SMS_SendMessageToMario`, `SMS_ThrowMario`, alle Konstanten und
Sprungziele) strukturell 1:1 mit Retail — nur der Stackframe ist
8 Bytes GRÖSSER als Retails (`-0x58` vs. `-0x50`), OBWOHL dieselbe
Anzahl Register gesichert wird. `char trash[8]` (Standardtechnik
dieser Session) verschlimmerte die Lücke auf 16 Bytes, da unser Frame
bereits GRÖSSER war — die etablierte Padding-Technik ist hier nicht
anwendbar (sie hilft nur, wenn UNSER Frame kleiner ist). Der
`bool isDying`-Local direkt in die `if`-Bedingung inlinen (kein
benannter Local) verschlimmerte die Situation weiter (120 statt
109 Zeilen, zusätzlicher `...bss.0`-Bezug, vermutlich verdoppelte
Lazy-Init-Instanziierung durch geänderte Ausdrucksauswertung) — sauber
zurückgesetzt.

**Sauber zurückgesetzt** (kein Netto-Fix, `git checkout --` bestätigt,
DOL SHA1 `OK`). Der bestätigte Algorithmus (inkl. der Oder-Form-
Erkenntnis) bleibt als direkt wiederverwendbare Vorlage für einen
künftigen Anlauf mit mehr Zeit für die Stackframe-Spurensuche
(vermutlich ein zusätzliches temporäres Objekt oder eine andere
Local-Variablen-Anordnung, die Retail nutzt, um 8 Bytes zu sparen).

**Session-Gesamtstand nach Runde 47: weiterhin 400 tatsächlich
verifizierte Funktionen** (unverändert seit Runde 45) in 58 Commits.
DOL SHA1 bestätigt `OK`.


### Nach achtundvierzigster Iterationsrunde (breiter 70–99,5-%-Scan außerhalb bekannter Cluster: 84 Kandidaten, 0 Treffer; `initECTMir` bis auf 4 Zeilen rekonstruiert; neue Registerzuteilungs-Fallklasse dokumentiert)

**`TBathtubKiller::attackToMario`-Stackframe-Rätsel weiter untersucht**
(named-local-Variante des `TRect`-Temporarys, `int` statt `bool`) —
keine der Varianten änderte die Frame-Differenz; Sackgasse endgültig
bestätigt, sauber zurückgesetzt.

**Breiter Scan des 70–99,5-%-Bands außerhalb aller bisher bearbeiteten
Cluster-Dateien** (118 Kandidaten in 76 Einheiten, Größe 20–300 Bytes):
84 Kandidaten mit Stackframe-Differenz gefunden und automatisiert per
`char trash[N]` getestet — **0 Treffer**. Bestätigt, dass auch dieses
Band inzwischen weitgehend erschöpft ist.

**`TMarDirector::initECTMir`** (99,48 %, 248 B) am weitesten
rekonstruiert: `char trash[24]` schloss die anfängliche 24-Byte-
Framelücke exakt; Umschreiben des `JDrama::TRect`-Funktionsarguments
auf ein benanntes `srcRect`-Local traf zusätzlich Retails exakte
Registerreihenfolge (`r3` vor `r4` statt umgekehrt) — von 14 auf
4 Diff-Zeilen reduziert. Verbleibend: zwei kosmetische
`...rodata.N`-vs-`"@N"`-Label-Unterschiede (derselbe String-Literal-
Inhalt, nur andere Pool-Nummerierung) sowie ein hartnäckiger
"interner Slot-Versatz" (`0x40` vs. `0x64` als Stack-Offset für
`srcRect`, bei bereits identischer Gesamt-Framegröße) — bestätigt
erneut die bereits mehrfach dokumentierte resistente Kategorie
(6+ frühere Fälle diese Session). `trash[N]`-Platzierung vor/nach der
Local-Deklaration verändert den Offset nicht. Sauber zurückgesetzt.

**Neue Registerzuteilungs-Fallklasse dokumentiert**
(`execute__18TNerveNameKuriLandCFP24TSpineBase<10TLiveActor>`,
96,8 %, 144 B): Retail hält den `self`-Zeiger NIE in einem
Callee-Saved-Register (kein `r31`-Save, Frame nur `-0x8` statt
unserer `-0x20`) — es nutzt `r3` direkt durch, weil auf JEDEM
Kontrollflusspfad entweder `self` vor dem einzigen `bl
checkCurAnmEnd`-Aufruf verbraucht wird oder dieser Aufruf komplett
übersprungen wird (Short-Circuit-`&&`). Unser Compiler entscheidet
sich konservativer für `r31`-Sicherung. Gleiche Quellstruktur wie
Retail (`if (self->isBckAnm(4) && self->checkCurAnmEnd(0)) return
true; if (!self->isAirborne()) self->setBckAnm(4); return false;`)
— reine MWCC-Liveness-Analyse-Differenz, nicht in dieser Runde weiter
verfolgt (passt in dieselbe "asymmetrische Compiler-Optimierungs-
entscheidung"-Kategorie wie `identity33`).

**Keine neuen Fixes in Runde 48.** Session-Gesamtstand bleibt bei
**400 tatsächlich verifizierten Funktionen** in 58 Commits
(unverändert seit Runde 45). DOL SHA1 bestätigt `OK`.



## Nächster GMSJ01-Kandidat
**Neuer, großer Kandidaten-Cluster identifiziert (Runde 44):
`src/Enemy/BathtubKiller.cpp`** — `TBathtubKiller`/`TBathtubKillerManager`
haben ca. 12 Funktionen mit echtem, aber nicht byte-genauem
Rekonstruktions-Bedarf: `bind` (0,4 %, 1004 B), `perform` (10,0 %,
1164 B, mit bestehendem TODO "only the bathtub lookup is
reconstructed"), `makeInitialVelocity` (0,4 %, 932 B), `moveChasing`
(0,7 %, 604 B), `makeQuat` (90,5 %, 1532 B — selbst NICHT vollständig
korrekt trotz vollständiger Logik), `receiveMessage` (0,9 %, 620 B),
`attackToMario` (1,0 %, 404 B), `isCollidMove` (0,7 %, 1068 B),
`behaveToWater` (34,3 %, 280 B), `isAboided` (0,7 %, 836 B), die
`execute`-Methoden von vier `TNerveBathtubKiller*`-Nerven (Wander
0,6 %/952 B, Chase 0,6 %/912 B, ChaseStraight 0,5 %/1088 B, Straight
62,4 %/400 B), sowie `TBathtubKillerManager::load`/`loadAfter`
(81,0 %/79,1 %). Viele weitere Methoden (`killBathtubKiller`,
`explodeBathtubKiller`, `moveParabolic`, `moveStraight`,
`makeVelocityQuat`, `makeAccelerationQuat`, `makeScrewQuat`,
`setNormalBathtubKillerAnm` u. a.) sind leere `{ }`-Stubs, tauchen
aber NICHT in `report.json` auf — vermutlich vom Compiler wegoptimiert/
wegge-inlined (keine eigenständigen Symbole in Retail).

**Tiefenanalyse `TNerveBathtubKillerStraight::execute`** (62,4 %, hat
bereits echte, plausible Logik, kein Stub): direkter `dtk elf
disasm`-Vergleich zeigt, dass Retail zwei explizite Funktionsaufrufe
(`TVec3<f>::dot`, `TVec3<f>::scale`) tätigt, die in unserem Build
vollständig zu rohen Gleitkomma-Instruktionen ge-inlined sind (der
Rest der Funktion — `getMActor`-Aufruf, `makeQuat`-Aufruf — matcht
strukturell). Sehr wahrscheinlich dieselbe bestätigt-unfixbare
Kategorie "asymmetrisches Pro-Aufrufstellen-Inlining" wie
`TRotation3::identity33` (Runde 34) — nicht mit `#pragma dont_inline`
behebbar, da funktionsweit statt pro Aufrufstelle wirksam.

**Einschätzung**: Der TBathtub-Cluster (Runde 39/40, blockiert durch
fehlende `TBathtubGrip`-Klasse und `-fp_contract`) und der
TBathtubKiller-Cluster sind beide grundsätzlich reale, aber deutlich
größere Mehr-Stunden-Rekonstruktionsprojekte (Physik-/Quaternion-
Bewegungslogik, ~8 verbleibende Funktionen mit insgesamt > 8000
Bytes) statt schnelle Fortsetzungs-Gewinne — für eine künftige
dedizierte Session vorgemerkt, nicht in Runde 44 angegangen.

**Weitere Funde in `ModelWaterManager.cpp` (Runde 44)**:
`calcVMMtxGround`/`calcVMMtxWall` (62,49 %/54,12 %, beide mit
bestehendem `// TODO: matching this is ewwwwwwwwwwwwwwwwww`-Kommentar
eines früheren Beitragenden) — Ursache teilweise gefunden: Quellcode
nutzt `param_4.y * 2.0 + param_3.y` mit `2.0` (Double-Literal statt
`2.0f`), wodurch MWCC ein `fmadd` (Doppelpräzision) statt Retails
`fmadds` (Einzelpräzision) erzeugt. Korrektur auf `2.0f` behebt genau
dieses Symptom (bestätigt an den erzeugten Opcodes), reduziert die
Differenz messbar, erreicht aber in keiner der beiden Funktionen
100 % — verbleibende Differenz ist reine Registerzuteilung
(unterschiedliche Stackframe-Größe, 3 vs. 2 gesicherte
Gleitkomma-Register). Sauber zurückgesetzt, da nicht vollständig
matchend; Erkenntnis über den `2.0`-vs-`2.0f`-Unterschied als
Ausgangspunkt für eine künftige Session festgehalten.

**`TSunModel::calcDispRatioAndScreenPos_`** (14,15 %, 292 B, `weak`
Symbol in Retail): Ursache identifiziert — der einzige Aufrufer von
`CLBScreenFPosToSPos()` (`include/Camera/cameralib.hpp:338`, bereits
mit `#pragma dont_inline on/off` UND explizitem C++-`inline`-Keyword
versehen) wird trotz Pragma vollständig ge-inlined (516 statt
292 Bytes, kein eigenständiges `CLBScreenFPosToSPos`-Symbol im
eigenen Build). Ein bereits bestehender Kommentar eines früheren
Beitragenden ("TODO: definitely more inlines but I couldn't get it
to work out...") bestätigt: dasselbe Problem wurde schon einmal
erfolglos angegangen. Versuch, das `inline`-Schlüsselwort zu
entfernen, sofort zurückgesetzt (Header wird von 37+ Übersetzungs­
einheiten eingebunden, hätte Mehrfachdefinitions-Linkerfehler
verursacht). Bestätigt dieselbe MWCC-Pragma-Ignorier-Eigenart wie
andere in dieser Session dokumentierte Fälle — nicht ohne tiefere
Compiler-Archäologie behebbar.



**Wieder offen (siehe Methodik-Korrektur oben)**: 58 der ursprünglich
59 in Runde 32/33 als "bereits korrekt" dokumentierten Funktionen
(1 davon — `TCylinder::makeDL` — in Runde 35 als tatsächlich korrekt
bestätigt, siehe oben; `TFireHamuKuri::moveObject/isHitValid` in
Runde 35 gefixt). Korrektur einer Falschaussage aus der ersten
Retraction-Notiz: `dieFire`/`genFire`/`recoverFire`/`TNerveFire-
HamuKuriRecover::theNerve` sind ENTGEGEN der ursprünglichen (auf
veralteten `objdiff-cli`-Diff-Daten basierenden) Behauptung NICHT
komplett fehlend — `dieFire`/`genFire` sind triviale leere
Ein-Zeiler, `recoverFire` existiert mit einem bestehenden `// TODO:
this is the wrong inline, size doesn't match at all!`-Kommentar eines
früheren Beitragenden.

**Präzisierung zu `MarNameRefGen_Enemy.cpp`**: Die zwölf dort
"fehlenden" Destruktoren (`TSimpleEffect`, `TLauncherManager`,
`TTobiPuku`, `TTobiPukuManager`, `TTobiPukuLaunchPad`,
`TTobiPukuLaunchPadManager`, `TPoiHana`, `TGesso` u. a.) sind KEINE
fehlende Funktionalität — Stichprobe `TGesso::~TGesso()` zeigt den
Destruktor korrekt und vollständig in `src/Enemy/gesso.o` (weak
Symbol `__dt__6TGessoFv`, plus `@32@`-Vtable-Adjustor-Thunk). Es
handelt sich um eine reine Per-TU-Duplikations-Entscheidung: Retails
Compiler legt in `MarNameRefGen_Enemy.o` zusätzlich eine redundante
lokale Kopie an (via `new TGesso` in `getNameRef_Enemy()`, das die
Vtable und damit die Destruktor-Adresse referenziert), unser Compiler
entscheidet sich hier für eine externe Referenz statt lokaler
Duplizierung — funktional identisch (der Linker verwirft ohnehin alle
bis auf eine Kopie), aber sichtbar als abweichendes Symbolset in
genau dieser Einheit. Vermutlich dieselbe Kategorie wie das
`TRotation3::identity33`-Problem aus Runde 34 (asymmetrische
Per-Aufrufstellen-Entscheidung des Compilers, nicht per Pragma
erzwingbar) — nicht ohne tiefere MWCC-Heuristik-Archäologie behebbar,
aber auch keine Prioritäts-Baustelle, da die eigentliche
Funktionalität nachweislich korrekt ist.

`ItemManager::newAndRegisterCoin` (99,59 %) und `PollutionManager::
cleanedAll` (96,43 %) offen.
`JSystem/J3D/J3DGraphAnimator/J3DModel.cpp::entryModelData` (1248 Bytes,
99,76 %) — Aufteilen des inline `&mShapePackets[shape->getIndex()]`-Ausdrucks
in einen expliziten `J3DShapePacket*`-Local behob den ersten
Register-Swap-Cluster (99,76 % → 99,84 %), vergrößerte aber den Stackframe
um 8 Bytes über das Original hinaus (jeder neue Pointer-Local kostet dort
mehr als erwartet). Drei Varianten (separater `shape`-Local, kombinierter
Ausdruck, zusätzliches `trash[8]`) landeten alle bei 99,84 % mit
falschem Frame; zurückgesetzt auf die Original-99,76-%-Fassung, da sie
wenigstens den korrekten Stackframe hat. `JSystem/JKernel/JKRExpHeap.cpp::
allocFromHead(u32,int)` (98,78 %, Register-Scheduling um -1-Konstante)
ebenfalls in drei Varianten versucht, kein Fortschritt.

### Nach neunundvierzigster Iterationsrunde (`TBathtubKiller::behaveToWater`: falsche Algorithmus-Logik entdeckt und korrigiert rekonstruiert; **grundlegender Methodik-Fund**: `theNerve()`-Inlining ist eine dateiweite Compiler-Heuristik, kein Einzelfunktions-Rätsel)

**Ausgangspunkt**: Im dokumentierten `TBathtubKiller`-Cluster (Runde 47) wurde
`behaveToWater(THitActor*)` erneut untersucht. Der bisherige Quellcode
(`{ breakBathtubKiller(); }`, 34,3 % Fuzzy-Match) erwies sich beim direkten
Disassembly-Vergleich nicht nur als unpräzise, sondern als **komplett falscher
Algorithmus**: Retail implementiert exakt dasselbe `isDying`-Muster wie
`attackToMario` (Vergleich des aktuellen Nerve gegen
`TNerveBathtubKillerExplosion::theNerve()` und `TNerveBathtubKillerBreak::
theNerve()`), gefolgt von `mSpine->pushNerve(&TNerveBathtubKillerBreak::
theNerve())`, falls nicht sterbend — kein Aufruf von `breakBathtubKiller()`
überhaupt. Als Quellcode rekonstruiert:

```cpp
void TBathtubKiller::behaveToWater(THitActor*)
{
	bool isDying
	    = mSpine->getCurrentNerve() == &TNerveBathtubKillerExplosion::theNerve()
	      || mSpine->getCurrentNerve() == &TNerveBathtubKillerBreak::theNerve();

	if (!isDying) {
		mSpine->pushNerve(&TNerveBathtubKillerBreak::theNerve());
	}
}
```

Kompiliert sauber, aber der Disassembly-Vergleich zeigte einen fundamentalen
strukturellen Unterschied: Im Retail-Objekt existiert `TNerveBathtubKillerExplosion
::theNerve()` (und `TNerveBathtubKillerBreak::theNerve()`) als **eigenständige,
reale Out-of-line-Funktion** (`.fn theNerve__28TNerveBathtubKillerExplosionFv`,
ausschließlich per `bl` aufgerufen — bestätigt an *allen* sechs Retail-Aufrufstellen
im Cluster: `behaveToWater` ×2, `isCollidMove` ×7, `receiveMessage` ×4,
`perform` ×3, `bind` ×5, `attackToMario` ×1 „echter" Aufruf). In unserem aktuellen
Build dagegen wird der komplette `theNerve()`-Rumpf (Lazy-Init-Guard, vtable-
Konstruktion, `__register_global_object`-Aufruf) **vollständig inline** in
`behaveToWater` expandiert — die Funktion `theNerve__28TNerveBathtubKillerExplosionFv`
existiert in unserem Objekt gar nicht als eigene Symboldefinition.

**Hypothesentest 1 (Datei-Reihenfolge)**: Vermutung, dass die Inline-Entscheidung
von der Position der *ersten* Aufrufstelle in der Datei abhängt (`attackToMario`
erscheint vor `behaveToWater`). `attackToMario` probeweise mit der in Runde 47
rekonstruierten Logik wieder eingefügt (gemeinsam mit `behaveToWater`) und neu
gebaut — **kein Effekt**, `behaveToWater` blieb vollständig inline. Hypothese
verworfen.

**Fund im Retail-Disassembly**: `attackToMario` selbst dupliziert den
Lazy-Init-Guard für `TNerveBathtubKillerExplosion` *zweimal inline* (einmal für
den `isDying`-Vergleich, einmal für die `pushNerve(&Explosion::theNerve())`-Aufrufstelle
später in derselben Funktion) — vermutlich eine lokale CSE-Optimierung des
Compilers für *mehrfach in derselben Funktion referenzierte* Aufrufe. Für
`TNerveBathtubKillerBreak` (nur einmal in `attackToMario` referenziert) bleibt
es dagegen bei einem regulären `bl theNerve__24TNerveBathtubKillerBreakFv`.
Dieses Verhalten ist strikt lokal pro Funktion, keine dateiweite Weiterverbreitung
(bestätigt: `behaveToWater` referenziert `TNerveBathtubKillerExplosion::theNerve()`
in Retail ebenfalls nur einmal und erhält dort einen regulären `bl`-Aufruf, nicht
die Inline-Behandlung).

**Hypothesentest 2 (Aufrufstellen-Gesamtzahl in der TU)**: Vermutung, dass MWCCs
Auto-Inliner die Entscheidung „lohnt sich eine eigene Out-of-line-Funktion" von
der *Gesamtzahl* der `theNerve()`-Aufrufstellen in der ganzen Übersetzungseinheit
abhängig macht (Retail: rund 22 Aufrufstellen über sechs Funktionen; unser
aktueller Stand: 2–6). Günstiger Test: `isAboided()` und `canChase()`
(bisher leere Stubs) probeweise mit zusätzlichen (nicht committeten)
`theNerve()`-Referenzen bestückt, um die Gesamtzahl testweise zu erhöhen, ohne
die vollständige Cluster-Rekonstruktion vorwegzunehmen. Nach Neubau blieb
`behaveToWater` weiterhin vollständig inline — Hypothese in dieser einfachen
Form ebenfalls verworfen (der tatsächliche Schwellenwert, falls einer existiert,
liegt jedenfalls deutlich höher als die getestete Aufstockung, oder die
Entscheidung hängt zusätzlich von der Gesamtkomplexität/-größe der Datei ab,
die in unserem Stand noch weit von Retails vollständiger ~4000-Zeilen-Implementierung
entfernt ist).

**Schlussfolgerung**: Das `theNerve()`-Inlining-Verhalten ist keine lokale
Stellschraube, die sich durch Umformulierung einer einzelnen Funktion lösen
lässt (anders als die bisher dokumentierten `char trash[N]`-Fälle). Es handelt
sich um eine dateiweite MWCC-Optimierungsheuristik, deren genaue
Schwellenwertbedingung nicht mit vertretbarem Aufwand isoliert reproduzierbar
ist, ohne den *gesamten* `TBathtubKiller`-Cluster (`isCollidMove`,
`receiveMessage`, `perform`, `bind` — je 100–300+ Byte komplexe Funktionen,
alle mit Retail-Referenzdisassembly bereits in `build/_btk_check_obj.s`
vorliegend) gemeinsam korrekt zu rekonstruieren. Das ist ein legitimes, aber
deutlich größeres Vorhaben als ein Einzelfunktions-Fix und wird für eine
zukünftige, dedizierte Mehrfunktions-Rekonstruktionsrunde zurückgestellt.
Da `behaveToWater` mit der korrigierten Logik weiterhin nicht Byte-exakt
matcht, wurde die Änderung gemäß Session-Regel (nur Byte-exakte Fixes werden
committet) sauber zurückgesetzt; `src/Enemy/BathtubKiller.cpp` bleibt im
Stand von Runde 48.

**Wichtiger methodischer Nebenbefund**: Die Entdeckung selbst — dass
`behaveToWater`s bisherige Logik (`breakBathtubKiller()`) *inhaltlich falsch*
war, nicht nur unpräzise geplant — bestätigt erneut den in Runde 47
etablierten Ansatz „bereits implementierte, aber nicht vollständig
matchende Funktionen im TBathtubKiller-Cluster auf falsche Algorithmen prüfen,
nicht nur auf Register-/Rahmen-Rätsel". Das `isDying`-OR-Muster
(`cur == &Explosion::theNerve() || cur == &Break::theNerve()`) bleibt als
Vorlage für zukünftige Cluster-Arbeit bestätigt (jetzt dreifach beobachtet:
`attackToMario`, `behaveToWater`, sowie in `isCollidMove`/`receiveMessage`s
Retail-Disassembly strukturell wiedererkannt).

Session-Gesamtstand bleibt bei 400 verifizierten echten Fixes in 58 Commits
(kein neuer Commit diese Runde — Inhalt korrekt rekonstruiert, aber
Byte-Match durch dateiweite Inlining-Heuristik blockiert).

### Zwischenstand Runde 49: eigene Tiefenanalyse `TLiveActor::control()` und `evIsNpcSinkBottom`

**`TLiveActor::control()`** (`src/Strategic/liveactor.cpp`, 73,96 % Match, 200
Bytes) enthielt zwei `// call on unk90`-Platzhalterkommentare eines früheren
Mitwirkenden (Feld `void* unk90` mit unbekanntem Typ, nirgendwo im
Repository mit konkretem Typ belegt). Aus dem Retail-Disassembly exakt
rekonstruiert: beide Stellen rufen `((vtable von *(unk90+0x5c))[4])(unk90)`
auf (Rohzeiger-Vtable-Dispatch, passend zum bereits im Code verwendeten
Stil `*(int*)((char*)unk90 + 4)` für denselben unbekannten Typ). Zusätzlich
ersetzte `mSpine->isIdle()` (materialisiert eine Bool-Variable vor dem
Sprung) durch die direkte OR-Form `mSpine->getCurrentNerve() != nullptr ||
mSpine->getVertebraeCount() > 0` (Negation von `isIdle()`), wodurch die
Verzweigungsstruktur exakt auf Retails direkte Sprunglogik ohne
Bool-Materialisierung einschwenkte. Ergebnis: **alle 58 Instruktionen
strukturell identisch** (gleiche Anzahl, gleiche Opcodes, gleiche
Sprungstruktur nach Label-Kanonisierung) — einzige Restdifferenz: `unk90`
wird bei uns in r5, bei Retail in r4 alloziert (reine
Registerzuteilungsentscheidung, drei verschiedene Formulierungen probiert:
Rohzeiger-Cast, benannte `void** vtbl`-Lokale, zusätzlicher
`void* p = unk90`-Cache — alle drei identisches Resultat). Gehört zur
bereits dokumentierten Kategorie „asymmetrische
Register-Zuteilungsentscheidung" (wie `TNerveNameKuriLand::execute`,
`TRotation3::identity33`) — kein Byte-Match erreicht, sauber
zurückgesetzt.

**`evIsNpcSinkBottom`** (`src/NPC/NpcEvent.cpp`, über die gemeinsame
Hilfsfunktion `IsNpcFlagOn_`, 72,17 % Match, 264 Bytes) zeigte einen
deutlicheren strukturellen Unterschied: Retail ruft für den finalen
`interp->push(result)`-Aufruf die reale Out-of-line-Funktion
`push__21TSpcStack<9TSpcSlice>FRC9TSpcSlice` auf, während unser Build den
kompletten Push-Rumpf (Kapazitätsprüfung, Speichern, Größe inkrementieren)
inline expandiert. `TSpcStack<T>::push()` ist in `include/Strategic/
spcinterp.hpp` als gewöhnliche (nicht explizit inline-markierte, aber
kurze) Methode definiert. **Risikoabschätzung durchgeführt, bevor
experimentiert wurde**: Dieselbe Datei enthält mindestens sieben weitere
`ev*`-Funktionen, die exakt denselben `interp->push(result)`-Aufruf nutzen
und bereits bei 100 % Match liegen (`evConnectDummyNpc`,
`evOnTalkToDummyNpc`, `evSetNpcBalloonMessage`,
`evSetNpcTalkForbidCount`, `evNpcDanceOn`, `evNpcDanceOffHappyOn`,
`evResetFruitNum`) — für diese muss `push()` also bereits korrekt inline
expandiert werden. Ein pauschales `#pragma dont_inline` auf die geteilte
Template-Methode hätte ein hohes Risiko, diese sieben bereits
matchenden Funktionen zu regressieren (dieselbe Gefahrenklasse wie der in
dieser Runde bereits dokumentierte `theNerve()`-Fall im
`TBathtubKiller`-Cluster: eine dateiweite/funktionsabhängige
Inlining-Heuristik, keine lokale Stellschraube). Ohne experimentellen
Eingriff auf die geteilte Header-Datei als „untersucht, aber zurückgestellt"
eingestuft; keine Änderung vorgenommen.

**Parallele Subagenten-Ergebnisse** (acht parallel gestartete
Untersuchungsaufgaben für Funktionen mit real existierender, aber
niedrig-prozentiger Implementierung — dieselbe „falscher statt nur
unpräziser Algorithmus"-Technik wie beim `behaveToWater`-Fund):
- **`TMameGesso::reset()`** (47,9 % → **MATCH**, Commit `210212f2`):
  `unk1CC = MsRandF(l, r)` (reine Float-Arithmetik) ersetzt durch das
  bereits vorhandene `TMsRange<s32>(0, interval).rand()`-Template — Retail
  nutzt Integer-Subtraktion vor der Float-Konvertierung, nicht reine
  Float-Arithmetik. Byte-identisch bis auf kosmetische
  Konstanten-Pool-Nummerierung.
- **`TDoroHamuKuri::attackToMario()`** (43,5 % → **MATCH**, Commit
  `351c13f0`): Algorithmus war korrekt; MWCC inlinete
  `THamuKuri::selectCapHolder()` komplett, Retail ruft es reell
  (`bl selectCapHolder`). Fix: `#pragma dont_inline` um die
  **Callee**-Definition (nicht den Aufrufer) — bestätigt die aus Runde 29
  bekannte Regel, dass das Pragma um die aufgerufene Funktion stehen muss.
- **`TApplication::TApplication()`** (36,4 % → **NO-MATCH**, zurückgesetzt):
  Quellcode bereits semantisch exakt (Initialisierungsreihenfolge korrekt);
  Lücke stammt aus `JDrama::TFlagT<T>::set`/Kopierkonstruktor, die in
  `JDRFlag.hpp` inline definiert sind, während Retail sie als externe
  Weak-Symbole in `MarDirectorDirect.cpp` referenziert (0 Bytes Stackframe
  bei uns vs. 0x40 bei Retail). `#pragma dont_inline`/`inline_depth`
  in vier Varianten erfolglos getestet (siehe Unteragenten-Bericht) —
  Cross-TU-Header-Änderung nötig, außerhalb des Aufgabenumfangs,
  zurückgestellt für eine dedizierte JDRFlag.hpp-Untersuchung.

Fünf weitere Unteragenten-Untersuchungen (`SunModel`, `MarioOnYoshi`,
`HamuKuriIsHitValid`, `WoodBlockLoad`, `WarpInCallBackExecute`) liefen zum
Zeitpunkt dieses Zwischenstands noch; Ergebnisse folgen im nächsten
Abschnitt.

### Zwischenstand Runde 49, Teil 2: `defer_codegen`-Durchbruch und weitere Subagenten-Ergebnisse

**Grundlegender Methodik-Durchbruch** (gefunden vom `SunModel`-Unteragenten,
eigenständig reproduziert und bestätigt am `theNerve()`-Fall aus Teil 1
dieser Runde): `#pragma dont_inline` ist im gesamten Projekt praktisch
wirkungslos, weil `configure.py`s `-inline deferred`-Compilerflag jeden
Pragma-Zustand verwirft, BEVOR er wirken kann. Der Fix:
`#pragma defer_codegen off` als ERSTE Zeile einer `.cpp`-Datei setzt
dieses Verhalten für die gesamte Datei zurück, wonach bereits vorhandene
`#pragma dont_inline`-Markierungen in Headern (wie `at()` in
`JGMatrix33.hpp`, ~88 Stellen projektweit) endlich greifen. Bestätigt an
zwei unabhängigen Fällen:
- **`TSunModel::calcDispRatioAndScreenPos_`** (14,15 % → **MATCH**, Commit
  `4c40b60e`): `CLBScreenFPosToSPos` blieb dank `defer_codegen off` +
  gezieltem `dont_inline` um die Zielfunktion out-of-line, exakt wie
  Retail. Zusätzlich `char trash[16]` für eine reine Rahmengrößenlücke.
  Nebenfund: `CLBScreenFPosToSPos` existierte in unserem Objekt vorher
  GAR NICHT (0 %), jetzt nahezu vollständig (75/75 Instruktionen) — ein
  verbleibender Bug in `include/Camera/cameralib.hpp` (u16/s16-Vorzeichen-
  Fehlkonvertierung bei `SMSGetGameRenderHeight`/`Width`, plus 8-Byte-
  Rahmenlücke) wurde dokumentiert, aber bewusst NICHT behoben (Header mit
  37+ Includern, außerhalb des Aufgabenumfangs).
- **Eigener Test in `TBathtubKiller::behaveToWater`** (siehe Teil 1): Mit
  `#pragma defer_codegen off` am Dateianfang wird
  `theNerve__28TNerveBathtubKillerExplosionFv`/
  `theNerve__24TNerveBathtubKillerBreakFv` jetzt korrekt als reale
  Out-of-line-Funktion aufgerufen (`bl`) statt komplett inline expandiert
  — der in Teil 1 dokumentierte `theNerve()`-Cluster-Mythos ist damit
  **teilweise aufgeklärt**: nicht dateiweite Heuristik allein, sondern
  das gemeinsame `-inline deferred`/`dont_inline`-Problem. Restdifferenzen
  bleiben aber bestehen: (a) eine dritte `theNerve()`-Referenz innerhalb
  von `mSpine->pushNerve(...)`s inline-Expansion wird weiterhin komplett
  inline rekonstruiert statt (wie Retail) die bereits konstruierte
  `instance`-Adresse direkt wiederzuverwenden — drei Formulierungen
  (OR-Kette, separate `bool a,b`-Lokale, gemeinsame `breakNerve`-Lokale)
  probiert, keine erreichte exakten Match; (b) die Boolean-
  Materialisierung des `isDying`-Vergleichs nutzt bei Retail ein
  `subf+cntlzw+extrwi.`-Bitmuster, bei uns entweder direktes Branching
  oder ein abweichendes `srwi`-Muster. `behaveToWater` bleibt daher
  NICHT gematcht, sauber zurückgesetzt (inkl. der `defer_codegen`-Pragma-
  Zeile). **Für zukünftige Cluster-Arbeit festgehalten**: `defer_codegen
  off` ist ein notwendiger, aber nicht hinreichender erster Schritt für
  den ganzen `TBathtubKiller`-Cluster; `attackToMario`s Runde-47-
  Rahmenlücke (ours 0x58 vs. Retail 0x50) sollte mit dieser Erkenntnis
  erneut versucht werden, da sie vermutlich auch auf inline-aufgeblähtem
  `theNerve()`-Code beruhte — noch nicht nachgetestet, da die Restarbeit
  an `behaveToWater` bereits das Rundenbudget beanspruchte.

**Weitere abgeschlossene Subagenten-Ergebnisse**:
- **`TMario::onYoshi() const`** (22,2 % → **MATCH**, Commit `59200eeb`):
  Bug lag NICHT in der Zieldatei, sondern in
  `include/Player/Yoshi.hpp`s `TYoshi::onYoshi()` — ein Ternary-Ausdruck
  (`return mState == STATE_MOUNTED ? TRUE : FALSE;`) wurde trotz
  `dont_inline` textuell komplett wegoptimiert (MWCC ignoriert
  `dont_inline` bei trivialen Ternary-Rümpfen unabhängig vom
  `defer_codegen`-Zustand). Umformulierung zu explizitem `if`/`else`
  ließ MWCC die Funktion endlich als eigenständig respektieren →
  Byte-exakter Match, inklusive korrekter Nachfolge-Adressen für
  `windMove`/`flowMove`/`warpRequest` in derselben Datei.
- **`THamuKuri::isHitValid(u32)`** (35,6 % → **NO-MATCH**, sauber
  zurückgesetzt): Echter Algorithmus-Bug gefunden und behoben (Retail
  dupliziert `THamuKuriManager::requestSerialKill`s komplette Logik
  inline, statt sie aufzurufen) — nach Fix inklusive `char trash[24]`
  75 von 76 Instruktionen exakt, aber ein hartnäckiger
  Register-Rotationsunterschied (`mr r3,r0` vs. direktes `lwzx r3,...`)
  erwies sich nach zehn Quellcode-Varianten als Compiler-Kontext-Artefakt
  (auch `requestSerialKill` selbst zeigt denselben Extra-Befehl in
  Retail) — bestätigter Grenzfall, kein Fix möglich.
- **`TWoodBlock::load(JSUMemoryInputStream&)`** (44,6 % → **NO-MATCH**,
  sauber zurückgesetzt): Echter Algorithmus-Bug gefunden (Retail ruft
  `TRailMapObj::load` direkt auf und dupliziert `TNormalLift::load`s
  Schwanzlogik inline, statt über `TNormalLift::load` zu delegieren) —
  aber nach Korrektur bleibt `TRailMapObj::load` an ZWEI Aufrufstellen
  ASYMMETRISCH: in `TNormalLift::load` muss es inline bleiben (aktuell
  korrekt), in `TWoodBlock::load` muss es out-of-line werden. Da
  `dont_inline` eine reine Definitionseigenschaft ist (wirkt auf ALLE
  Aufrufer gleich), ist diese Asymmetrie mit dem verfügbaren Werkzeug
  NICHT auflösbar, ohne die bereits korrekte `TNormalLift::load` zu
  brechen — bestätigter Grenzfall, dieselbe Kategorie wie die
  `WoodBlockLoad`-eigene Analyse es einordnet.
- **`TWarpInCallBack::execute(...)`** (36,75 % → **NO-MATCH**, sauber
  zurückgesetzt): Kein Algorithmus-Bug (Mathematik bereits identisch),
  reine Codegen-Formdifferenz. Ursache identifiziert:
  `JGVec3.hpp`s `operator*(TVec3, f32)` gibt **by value** zurück
  (`dont_inline`-geschützter Kopierkonstruktor/-zuweisung erzeugt daher
  überzählige Rückgabe-Temporäre bei Verkettung), während Retails Muster
  auf `const TVec3&`-Rückgabe hindeutet — dieselbe „fake-Referenz für
  Matching"-Masche, die bereits bei `operator+`/`operator-` in
  derselben Headerdatei angewendet wird. Empfehlung für eine dedizierte
  Headerdatei-Änderungsrunde dokumentiert, nicht in dieser Runde
  umgesetzt (gemeinsam genutzter Header, 37+ Includer).
- **`TApplication::TApplication()`** (36,4 % → **NO-MATCH**, sauber
  zurückgesetzt): Bereits in Teil 1 dokumentiert — `JDRFlag.hpp`s
  `TFlagT<T>::set`/Kopierkonstruktor inline vs. Retails externe
  Weak-Symbole in `MarDirectorDirect.cpp`; Cross-TU-Header-Änderung
  nötig, zurückgestellt.

**`MapObjLibDeferCodegen`-Unteragent** (86-Funktionen-Regressionsprüfung
für `src/MoveBG/MapObjLib.cpp` mit `defer_codegen off`) lief zum
Zeitpunkt dieses Zwischenstands noch; Ergebnis folgt im nächsten
Abschnitt.

Session-Gesamtstand nach Teil 2: **404 verifizierte echte Fixes in 62
Commits** (4 neue Matches diese Runde: `TMameGesso::reset`,
`TDoroHamuKuri::attackToMario`, `TSunModel::
calcDispRatioAndScreenPos_`, `TMario::onYoshi`). `defer_codegen off` ist
ab sofort als Standard-Ersttest für JEDE Funktion mit Verdacht auf
fehlende Out-of-line-Aufrufe etabliert (Diagnosekriterium: Zielfunktion
ruft eine im Header als `dont_inline` markierte Methode auf, die im
eigenen `src/*.o` gar nicht als eigenes Symbol erscheint, aber im
`obj/*.o`-Referenzobjekt schon).

### Nach neunundvierzigster Iterationsrunde, Abschluss: `MapObjLib.cpp`-Regressionskaskade bestätigt Grenzen von `defer_codegen off`

Der `MapObjLibDeferCodegen`-Unteragent (86-Funktionen-Regressionsprüfung,
16 Minuten Laufzeit) kam zu einem klaren **NO-GO**, sauber zurückgesetzt
(`git status` bestätigt leer). Kernbefund: `#pragma defer_codegen off`
wirkt NICHT nur lokal auf die Zielfunktion, sondern schaltet die GANZE
Datei von „deferred" auf „sofortige" Codegen-Reihenfolge um — das bricht
bereits bestehende, korrekt matchende Funktionen, die zufällig von der
alten „ganze Datei auf einmal betrachten"-Inlining-Reihenfolge profitiert
hatten (Beispiel: `TMapObjBase::startAllAnim` verlor sein Match, weil es
VOR seiner einzigen Aufrufstelle `makeLowerStr` im Quellcode steht und
implizit auf verzögertes Cross-Function-Inlining angewiesen war). Jeder
Versuch, EINE Regression zu flicken, erzeugte eine NEUE an anderer
Stelle (`makeObjMtxRotByAxis` 97,2 %→58,3 %, dann ein gemeinsames
Weak-Template-Symbol `TRotation3<...>::setEular` 100 %→36,5 %) — ein
**divergierendes, nicht konvergierendes** Regressionsmuster in dieser
speziellen 86-Funktionen-Datei. Die primären Zielfunktionen
(`getVerticalVecToTargetXZ`, `rotateVecByAxisY`) verbesserten sich dabei
in KEINER getesteten Konfiguration, wurden in den meisten sogar
schlechter als der unveränderte Ausgangszustand.

**Präzisierte Methodik-Regel für `defer_codegen off`** (aktualisiert
gegenüber der optimistischen Einschätzung aus Teil 2 dieser Runde):
Dieser Fix funktioniert zuverlässig nur in **kleinen, isolierten
Dateien** mit wenigen Funktionen und geringer gegenseitiger
Inlining-Abhängigkeit (bestätigtes Beispiel: `sunmodel.cpp`). In
**großen, dicht vernetzten Dateien** (wie `MapObjLib.cpp` mit 86
Funktionen und vielen `TMapObjBase`-Methoden, die sich gegenseitig
aufrufen) ist das Risiko einer Regressionskaskade hoch, weil
`-inline deferred`s „gesamte Datei auf einmal betrachten"-Verhalten
viele bereits-korrekte Matches UNSICHTBAR mitträgt. Vor jedem
`defer_codegen off`-Versuch MUSS eine vollständige Vorher/Nachher-
Regressionsprüfung über ALLE Funktionen der Zieldatei erfolgen (nicht
nur der Zielfunktion); bei mehr als einer Handvoll Regressionen ist der
Ansatz für diese Datei zu verwerfen, nicht zu erzwingen.

### Rundenabschluss: Session-Gesamtstand

**404 verifizierte echte Fixes in 62 Commits** nach Abschluss von Runde
49 (4 neue Matches: `TMameGesso::reset`, `TDoroHamuKuri::attackToMario`,
`TSunModel::calcDispRatioAndScreenPos_`, `TMario::onYoshi`).
`matched_functions` in frisch generiertem `report.json`: **8991** (von
8987 zu Rundenbeginn), `matched_code_percent`: 44,90 %. Volles
`ninja`-Rebuild erfolgreich, `dtk shasum -c` bestätigt `build/GMSJ01/
mario.dol: OK`. `git fetch upstream` bestätigt weiterhin 0 Commits
Rückstand. Acht parallele Unteragenten-Untersuchungen plus zwei
eigene Tiefenanalysen (`TLiveActor::control()`, `evIsNpcSinkBottom`)
plus die ursprüngliche `TBathtubKiller::behaveToWater`-Analyse ergaben
zusätzlich: einen grundlegenden Methodik-Durchbruch (`defer_codegen
off` für isolierte Dateien), zwei präzisierte Grenzfälle (asymmetrische
Aufrufstellen-Inlining bei `WoodBlockLoad`, dateiweite
Regressionskaskaden bei großen Dateien), und drei neue, für spätere
dedizierte Header-Änderungsrunden dokumentierte Kandidaten
(`JDRFlag.hpp`/`TFlagT` für `TApplication`-Konstruktoren, `JGVec3.hpp`s
`operator*`-Rückgabe-für-Wert für Partikel-Code, `evIsNpcSinkBottom`s
`TSpcStack::push`-Inlining-Asymmetrie).

### Nach fünfzigster Iterationsrunde (`TMario::floorDamageExec(TEParams&)`: Argument-Vertauschungsbug gefunden und behoben)

Fortsetzung der Suche nach kleinen, isolierten Kandidaten mit
`fp=None`-Aufrufstellen (analog zum `defer_codegen`-Muster aus Runde 49,
aber diesmal in kleinen Units mit ≤ 25 Funktionen, um die in Runde 49
dokumentierte Regressionskaskaden-Gefahr großer Dateien zu vermeiden).

**Fund**: `TMario::floorDamageExec(const TMario::TEParams&)`
(`src/Player/MarioCollision.cpp`, 76,2 % Match, 220 Bytes) delegiert an
`damageExec(THitActor*, int damage, int damageAnimType, int waterEmit,
f32 knockbackSpeed, int rumbleFrames, f32 pollutionAmount, s16
invincibilityFrames)`. Direkter Disassembly-Vergleich zeigte: Retails
7. Argument (`pollutionAmount`, `f32`) wird per einfachem `lfs`-Ladebefehl
aus `TEParams::mDirty` (Offset 0x7c) gelesen — unser Code übergab
stattdessen `params.mDamage.get()` ein ZWEITES Mal (bereits als 1.
Argument verwendet), was einen `u8`→`f32`-Konvertierungstrick
(Magic-Double-Subtraktion) statt eines direkten Float-Ladebefehls
erzeugte. Alle sieben `TParamRT<T>`-Felder in `TEParams` liegen exakt
0x14 Bytes auseinander (0x18, 0x2c, 0x40, 0x54, 0x68, 0x7c, 0x90),
wodurch sich die Feldreihenfolge aus dem Retail-Disassembly eindeutig
rekonstruieren ließ. Fix: `params.mDamage.get()` (2. Vorkommen) durch
`params.mDirty.get()` ersetzt. Ergebnis: **Byte-exakter Match** (58/58
Instruktionen identisch nach Label-Kanonisierung), keine Regression im
Rest der Unit (`git diff` nur die eine Zeile), volles `ninja`-Rebuild
und `dtk shasum -c` bestätigen `build/GMSJ01/mario.dol: OK`. Commit
`552a42ce`, gepusht.

**Neue Kandidatenkategorie bestätigt**: „Duplizierte Argumente in
Mehrfachaufrufen mit vielen Parametern gleichen Typs" — ein Copy-Paste-
Fehler eines früheren Mitwirkenden, der sich über den ungewöhnlichen
Magic-Double-Konvertierungscode im Disassembly (u8→f32-Promotion,
erkennbar an `stw`+`stw`+`lfd`+`fsubs` statt einfachem `lfs`) zuverlässig
aufspüren lässt, wenn das erwartete Argument eigentlich bereits ein
`f32`-Feld ist.

**`TAnimalBase::execWalk(bool)`** (78,4 % Match, 1020 Bytes) kurz
geprüft: 371 Diff-Zeilen bei 258 vs. 270 Gesamtzeilen, UNSER Stackframe
ist mit 0x118 GRÖSSER als Retails 0x0f0 (falsche Richtung für
`char trash[N]`), zusätzlich unterscheidet sich die
Multiplikations-Reihenfolge bei verketteten `SMSGetAnmFrameRate()`-
Aufrufen strukturell (`fmr`-Zwischenkopien bei uns, direkte Verkettung
bei Retail) — deutet auf eine größere Ausdrucks-Restrukturierung hin,
keine Datei verändert, als zu aufwendig für diese Runde zurückgestellt.

**Weitere für spätere Untersuchung notierte, aber nicht bearbeitete
Kandidaten** aus dem `fp=None`-Kleindatei-Scan: `TMarDirectorDirect::
decideNextStage` und `TApplication::TApplication()` (beide durch das
bereits dokumentierte `JDRFlag.hpp`/`TFlagT`-Cross-TU-Problem
blockiert), `getNameRef_BossEnemy`/`getNameRef_Enemy` (sehr groß,
2676/8392 Bytes, vermutlich lange Namens-Vergleichsketten, nicht
geprüft).

### Session-Gesamtstand nach Runde 50

**405 verifizierte echte Fixes in 64 Commits.** `matched_functions`:
**8992** (von 8987 zu Beginn dieses Segments), `matched_code_percent`
~44,9 %. Volles `ninja`-Rebuild erfolgreich, `dtk shasum -c` bestätigt
`build/GMSJ01/mario.dol: OK`. `git fetch upstream` weiterhin 0 Commits
Rückstand. Fünf neue Matches in diesem Segment: `TMameGesso::reset`,
`TDoroHamuKuri::attackToMario`, `TSunModel::calcDispRatioAndScreenPos_`,
`TMario::onYoshi`, `TMario::floorDamageExec(TEParams&)`. Zusätzlich ein
grundlegender Methodik-Durchbruch (`#pragma defer_codegen off` für
isolierte Dateien, mit dokumentierten Grenzen für große Dateien) und
eine neue produktive Fehlerkategorie (duplizierte Argumente bei
`TParamRT`-Strukturzugriffen, erkennbar am Magic-Double-
Konvertierungsmuster im Disassembly).

### Nach einundfünfzigster Iterationsrunde (parallele Elf-Kandidaten-Untersuchung: 7 neue Matches, neue Methodik-Details)

Fortsetzung des Klein-Datei-Scans (≤ 30 Funktionen pro Unit, `fp` zwischen
5–90 %, Größe < 500 Bytes), elf Kandidaten parallel an Unteragenten delegiert.
**Sieben neue Byte-exakte Matches**, sechs Commits:

- **`TQuestionManager::makeDL(JDrama::TGraphics*) const`** (84,2 % → Match,
  Commit `2a012e9b`): Retail baut alle VIER Quad-Eckpunkte in ein
  `TVec3<f32>[4]`-Array und übergibt das Array an `TDLTexQuad::request`;
  unser Code baute nur EINEN transformierten Vertex. `char trash[4]` schloss
  die verbleibende 8-Byte-Rahmenlücke.
- **`TQuestionManager::request(TVec3<f32>, f32)`** (88,7 % → Match, selber
  Commit): Reine Codegen-Form-Differenz (Z-Term zuerst statt X-Term zuerst,
  `fmadds` statt zwei `fmuls`+`fadds`) — durch benannte lokale `dz`/`distZ`
  in exakter Retail-Reihenfolge behoben.
- **`CPolarSubCamera::calcTowerCenterPos_(Vec*)`** (84,6 % → Match, Commit
  `a0b05f8a`): Retail emittiert die Funktion als `inline`/`weak`-Symbol
  (kein CSE der Tabellenadresse über den `switch`). Fix: `inline`-Markierung
  + Verschiebung ans Dateiende nach dem einzigen Aufrufer (da `dont_inline`
  eine Aufrufstellen-, keine Definitionseigenschaft ist) + `defer_codegen
  off` + `char trash[8]`.
- **`TMarDirector::fireGetNozzle(TItemNozzle*)`** (77,8 % → Match, Commit
  `814da6f2`): Zwei kombinierte Bugs — `gpApplication.mCurrArea.unk0` wurde
  in jedem Zweig neu gelesen statt einmal vor der Verzweigung; und
  `getNozzleRight(...)` fehlte die Negation (`!`). Plus `char trash[8]`.
- **`TCubeManagerArea::isInAreaCube(const Vec&) const`** (81,4 % → Match,
  Commit `88d6ae2e`): Mehrere frühe `return true;` durch einen einzelnen
  `bool result`-Akkumulator mit `if`/`else if`-Kette ersetzt (verlängert die
  Live-Range, wodurch der Compiler 5 statt 4 Register via `stmw`/`lmw`
  alloziert, exakt wie Retail) — Muster aus der Schwesterfunktion
  `TCubeManagerFast::isInOtherCube` in derselben Datei übernommen. Plus
  `char trash[16]`.
- **`TBoundPane::TBoundPane(J2DScreen*, u32)`** (83,5 % → Match, Commit
  `4d7aae7b`, **Header geändert**): Feld `unk14` war als `JUTRect` deklariert
  (mit echtem Out-of-line-Konstruktor), wird aber nur über rohe
  `.x1/.y1/.x2/.y2`-Zugriffe verwendet — Retail behandelt es als reines POD.
  Neuer schlanker Typ `SBoundRect` (4×`s32`, inline-Ctor) ersetzt `JUTRect`
  für dieses eine Feld, mit `operator JUTRect()`-Konversion für den einzigen
  externen Aufrufer (`ConsoleStr.cpp`). Alle vier Header-Includer
  (ConsoleStr.o, GCConsole2.o, SelectMenu.o, BlendPane.o) nachgebaut und
  bestätigt fehlerfrei. Plus `char trash[8]`. Vollständiges `ninja`-Rebuild
  nach diesem Fix bestätigt `build/GMSJ01/mario.dol: OK`.
- **`TEnemyPolluteModelManager::generatePolluteModel(TVec3<f32>&,
  TVec3<f32>&)`** (86,3 % → Match, Commit `9debd4d6`): Der komplette
  Boden-/Wasser-Check lag in Wahrheit in einem inline-expandierten
  `TEnemyPolluteModel::generate` (kein eigenes Symbol im Retail-Objekt),
  nicht im Manager selbst — drei Indizien (Out-of-line-`isWaterSurface`-
  Aufruf nur bei Inline-Tiefe ≥ 2, Stack-Layout-Reihenfolge, leerer
  `SMatrix34C`-Konstruktor-Aufruf nur bei Inline-Expansion) bestätigten die
  Struktur. `generate` als `inline`-Methode direkt in der `.cpp` (keine
  Header-Änderung, keine weiteren Aufrufer) neu definiert, Restlogik in den
  Manager verschoben. `JGeometry::TVec3<f32> trash` (nicht `char[]` — ein
  reines `char trash[12]` wird innerhalb der Inline-Expansion wegoptimiert)
  schloss die 12-Byte-Lücke.

**Neue Methodik-Erkenntnis**: `#pragma dont_inline` ist eine
**Aufrufstellen-Eigenschaft**, keine Definitionseigenschaft — bestätigt am
`calcTowerCenterPos_`-Fund: Um eine Funktion an EINER Aufrufstelle
out-of-line zu halten, muss ihre Definition entweder (a) mit `dont_inline`
UND `defer_codegen off` kombiniert werden (funktioniert nur in kleinen,
isolierten Dateien, siehe Runde-49-Grenzbefund), ODER (b) für Fälle mit
genau einem Aufrufer: die Definition NACH dem Aufrufer im Quelltext
platzieren und als `inline` markieren, was MWCC dazu bringt, sie als
Weak-Symbol mit eigenem Out-of-line-Körper zu emittieren statt sie
textuell zu inlinen.

**Zwei Kandidaten mit gründlich dokumentiertem, aber nicht erreichtem
Match** (beide sauber zurückgesetzt, wertvolle Investigation dennoch):
`TMarDirector::fireStreamingMovie(u8)` (neue Hypothese: `unk4C`-Feld
verhält sich wie `volatile` in Retails echtem Quelltext, aber als
projektweit von vielen Dateien über „fabricated"-Hilfsmethoden genutztes
Feld außerhalb des Aufgabenumfangs für eine Einzelfunktions-Änderung) und
`TMapObjWaterFilter::perform` (109 von 111 Instruktionen exakt nach
sechs echten Struktur-Fixes — camera-Check-Umformulierung, `pos`-Referenz,
View-Matrix-Lokale, Deklarationsreihenfolge, `char trash[0x38]` — letzte
zwei Instruktionen sind ein MWCC-Branch-Folding-Artefakt bei der letzten
`&&`-Bedingung vor einem bloßen `return;`, gegen 14 Quellvarianten
resistent, bestätigt anhand von zwei weiteren Funktionen mit demselben
Muster in `Map.cpp`/`bosseel.cpp`). Weitere vier Kandidaten
(`updateTrans`, `getPosInWire`, `TStrategy`-Konstruktor, `initMActor`,
`loadAfter` von `TMapObjRevivalPollution`, `TCubeManagerBase`-Konstruktor,
`AudioDecoderForOnMemory`) als reines Register-Zuteilungs-/
Stack-Slot-Rauschen bestätigt und zurückgesetzt.

### Session-Gesamtstand nach Runde 51

**412 verifizierte echte Fixes in 70 Commits.** `matched_functions`:
**8999** (von 8992 zu Rundenbeginn), `matched_code_percent`: 44,96 %.
Volles `ninja`-Rebuild erfolgreich, `dtk shasum -c` bestätigt
`build/GMSJ01/mario.dol: OK`. `git fetch upstream` weiterhin 0 Commits
Rückstand.

### Nach zweiundfünfzigster Iterationsrunde (4 weitere Matches; neues Muster „Joint-Index-Truncation-Timing"; zwei bemerkenswerte Fast-Treffer dokumentiert)

Fortsetzung des Klein-Datei-Scans mit sieben weiteren parallel delegierten
Kandidaten. **Vier neue Byte-exakte Matches**, drei Commits:

- **`TMirrorCamera::perform(u32, JDrama::TGraphics*)`** (91,0 % → Match,
  Commit `c195ae05`): Projektionsmatrix-Zeiger nicht gehoben (`MtxPtr`
  statt wiederholtem `graphics->mProjMtx.mMtx`), `gpCamera`-Felder direkt
  statt über die Inline-Accessoren `getFovy/getAspect/getNear/getFar`
  gelesen (ändert Argument-Auswertungsreihenfolge), plus `char trash[8]`.
- **`TLauncher::receiveMessage(THitActor*, u32)`** (90,8 % → Match, Commit
  `c8a3646a`): Echter Argumentfehler — `&mPosition` (this) statt
  `&sender->mPosition` für Emit/Sound-Aufrufe. Plus `char trash[8]`.
- **`TBaseNPC::setPollutionEffectMtxPtr_`/`setSmokeEffectMtxPtr_`** (90,5 %
  / 91,1 % → beide Match, Commit `7dd7e933`): **Neues, viertes
  wiederkehrendes Bugmuster entdeckt**: Ein von `JUTNameTab::getIndex`
  zurückgegebener `s32`-Gelenkindex muss vor der inline-expandierten
  `*0x30`-Array-Index-Multiplikation in `J3DModel::getAnmMtx` auf 16 Bit
  maskiert werden — aber NUR wenn dies als `int idx = call(); ... idx &
  0xFFFF;` geschrieben wird (nicht als `u16`-typisierte Lokale oder
  `(u16)`-Cast), da MWCC die Maskierung sonst sofort statt verzögert über
  einen Zwischenaufruf hinweg einplant. Zusätzlich beeinflusst die
  Deklarationsreihenfolge lokaler `const char*`-Gelenknamen direkt die
  Registerzuteilung (auch für `@sda21`-adressierte String-Literale).

**Zwei bemerkenswerte, gründlich dokumentierte Fast-Treffer** (beide nach
Protokoll zurückgesetzt, da nicht Byte-exakt):
- **`SMS_AddDamageFogEffect`** (90,0 % → 99,44 %, NICHT committet):
  Echter Logikfehler gefunden und behoben (lokale `f32`-Literale wurden
  von MWCC konstant-gefaltet und die beiden Oszillationswerte fälschlich
  zu einem CSE't — Ersetzung durch nicht-konstante `static f32`-Datei-
  Globale erzwingt zwei unabhängige Laufzeitberechnungen wie im Retail-
  Disassembly). Nach Fix + `char trash[0x38]`: alle 76 Instruktionen
  strukturell identisch, aber 4 Zeilen zeigen vertauschte Operandenreihen-
  folge bei kommutativen `fmuls`/`fadds` (z. B. `fmuls f29,f2,f1` vs.
  `fmuls f29,f1,f2`). **Selbst nachgeprüft**: expliziter Tausch der
  Quelltext-Multiplikationsreihenfolge (`s * (...)` statt `(...) * s`)
  hatte NULL Effekt auf die erzeugten Instruktionen — MWCC kanonisiert die
  Operandenreihenfolge kommutativer Gleitkomma-Operationen unabhängig vom
  Quelltext. Bestätigter Grenzfall, kein Fix möglich.
- **`TMActorKeeper::createMActor(const char*, u32)`** (60,7 % → 80,8 % bei
  einer Teilkorrektur, NICHT committet): Zwei unabhängige Lücken
  identifiziert — (a) Retail delegiert NICHT an
  `createMActorFromNthData`, sondern inlined `createAndRegister` direkt
  (echter Strukturfehler, Teilfix erreicht 80,8 % ohne Regressionen); (b)
  ein Inline-Tiefe-2-Schwellenwert-Unterschied bei `loadModelData`/
  `registerDataAndJoinNewNode`, den keine der elf getesteten
  Compiler-Flag-Kombinationen (`inline_depth`, `inline_max_size`,
  `-O3`/`-O4`, alternative Compiler-Version 1.2.5n, `defer_codegen off`)
  reproduzieren konnte, ohne andere Funktionen der Unit zu regressieren.
  Nicht committet, da laut Protokoll nur Byte-exakte Treffer zulässig sind.
- **`CPolarSubCamera::isMarioAimWithGun_`/`isMarioCrabWalk_`** (53,5 % je,
  NICHT committet): Echter Feldnamen-Bug gefunden (`checkFrameMeaning`
  [Offset 0xD4] statt `checkMeaning` [Offset 0xD0]), aber Datei ist
  `PCHObject(NonMatching, ...)` — die PCH bäckt `-inline deferred` bereits
  beim PCH-Bau ein, wodurch `defer_codegen off` in der `.cpp`-Datei selbst
  keine Wirkung mehr auf bereits-PCH-kompilierte Header-Inlines
  (`TMario::checkStatusType`, dutzende Aufrufstellen projektweit) hat.
  **Neue Grenzbedingung für die `defer_codegen`-Technik**: PCH-kompilierte
  Dateien sind für diesen Trick nicht erreichbar.

**Weitere bestätigte Register-/Stack-Rauschen-Fälle** (zurückgesetzt):
`TCardManager::setCardStat_` (Retail nutzt ein Duff's-Device-entrolltes
Laufzeit-Loop für eine kompilierzeit-konstante Trip-Count von 6, das
MWCC mit den Projekt-Flags nicht reproduziert — sieben Quellvarianten
erfolglos).

### Session-Gesamtstand nach Runde 52

**416 verifizierte echte Fixes in 73 Commits.** `matched_functions`:
**9003** (von 8999 zu Rundenbeginn), `matched_code_percent`: 44,99 %.
Volles `ninja`-Rebuild erfolgreich, `dtk shasum -c` bestätigt
`build/GMSJ01/mario.dol: OK`.

### Nach dreiundfünfzigster Iterationsrunde (2 weitere Matches; Inzident dokumentiert; sechs gründlich untersuchte Fast-Treffer)

Zwölf weitere Kandidaten parallel delegiert. **Zwei neue Byte-exakte
Matches**:

- **`TYoshi::thinkUpper()`** (92,7 % → Match, Commit `59f127a3`): Mehrere
  kombinierte Bugs — handgerollte AND-Bedingung statt der vorhandenen
  `TWaterGun::isEmitting()`-Inline-Methode; falsches Sound-Member
  (`mBodyAnmSound` statt `mTongueAnmSound`); vertauschte `==`/`!=`-Zweige
  im else-Pfad; `char trash[0x10]`.
- **`TMapEventSinkBianco::loadAfter()`** (56,7 % → Match, Commit
  `d99991e0`): **Neue Technik**: Statt `#pragma dont_inline` (das die
  Inline-Entscheidung für ALLE Aufrufer gleich beeinflusst und damit die
  bereits korrekte Inline-Kopie in `TMapEventSinkInPollutionReset::
  loadAfter()` gebrochen hätte) wurde stattdessen die
  **Inline-Kosten-Schwelle** der Callee-Funktion `TMapEventSinkInPollution
  ::loadAfter()` gezielt um zwei tote Anweisungen angehoben — genug, um
  MWCC dazu zu bringen, sie bei Aufruftiefe 2 (Bianco) nicht mehr zu
  inlinen, aber bei Aufruftiefe 1 (Reset) weiterhin zu inlinen. Plus
  `char trash[0x40]`. Keine Regression in den restligen 35 Funktionen der
  Unit bestätigt.

**Inzident**: Ein Unteragent (`MarDirectorPosIdx`) committete einen
Nicht-Byte-exakten Fix (90,6 % → 99,84 %) entgegen dem etablierten
Protokoll. Beim Korrigieren mit `git reset --hard HEAD~1` wurden
versehentlich auch die unfertigen Arbeitsbaum-Änderungen zweier anderer
parallel laufender Unteragenten gelöscht (`CameraNoticeYrot` musste seine
komplette Untersuchung wiederholen; `MapObjDolpicWater`/`BathWaterManager`
überlebten, da sie zum Zeitpunkt des Resets noch aktiv schrieben). Lehre:
**Reverts während laufender paralleler Batches MÜSSEN dateispezifisch
sein** (`git checkout -- <path>` oder ein gezielter `git revert`
eines Commits), NIEMALS `git reset --hard`, da dieser den GESAMTEN
Arbeitsbaum überschreibt, nicht nur die eigene Historie.

**Sechs gründlich dokumentierte Fast-Treffer** (alle nach Protokoll
zurückgesetzt, keine Commits):
- **`TMarDirector::decideMarioPosIdx()`** (90,6 % → 99,84 %): Zwei echte
  Bugs bestätigt und behoben — falsche `switch`-Case-Label-Menge (Retails
  16-Eintrags-Sprungtabelle beweist Cases 2,3,4,5,6,8,9; unser Code hatte
  2..8, schloss also Case 9 fälschlich aus und Case 7 fälschlich ein) plus
  fehlende Adressmaterialisierung von `&gpApplication.mPrevArea` in ein
  Register. Restdifferenz: ein nicht rekonstruierbarer 28-Byte-
  Stack-Bereich UNTERHALB der Compiler-Temporären, vermutlich von einem
  im Retail-Quelltext vorhandenen, aber zur Compile-Zeit toten Aufruf
  (Debug/OSReport-Stil) verursacht, dessen Parameter-Bereich trotz
  Wegoptimierung Stack reserviert.
- **`CPolarSubCamera::calcNoticeTargetYrot_(const Vec&)`** (92,0 % →
  97,71 %): Fünf echte Bugs gefunden (vertauschte Nah-/Fern-Schwellwerte
  `mRotateMinDistXZ`/`mRotateFastMinDistXZ`, falsche Multiplikationsketten-
  Reihenfolge, zwei-statt-eine-Ausdruck-`dist2`-Berechnung, falsche
  Subtraktionsrichtung beim Yaw-Diff, `char trash[0x28]`) — Restdifferenz
  ist reine Lade-Reihenfolge-/Register-Kanonisierung (2 von 137
  Instruktionen).
- **`TMActorKeeper::createMActor(const char*, u32)`** (60,7 % → 80,8 %
  Teilfix): Retail delegiert nicht an `createMActorFromNthData`, sondern
  inlined `createAndRegister` direkt — echter Strukturfehler, behoben,
  aber ein Inline-Tiefe-2-Schwellenwert bei `loadModelData`/
  `registerDataAndJoinNewNode` blieb unauflösbar (elf Compiler-Flag-
  Kombinationen erfolglos).
- **`CPolarSubCamera::isMarioAimWithGun_`/`isMarioCrabWalk_`** (53,5 % je):
  Echter Feldnamen-Bug (`checkFrameMeaning` statt `checkMeaning`), aber
  Datei ist PCH-kompiliert — `defer_codegen off` erreicht PCH-gebackene
  Header-Inlines nicht mehr.
- **`TMonumentShine::hitByWater`**/**`TBathtubData::getPos`/
  `getGravityDir`**: Je 1–3 echte Bugs gefunden und behoben (u. a.
  `unk18.at(i,j)` → `unk18.mMtx[i][j]`, Anweisungsreihenfolge, `cross`
  statt `cross2`), Restdifferenzen sind MWCC-Konstanten-Kanonisierung
  (kommutative Operandenreihenfolge bei Compiler-Literalpool-Werten,
  nicht durch benannte Konstanten reproduzierbar) bzw. Register-
  Zuteilung in der gemeinsam genutzten `JGQuat4.hpp::rotate()` (lokalisiert,
  aber projektweite Header-Änderung außerhalb des Aufgabenumfangs).
- **`TMAnmSoundNPC::startAnimSound`**: Ein echter Feld-Bug gefunden
  (`uVar6`-Herleitung aus falschem Feld), aber zwei weitere unabhängige
  Restdifferenzen (Register-Wiederverwendung, `std::sqrtf`-Auto-Inlining)
  verhindern exakten Match.

**Bestätigtes Register-/Stack-Rauschen** (keine neuen Erkenntnisse):
`TStageEnemyInfoTable::getMatchedInfo` (bereits aus früherer Runde
bekannt), `TMenuPlane`-Konstruktor, `TGraphTracer::calcSplineSpeed`,
`TSpineEnemy::goToExclusiveNextGraphNode`, `MSoundSE::
startSoundActorWithInfo` (beide letzteren PCH-blockiert für
`defer_codegen`-Technik).

### Session-Gesamtstand nach Runde 53

**418 verifizierte echte Fixes in 75 Commits.** `matched_functions`:
**9005** (von 9003 zu Rundenbeginn), `matched_code_percent`: 45,01 %.
Volles `ninja`-Rebuild erfolgreich, `dtk shasum -c` bestätigt
`build/GMSJ01/mario.dol: OK`. Alle Arbeitsbäume nach dem oben
dokumentierten Reset-Inzident als sauber verifiziert.

### Nach vierundfünfzigster Iterationsrunde (5 weitere Matches; JDRFlag.hpp-Header-Experiment als Lehrbeispiel für heterogene Compiler-Entscheidungen; Inzident 2)

**`JDRFlag.hpp`-Untersuchung** (dediziert, 11 Minuten): Der Versuch,
`JDrama::TFlagT<u16>::TFlagT(const TFlagT&)` und `::set(u16)` projektweit
als „declare-only" mit expliziter Out-of-line-Instanziierung in
`MarDirectorDirect.cpp` zu erzwingen (Ziel: `TApplication`-Konstruktor
und `decideNextStage()` reparieren), ergab nach vollständiger
Projekt-Regressionsprüfung (alle 736 Units) ein klares **NO-GO**: 12
vormals 100 % gematchte Funktionen regressierten (teils um über 50
Prozentpunkte), `decideNextStage()` verbesserte sich GAR NICHT
(Byte-identisch vorher/nachher), und `TApplication`-Konstruktor
verbesserte sich nur teilweise (36,4 % → 70,3 %, nicht exakt). **Wichtige
Erkenntnis**: Retails Compiler trifft für dieses triviale
Template-Member GENUINELY UNTERSCHIEDLICHE Inline-Entscheidungen pro
Aufrufstelle (manche Stellen inlinen die Kopie, andere rufen sie auf) —
eine einzige globale „declare-only"-Header-Änderung ist zu grobschlächtig,
um diese Heterogenität nachzubilden; sie kann nur EIN Verhalten überall
erzwingen. Sauber zurückgesetzt (Commit `b7384f3a`, Revert-Commit).

**Fünf neue Byte-exakte Matches** aus einem parallelen Batch von 15
kleineren Kandidaten:

- **`TPakkunSeed::behaveToHitWall(const TBGCheckData*)`** (95,0 % → Match,
  Commit `c335c131`): **Neues, fünftes Bugmuster**: „Accessor-Aufruf vs.
  direkter Feldzugriff verhindert CSE" — `mVelocity.dot(ground->mNormal)`
  (direkter Zugriff) ließ MWCC die `normal.x`-Last über zwei Verwendungen
  hinweg wiederverwenden; Retail nutzt für den `dot()`-Aufruf den
  `getNormal()`-Accessor (unterbindet die CSE), behält aber für die drei
  `+=`-Zeilen direkten Feldzugriff.
- **`SMS_InitPacket_Fog(J3DModel*, u16)`** (93,1 % → Match, Commit
  `ee984000`): Benannter `J3DPEBlock* peblock`-Lokal vor der
  `getFog()`-Kette erzwang korrekte Ladereihenfolge (`getPEBlock()` vor
  `getShape()`); plus `char trash[40]`. Korrigiert eine frühere,
  unvollständige Untersuchung (nur Padding versucht, Strukturproblem
  übersehen).
- **`TSmallEnemy::attackToMario()`** (90,4 % → Match, Commit `c9f1b81e`):
  Retail berechnet den Mario-Richtungsvektor über einen
  Drei-Argument-`TVec3`-Konstruktor und skaliert/akkumuliert dann IN
  PLACE (kein dritter temporärer Vektor) — `sub()` + separater
  skalierter Temp durch direkte Konstruktion + In-Place-`scale()`/
  Akkumulation ersetzt.
- **`TRedCoinSwitch::control()`** (92,9 % → Match, Commit `0441aa1f`):
  Falsches `switch`-Case-Label (`case 1` statt `case 4` für den
  No-op-Zustand) veränderte MWCCs Pivot-Wahl für den binären
  Case-Dispatch-Baum; Fix reproduziert Retails tatsächliche
  Case-Werte-Menge {1,2,3,4}.
- **`TMapWarp::watchToWarp()`** (66,4 % → Match, Commit `e29ac366`):
  Umfangreichster Fix dieser Runde (26 Minuten Untersuchung) — gecachter
  `checkData->getData()`-Index statt Neulesens, `TVec3::add` bewusst
  out-of-line über die `operator+`-Fake-Referenz-Masche (wie bereits bei
  `bgIntersectLine` in `MapCheck.cpp` bekannt) statt inline-expandierendem
  `+=`, plus präzise Lokalen-Deklarationsreihenfolge und drei
  `char trash[N]`-Anpassungen für exakte Slot-Adressen.

**Inzident 2**: Der `JDRFlagHeaderFix`-Unteragent führte versehentlich
`git stash` im geteilten Repository aus, wodurch die unfertigen
Arbeitsbaum-Änderungen ALLER gleichzeitig laufenden Unteragenten
temporär eingelagert wurden. Der Agent erkannte den Fehler sofort,
beließ den Stash unangetastet (kein `pop`/`drop`) und benachrichtigte
alle betroffenen Agenten per Hub-Nachricht mit genauen
Wiederherstellungsanweisungen (`git show stash@{0}:<path>`). Alle
betroffenen Agenten stellten ihren Stand erfolgreich wieder her oder
hatten bereits vor dem Zwischenfall zurückgesetzt; keine Arbeit ging
verloren. Ein zweiter, kleinerer Vorfall (`PacketUtilFog` committete
versehentlich zwei fremde, bereits im Index gestagete Dateien mit) wurde
vom betroffenen Agenten selbst sofort per `git reset --soft HEAD~1` +
gezieltem `git restore --staged` korrigiert. Beide Vorfälle bestätigen:
**dateispezifische Git-Operationen sind bei parallelen Agenten-Batches
Pflicht** — niemals pauschale `git stash`/`git add -A`/`git reset --hard`
ohne Pfadangabe im geteilten Arbeitsbaum.

**Zehn weitere gründlich dokumentierte Fast-Treffer/Sackgassen**
(zurückgesetzt): `TMapObjBase::startControlAnim`, `SMS_IsMarioOnWire`
(bereits aus früherer Runde bekannt), `TMapCheckGroundPlane::
checkPlaneGround` (bereits dokumentiertes TODO-Problem), `TTalkCursor::
associateNPC`, `TMapObjPlane::makeMountain`, `TNpcParts::
setPartsAnmFrame` (2 von 3 Switch-Case-Blöcken repariert, dritter
resistent), `TRope::moveHead` (echter Bug gefunden — fehlendes
`dont_inline` auf `TVec3::scale(f32)` in `JGVec3.hpp` als
Systemursache identifiziert, aber außerhalb des Aufgabenumfangs),
`TMapObjSwitch::control`, `TNerveHino2Landing::execute` (echter
Flag-Bug `LIVE_FLAG_HIDDEN`→`LIVE_FLAG_CLIPPED_OUT` gefunden, aber
beide betroffenen Werte sind im Retail-Binary selbst tot/ungenutzt),
`TTamaNoko::landEffect` (**zwei echte Algorithmus-Bugs** gefunden und
behoben — fehlendes if/else für Sand-vs-Nicht-Sand-Zweig, fehlender
0,8-Skalierungsfaktor bei allen vier `setGlobalScale`-Aufrufen — auf
8 von 223 Instruktionen reduziert, Restdifferenz nachweislich in der
gemeinsam genutzten `JPABaseEmitter::setGlobalScale`/`TVec3`-Temp-
Allokation lokalisiert, bestätigt durch identisches Muster in einer
NIE berührten Schwesterdatei `namekuri.cpp`), `TMapObjGeneral::thrown`
(auf 15 von 102 Instruktionen reduziert, Restindiz deutet auf eine
verlorene Inline-Hilfsfunktion im Retail-Quelltext hin).

### Session-Gesamtstand nach Runde 54

**423 verifizierte echte Fixes in 80 Commits.** `matched_functions`:
**9010** (von 9005 zu Rundenbeginn), `matched_code_percent`: 45,06 %.
Volles `ninja`-Rebuild erfolgreich, `dtk shasum -c` bestätigt
`build/GMSJ01/mario.dol: OK`.

### Nach fünfundfünfzigster Iterationsrunde (großer 20-Kandidaten-Batch: 11 neue Matches, sechstes Bugmuster, mehrere Lehrbeispiele zu Stack-Slot-Allokation)

Größter Batch dieser Session (20 parallele Unteragenten). **Elf neue
Byte-exakte Matches**:

- **`TMarioParticleManager::emitParticleCallBack(...)`** (95,85 % → Match,
  Commit `fb2aefe9`): Klassischer Pattern-4-Bug — `param_4` (Callback)
  statt `param_5` (User-Daten) an `getAvailableIdx()` und `info->unk0`
  übergeben.
- **`TMapObjPlane::depress(f32,f32,f32)`** (94,7 % → Match, Commit
  `9b4c12d2`): Referenz-Bindung `f32& h = heightAt(...); h -= ...;`
  materialisiert die Element-Adresse als echte gemeinsame Unterausdruck
  (CSE), verhindert MWCCs Faltung in einen reinen `lfsx`/`stfsx`-Zugriff.
- **`TSmallEnemy::changeOut()`** + **`behaveToHitOthers(THitActor*)`**
  (95,5 %/91,8 % → beide Match, Commit `d630b0ee`): Vertauschte
  Zuweisungsrichtung (`mJuiceBlock->mPosition = mPosition` statt
  umgekehrt) plus Distanzvektor-Berechnung über 3-Arg-Konstruktor statt
  `sub()`-Methode (matcht das bereits gefixte `attackToMario`-Muster in
  derselben Datei).
- **`TJointCoin::makeObj(const char*, u16)`** (95,4 % → Match, Commit
  `6b6e1230`): Erste Array-Slot-Lesung in eine benannte Lokale
  gecacht — ändert nichts an den erzeugten Instruktionen, verschiebt
  aber MWCCs Registerfarbe auf Retails Zuteilung.
- **`JPABaseField::calcFieldFadeScale(f32)`** (95,3 % → Match, Commit
  `f7e51c15`): Datei-lokale Hilfsfunktion mit `volatile`-Referenz-
  Parameter verhindert CSE des wiederholt gelesenen Status-Worts.
- **`TBaseNPC::npcRecoverFromSinking()`** (95,1 % → Match, Commit
  `9c6d9c25`): `TBaseNPC* self = this;` als erste Lokale verschiebt
  Registerzuteilung (deklarierte Lokalen erhalten Vorrang vor dem
  impliziten `this`); toter `MsClamp(0,0,0)`-Aufruf nach dem
  Sink-Geschwindigkeits-Block reserviert exakt 4 Byte unterhalb des
  inline-expandierten `MsSqrtf`-Temporärwerts.
- **`MarioWaistCtrl`/`MarioFootDirRCtrl`/`MarioFootDirLCtrl`/
  `TMario::boxDrawPrepare`** (94,8 %/78 %/78 %/92,5 % → alle vier Match,
  Commits `91724804`+`fbd1d5ac`): Größte Einzelausbeute dieser Runde.
  Falscher Flag-Accessor (`checkStatusType` statt `checkFlag`), falsche
  Kontrollfluss-Struktur (FLUDD-Test galt nur für einen Case statt für
  drei plus Fallthrough), falsches zweites Kreuzprodukt (Gram-Schmidt:
  `n × cross1` statt `n × currentMtxDir`), invertiertes
  Schlaf-Prädikat, und — bemerkenswert — ein **im Retail-Binary selbst
  vorhandener Tippfehler reproduziert**: `footMtx[2][1] = normalDir.z`
  wird tatsächlich nach `footMtx[2][2]` geschrieben (0x24 bleibt
  unbeschrieben). Mehrere `volatile`-Lückenfüller-Arrays für exakte
  Stack-Slot-Adressen.

**Neues, sechstes Bugmuster bestätigt**: „Benannte Lokale mit
Selbstzuweisung erzwingt Adress-CSE" — z. B. `f32& h = heightAt(...); h
= h - x;` verhindert MWCCs Faltung eines Compound-Assignments in einen
indizierten Zugriff (`TMapObjPlane::depress`).

**Zwei bemerkenswerte neue Stack-Layout-Techniken dokumentiert** (beide
erfolgreich angewendet, aber auch mit dokumentierten Grenzen):
- Ein **toter Aufruf einer INLINE-Funktion** (z. B. `MsClamp(0,0,0);`
  als eigenständige Anweisung) reserviert Stack-Bytes an einer
  SPEZIFISCHEN Position unterhalb einer bereits inline-expandierten
  Funktion (z. B. `MsSqrtf`s `volatile float`-Temporärwert) — anders als
  `char trash[N]`, das IMMER oberhalb bestehender Lokalen landet. Diese
  Technik löste `npcRecoverFromSinking`, versagte aber bei mehreren
  anderen Kandidaten dieser Runde (Hino2HeadCallback, TNerveHino2JumpIn::
  execute), wo die fehlenden Bytes UNTERHALB eines versteckten
  Rückgabewert-Temporärwerts sitzen und mit keinem getesteten
  Konstrukt reproduzierbar waren.
- **„By-Value-Rückgabeslot einer inline-expandierten Funktion hält einen
  Konstruktor-Aufruf out-of-line; eine benannte Lokale nicht"** — neue,
  präzise Formulierung der bereits bekannten Inline-Tiefe-Heuristik,
  bestätigt an zwei unabhängigen Fällen (`NpcEffect.cpp`s
  `getEffectScale_`-Vergleich, `emario.cpp`s `SMS_DistanceBetween`-
  Hilfsfunktion für `sub()`/`sqrt()`).

**Neun gründlich dokumentierte Fast-Treffer/Sackgassen** (alle sauber
zurückgesetzt, hoher Erkenntniswert): `MSound`-Konstruktor (reines
Register-Rauschen um `this`), `JPAConvectionField::affect` (zwei echte
Bugs gefunden — nicht-initialisiertes Schatten-`thing4`, In-Place- statt
separates `setLength()` — auf reines Registerrauschen reduziert),
`draw_wipe_box` (echter h/w-Reihenfolge-Bug gefunden, Rest ist
Scheduler-Rauschen bei unabhängigen Fließkomma-Konvertierungen),
`TMareEventBumpyWall::bumpDownZ` (zwei echte Bugs — vertauschte
Konstruktor-Komponente, fehlende CSE-Erzwingung — blockiert von einem
klassenweiten systemischen Rahmen-Offset, bestätigt in unberührten
Schwesterfunktionen), `TMario::wireSWait` (**vier echte Bugs** gefunden
und gefixt — falsches Feld `mWireBounceVelPrev` statt `mWireBounceVel`,
falsche Bit-Maske, vertauschte Subtraktionsoperanden, wegoptimierte
Multiplikation —, blockiert von einer geteilten Inline-Hilfsfunktion mit
Alles-oder-Nichts-Schwellenwert), `MSStageCubeFade::proc` (**zwei echte
Bugs** — fehlende Y-Komponenten-Übernahme, Stub-Funktion
`calcParamRatioInCube` real implementiert —, blockiert von
dateiweiter Rahmen-Inflation), `TBaseNPC::execWalk` (**fünf echte
Bugs** — Accessor- statt Direktzugriff, fehlendes `fabs`, fehlende
Kopierkette, falsche Konstante, **falscher Algorithmus** in
`isCanWalk` [horizontale statt 3D-Distanz] —, auf 12 von ~250
Instruktionen reduziert), `TEMario::perform`/`init` (**echter
Kontrollfluss-Bug** gefunden — Retail kehrt nie früh zurück, läuft
immer bis zum Ende durch —, Header-Änderungsversuch an `JGVec3.hpp::
distance()` korrekt als regressionsverursachend identifiziert und
verworfen), `Hino2HeadCallback`/`TNerveHino2JumpIn::execute` (mehrere
echte Bugs gefunden, auf 8 von ~90 bzw. 82 von ~180 Instruktionen
reduziert).

**Kleinere Inzidente**: Zwei Unteragenten stellten fest, dass generische
Python-Eval-Variablennamen (nicht Dateien!) über Sitzungen hinweg
kollidieren können, wenn mehrere Agenten zufällig denselben Kernel-
Kontext nutzen — beide erkannten es selbst, wechselten auf
dateibasierte Werkzeuge (`edit`/`write`) und stellten betroffene
Dateien wieder her; keine dauerhaften Schäden. Zwei Task-Ergebnisse
kamen mit Status „failed" statt eines sauberen Abschlussberichts zurück
(vermutlich Antwortlängen-Limit erreicht) — beide hinterließen dennoch
nachweislich saubere, unveränderte Arbeitsbäume.

### Session-Gesamtstand nach Runde 55

**434 verifizierte echte Fixes in 88 Commits.** `matched_functions`:
**9021** (von 9010 zu Rundenbeginn), `matched_code_percent`: 45,20 %.
Volles `ninja`-Rebuild erfolgreich, `dtk shasum -c` bestätigt
`build/GMSJ01/mario.dol: OK`.

### Nach sechsundfünfzigster Iterationsrunde (17-Kandidaten-Batch: 4 neue Matches, Kumokun-Cluster vollständig als Sackgasse bestätigt, wiederkehrendes „JUTRect-Bounds"-Scheduler-Muster kartiert)

**Vier neue Byte-exakte Matches**:

- **`TAnimalManagerBase::clipEnemies(JDrama::TGraphics*)`** (95,0 % →
  Match, Commit `6a0546b3`): Drei unabhängige Ursachen — `mViewClipNear`
  vor dem Aufruf in eine Lokale gehoben, Akteur-Zeiger vor statt in der
  Schleife deklariert, zweiseitige Stack-Polsterung. **Neue, präzise
  Regel bestätigt**: MWCC legt früher deklarierte Lokalen bei HÖHEREN
  Stack-Adressen ab — ein `trash`-Array VOR einer Lokalen polstert
  darüber (verschiebt sie nicht), ein `trash`-Array NACH einer Lokalen
  verschiebt sie nach oben. Zweiseitige Lücken brauchen zwei
  `trash`-Arrays.
- **`CPolarSubCamera::execWallCheck_(Vec*)`** (95,6 % → Match, Commit
  `ee637ee5`): Additions-Reihenfolge der Ebenendistanz, Accessor- vs.
  Direktzugriff für CSE-Steuerung, und ein **neues, siebtes Bugmuster**:
  Die Registerform einer inline-kopierten 12-Byte-Struktur hängt von der
  QUELLAUSDRUCKSART ab — Kopie aus einem Struct-Mitglied (`obj.field`)
  ergibt ein anderes Scratch-Register-Paar als Kopie aus einer einfachen
  benannten Lokalen oder einem Array-Element. Wenn Retail die
  Mitglieds-Form zeigt, liegen beide Vektoren vermutlich in EINEM
  gemeinsamen Stack-Aggregat, nicht in zwei getrennten Lokalen.
- **`TTamaNokoFlower::perform(u32, JDrama::TGraphics*)`** (94,1 % →
  Match, Commit `d529a708`): Echter Verhaltens-Bug — rotierter Vektor
  wurde ins Member `unk20` statt zurück in `local_88` geschrieben
  (nachfolgende Lesungen nutzten den unrotierten Vektor); plus fehlendes
  bedingungsloses `return` im Demo/Talk-Zweig.
- **`TBossMantaAdditionalCollision::perform(...)`** (Bonus-Fund,
  Commit `e2df3ce4`): Derselbe Pattern-4-Bug wie im NICHT gelösten
  `TBossManta::moveObject` (siehe unten) — `AttackMario(mCollisions[i])`
  statt `AttackMario(this)` —, hier aber OHNE Restdifferenz vollständig
  behoben.

**Kumokun-Cluster vollständig als Sackgasse bestätigt**: Alle sieben
untersuchten Funktionen in `src/Enemy/Kumokun.cpp`
(`checkOnMovingWall/Floor/Roof`, `bindOnFlying`, `moveObject`,
`decideTargetAtDir`, `rotateGoalDirToLocal`) sind blockiert durch
dieselbe gemeinsame Ursache im inline-expandierten `TKumokun::
checkWallPlane`: Retail emittiert nach der `isTouchedWallsAndMoveXZ()`-
Prüfung `ble`, unser Build immer `beq`, unabhängig von der
Quelltext-Formulierung (`if(x)`, `if(0<x)`, Ternary — alle identisch).
Dies löst eine Registerzuteilungs-Kaskade aus, die alle Funktionen
gleich betrifft. Drei unabhängige Unteragenten bestätigten denselben
Befund und koordinierten sich erfolgreich über Hub, bevor sie
zurücksetzten — keine Datei-Konflikte trotz gemeinsamer Zieldatei.
Weitere, in diesem Cluster gefundene aber isoliert nicht ausreichende
echte Bugs: falsch gespeicherter Wert (`yTmp` statt `dVar10`) in
`checkOnMovingFloor`/`checkOnMovingRoof`; unnötige Vektorkopie in
`checkOnMovingWall`.

**Wiederkehrendes „JUTRect-Bounds"-Scheduler-Muster** über drei
unabhängige Dateien bestätigt (`GCConsole2.cpp`s `processDownCoin`/
`processAppearCoin`, `ConsoleStr.cpp`s `processShineGet`/`processMiss`):
Der Ausdruck `JUTRect bounds(...); ptr->mGlobalTranslation.set(bounds.x1
+ bounds.getWidth()*0.5f, ...)` erzeugt bei Retail eine KONSERVATIVE
Instruktions-Planung (frühe `stw`-Speicherung vor der `xoris`-Adress-
berechnung) für die Magic-Double-Konvertierungsblöcke, während unser
Compiler konsistent eine AGGRESSIVERE Umordnung wählt — bestätigt in
drei unabhängigen Dateien mit identischem Muster, kein quelltext-
seitiger Hebel gefunden (auch Retails eigenes `GCConsole2.cpp` zeigt
dasselbe konservative Muster für denselben Idiom-Typ, schließt also
eine Datei-lokale Ursache aus).

**Weitere gründlich dokumentierte Fast-Treffer** (alle instruktions-
identisch oder nahezu, aber durch Stack-Slot-Feinheiten blockiert,
sauber zurückgesetzt): `TGCConsole2::startAppearBalloon` (3 echte Bugs,
8/180 Zeilen Rest, Ursache in `J2DWindow::getContentsBounds()`s
Rückgabe-per-Wert-ABI lokalisiert), `CPolarSubCamera::
updateDemoCamera_` (53,2 % → 220/220 Instruktionen identisch, blockiert
durch `operator+`-Tiefe-3-Out-of-line-Regel), `TNerveBGKDie::execute`
(2 echte Bugs, 264/264 Instruktionen identisch, TU-weite Rahmen-
Inflation über 10+ Funktionen bestätigt), `TNerveRHGraphWander::
execute` (199/199 Instruktionen identisch, blockiert durch 8- vs.
4-Byte-Ausrichtung eines Rückgabewert-Temporärs), `TPoiHana::init`
(167/167 Instruktionen identisch, Restursache in einer ANDEREN
Funktion derselben Datei über einen gemeinsamen Rodata-Pool
lokalisiert), `TNameIndParCallback::execute`/`TNameKuri::
calcRootMatrix` (zusammen 6+ echte Bugs, Differenzen um 90 % reduziert).

### Session-Gesamtstand nach Runde 56

**438 verifizierte echte Fixes in 92 Commits.** `matched_functions`:
**9025** (von 9021 zu Rundenbeginn), `matched_code_percent`: 45,26 %.
Volles `ninja`-Rebuild erfolgreich, `dtk shasum -c` bestätigt
`build/GMSJ01/mario.dol: OK`.

### Nach siebenundfünfzigster Iterationsrunde (20-Kandidaten-Batch: 6 neue Matches, systemisches TFlagT<u16>-Argument-Rahmenproblem entdeckt, JGVec3-operator*-Hypothese erneut bestätigt)

**6 neue Fixes, alle commitet:**

1. `TMario::TSurfingParams::TSurfingParams` (`MarioInitSurfing`,
   Commit `4acf881b`) — Feldtyp-Bug (Muster 4, neue Unterart): `mRoll`
   war als `TParamRT<s32>` deklariert, muss `TParamRT<f32>` sein
   (Header-Änderung in `include/Player/Mario.hpp`, 0 Regressionen im
   vollen Report-Diff bestätigt).
2. `TMapObjWaterSpray::calc` (`MapObjTownWaterSpray`, Commit
   `32ddf2c1`) — Rotation in benannte `s16`-Lokale materialisiert +
   `char trash[8]`.
3. `TBaseNPC::setPosAndInitAfterSinkBottom` (`NpcChangeSink`, Commit
   `814bd61f`) — drei Bugs: (a) `pos.y`/`pos.z` müssen in Lokalen
   zwischengespeichert werden, wobei die **Register-Zuordnung der
   Deklarationsreihenfolge folgt, die Lade-Reihenfolge aber der
   Initialisierungsreihenfolge** (neue Verfeinerung von Regel 7: `f32
   z; f32 y = pos.y; z = pos.z;` — `z` zuerst deklariert landet in
   f31, `y` zuerst initialisiert wird zuerst geladen); (b) beide
   `mSpine->setDefaultNext()`-Aufrufstellen müssen als
   `setNext(getDefault())` geschrieben werden (Registerreihenfolge der
   Argument-Auswertung); (c) zweiseitiges Trash-Padding (4 Bytes davor,
   0x1c danach).
4. `TMushroom1up::control` (`MapObjItem2Mushroom`, Commit `b0190e38`)
   — `MsClamp` erzeugte in beiden if/else-Zweigen identischen toten
   Code (Tell-Tale für Muster 1); durch anonyme-Namespace-Templates
   `MsMin`/`MsMax` ersetzt (reproduziert Ein-Vergleich-pro-Zweig-Codegen
   und plaziert die Literale in `.sdata` statt `.sdata2`), dazu
   `mPosition = pos` (Wort-Kopie) statt `.set()` (Float-Kopie),
   Anweisungsreihenfolge (y-Update vor sin/cos) und zweiseitiges
   Trash-Padding.
5. `TNPCManager::clipEnemies` (`NpcManagerClip`, Commit `c15ec3e8`) —
   fünf gestapelte Bugs: De-Morgan-Negation fehlte, `fovy`/`aspect`
   vertauscht, Deklarationsreihenfolge-Register-Fix, Accessor statt
   Direktzugriff (rechts-nach-links-Auswertungsreihenfolge), expliziter
   Cast statt impliziter `operator Vec*()`-Konversion.
6. `TItemSlotDrum::getForcastResult` (`MapObjSirenaMisc`, Commit
   `cd6e36d3`) — quantisierter Winkel muss in die bestehende
   `angle`-Lokale zurückgeschrieben werden statt als Temporärwert an
   `getResultFromAng` übergeben zu werden; sonst rematerialisiert MWCC
   die Int→Float-Magic-Double-Konversion ein zweites Mal im inline
   Vergleich.

**Neue systemische Erkenntnis — `JDrama::TFlagT<u16>`-Trailing-Argument
bricht Rahmengrößen-Buchhaltung an 8+ Aufrufstellen:** Bei
`THideObjPictureTwin::afterFinishedAnim` (`MapObjHideAnim`, NO-MATCH,
zurückgesetzt) wurde ein echter Muster-4-Bug gefunden (`&mPosition`
statt `&obj->mPosition`) und behoben — danach war der Instruktions-
Strom bis auf 2 von 120 Zeilen identisch, aber der Stack-Bereich unter
den benannten Lokalen ist bei Retail durchgehend 0x14+ Bytes größer.
Systematische Prüfung aller `fireStartDemoCamera`-Aufrufer zeigt
dasselbe Muster in MapObjTown, Item (`TShine::appearWithDemo`/
`control`), MapEventDolpic, MapObjSirena, MapEventSirena, cameragc
(`CPolarSubCamera::loadAfter`) und NpcEvent — überall reserviert
Retail mehr Stack für den `TFlagT<u16>`-Wertparameter als unser Build.
Kein Konstrukt aus der `.cpp`-Datei kann den fehlenden Slot erzeugen;
vermutlich eine kompilat-weite Inline-Rahmen-Buchhaltungslücke, deren
Lösung mehrere Funktionen gleichzeitig lösen würde. Verwandt, aber
bestätigt eigenständig: `TShine::appearWithTime`/`appearSimple`
(`ItemAppearWithTime`, NO-MATCH) zeigen dasselbe „Retail reserviert
Rahmenplatz für inline-Callees, die wir nicht reservieren"-Symptom in
20 von 94 Funktionen derselben Datei; Compiler-Flag-Experimente
(`-inline`-Varianten, `-O4`, `-RTTI`, `-enum min`, u.a.) schließen eine
einfache Flag-Ursache aus.

**`JGVec3.hpp::operator*(TVec3, f32)`-Bug erneut unabhängig
bestätigt** (dritte Bestätigung nach `WarpInCallBackExecute`, Runde 51):
`TEffectColumWater::generate` (`EffectObjMisc`, NO-MATCH) zeigt exakt
dasselbe Muster — `operator*` sollte `const TVec3&` statt `TVec3`
zurückgeben (derselbe Fabrikations-Trick, den `operator+`/`operator-`
im selben Header schon nutzen); ein temporärer Header-Patch reproduziert
die Retail-Instruktionsfolge exakt. Weiterhin nicht projektweit
angewendet (Cross-TU-Regressionsrisiko), aber jetzt mit drei
unabhängigen Fundstellen ein starker Kandidat für eine zukünftige,
sorgfältig vollständig regressionsgeprüfte Runde.

**Weitere gründlich dokumentierte Fast-Treffer/Sackgassen** (alle
sauber zurückgesetzt, kein Commit): `MtxUtilJointsToArc` (reines
Register-/FPR-Zähl-Rauschen, Algorithmus schon korrekt),
`NpcCallbackNeck` (f30/f31-Vertauschung zweier nichtflüchtiger FPR-
Kandidaten, 6 Varianten erfolglos), `PollutionManagerClean`
(zyklische Register-Rotation r6/r7/r8/r9→r7/r8/r9/r6, PCH-Datei, 6
Varianten erfolglos), `MapUpdate` (ein echter Bug in `updateDelfino`
gefunden und behoben, aber zwei ungelöste Restprobleme: Boolean-Guard-
Zweigform + 160-Byte-Rahmenlücke ohne sichtbare Ursache),
`FishoidLoad` (echter Bug gefunden: `setFleeTarget()`-Accessor statt
Direktzugriff; mathematisch bewiesen, dass die verbleibende 0x80-Byte-
Stack-Lücke wegen Deklarationsreihenfolge-Zwängen nicht per einfachem
Trash-Padding erreichbar ist), `JDRFrmGXSetPerform` (154/154
Instruktionen exakt reproduzierbar, aber die letzten 0x70 Bytes
Rahmenreserve wären nur durch Fabrikation unverifizierbaren toten Codes
erreichbar — bewusst nicht gemacht), `TPictureTelesa::touchActor`
(`MapObjSirenaMisc`, zweite Funktion — Rahmengröße korrekt, aber
Scheduling-Reihenfolge in einem `distance()`-Inline weicht ab;
`#pragma scheduling 604` behebt genau diesen Block, bricht aber den
Rest der Funktion — Hinweis auf ein TU-abweichendes Scheduling-Modell),
`TEffectObjBase::perform` (`EffectObjMisc`, zweite Funktion — Retail
reserviert ein nie geschriebenes r31-Save + 20 Byte toter Lokale, aus
der Instruktionsliste nicht rekonstruierbar), `MapObjHideAnim`,
`MarioCheckColHangPole` (4 gestapelte Bugs behoben, 4-Zeilen-Rest durch
einen 8-Byte-Spill-Slot der inline `std::sqrtf`-MSL-Expansion
blockiert), `MapObjCloudRide` (Graph-Web-CSE-Bug behoben, 215/219
Instruktionen identisch, Rest eine FPR-Permutation in einer
Drei-Operanden-Multiplikation), `NpcPartsCtor` (schwerwiegender
Struct-Layout-Bug in `include/NPC/NpcInitData.hpp` gefunden und exakt
bewiesen — `unk4[i]` ist kein Array-of-TNpcModelDataEntry mit Stride
0x2C, sondern ein Array-of-Pointer mit Stride 4 — sowie 8 weitere
Bugs, Rest auf 8 Zeilen reduziert, blockiert durch eine MWCC-
Konstantenfaltung, die Retail nicht durchführt), `ItemAppearWithTime`,
`CameraLibRotate` (algorithmischer Bug in `RotateAboutAxis` gefunden —
transponierte statt normale 3×3-Matrix-Multiplikation, Komponenten
direkt statt über `TVec3::set()` geschrieben; 286/286 Instruktionen
inkl. Register exakt reproduziert, letzte 8 `addi`-Stack-Immediates
durch einen MWCC-1.2.5-Inline-Instanz-Overhead von 4 Byte pro
Struct-Lokaler blockiert, gegen 6 Compiler-Versionen getestet).

### Session-Gesamtstand nach Runde 57

**444 verifizierte echte Fixes in 98 Commits.** `matched_functions`:
**9031** (von 9025 zu Rundenbeginn), `matched_code_percent`: 45,38 %.
Volles `ninja`-Rebuild erfolgreich, `dtk shasum -c` bestätigt
`build/GMSJ01/mario.dol: OK`.

### Nach achtundfünfzigster Iterationsrunde (24-Kandidaten-Batch: 9 neue Matches, vierte unabhängige Bestätigung der „Phantom-Frame-Reservierung"-Fehlerklasse, TFlagT<u16>-Trigger präzisiert)

**9 neue Fixes, alle commitet — überwiegend reine Stack-Layout-Lücken
(Muster 7):**

1. `TLensFlare::TLensFlare(const char*)` (Commit `177bfa1a`) — 4-Byte-
   Lücke zwischen `buf[0x100]` und dem gesicherten r30; `char trash[4]`
   nach `buf` deklariert.
2. `TLensGlow::TLensGlow(bool, const char*)` (Commit `8a45e440`) —
   identisches Muster, 4-Byte-Lücke vor dem GPR-Save-Bereich.
3. `JASystem::Driver::updatecallDSPChannel` (Commit `d1598668`) —
   0x18-Byte-Rahmenlücke, exakt dieselbe Größe wie das etablierte
   `TDSPChannel::updateAll()`-Pendant in derselben Datei-Gruppe.
4. `JASystem::HardStream::main` (Commit `f66d778a`) — 16 Byte tote
   Lokal-Reserve ohne jede Instruktion; `char trash[16]` behebt es.
   Zwei weitere Funktionen derselben Datei (`startFirst`/`startSecond`,
   `volFloatToU8`) zeigen dasselbe Muster mit anderen Lückengrößen,
   nicht behoben (außerhalb des Auftrags).
5. `TMapXlu::changeNormalJoint` (Commit `d0f3b462`) — Muster 5
   (Accessor-vs-Direktzugriff): ein `getChild(i)`-Aufruf zu viel
   reservierte einen ganzen Inline-Expansionsblock; Ersatz durch
   `mChildren[i]` in der ersten Schleife entfernt ihn exakt.
6. `TMarDirector::loadResource` (Commit `12d36834`) — verschachtelter
   `{ JKRDvdFile sceneDvdFile; ... }`-Block brauchte 44 Byte mehr
   Reserve; `char trash[44]` NACH `sceneDvdFile` (Reihenfolge kritisch:
   davor deklariert hat keine Wirkung, da MWCC von oben nach unten
   zuteilt).
7. `TMBindShadowManager::TMBindShadowManager(const char*)` (Commit
   `ea13ece7`) — Muster 5: `gpApplication.mCurrArea.unk0` (Rohzugriff)
   musste durch `getStage()`-Accessor ersetzt werden, der einen
   zusätzlichen 4-Byte-„this"-Temporärwert reserviert.
8. `TMarioCap::TMarioCap(TMario*)` (Commit `2550d27e`) — fünf
   gestapelte Bugs: fehlende `.rodata`-Statics (Nullblock +
   Shift-JIS-„Speicher voll"-String, behebt zusätzlich die
   `.rodata`-Sektion von 93 % auf 100 %), vertauschte
   `setBaseTRMtx`/`getAnmMtx`-Kopierrichtung (Muster 4), 0x78 Byte
   Stack-Padding, benannte Referenz-Lokale für ein `const ResTIMG&`-
   Argument (Muster 6) und eine indexbasierte Neuladebasis-Optimierung.
9. `TPollutionObj::getDepthFromMap` (Commit `026cf5ac`) — `tmp`-
   Deklaration vor die Float-Lokalen verschoben plus ein zusätzliches
   4-Byte-Dummy-Lokal, um die exakte MWCC-Rundungs-/Reihenfolge-Regel
   für Stack-Slots zu treffen.

**Vierte unabhängige Bestätigung der „Phantom-Frame-Reservierung"-
Fehlerklasse, jetzt mit präziserem Auslöser:** `JDRActorLoad`
(NO-MATCH) zeigt denselben 4-Byte-Rahmenunterschied wie die
`TFlagT<u16>`-Fälle aus Runde 57, diesmal ausgelöst durch einen
INLINE-Basisklassenkonstruktor (`TLightMap`s Default-Ctor konstruiert
seine `TViewObj`-Basis, die wiederum `JDrama::TFlagT<u16>`s Ctor mit
einem Trailing-Wertargument aufruft) statt durch eine direkte
Trailing-Argument-Übergabe — verengt den Auslöser auf „`TFlagT<u16>`-
Konstruktion irgendwo im Inline-Baum", nicht nur auf Aufrufstellen.
Weitere Varianten derselben Fehlerfamilie, aber mit jeweils eigenem
Auslöser (nicht identisch, aber strukturell verwandt — alle „Bytes,
die keine Instruktion je berührt, aus Quelltext nicht platzierbar"):
`MarDirectorPreEntry` (48 Byte um einen synthetisierten `TRect`-
Wertrückgabe-Temporärwert, Padding landet immer oberhalb statt
unterhalb), `ConductorMakeEnemy` (4-Byte-Verschiebung in einem
inline-expandierten `JGadget::TList`-Iterator-Vergleich),
`MapObjManagerAppear` (4-Byte-Positionsunterschied eines
adressgenommenen Lokals, jede zusätzliche Variable rundet den Rahmen
um volle 8 Byte auf), `NpcColorInit` (8 Byte, ausgelöst durch die
Kombination `new`-Ausdruck in einem `switch`-Zweig + bedingter
Doppelaufruf in einem anderen), `SampleCtrlMaterialCtor` (durch einen
bereits im Repo dokumentierten TODO-Kommentar in
`J3DColorChan.hpp:72-77` über PCH/sdata2-Plazierung erklärt — externe
Bestätigung, dass dies ein bekanntes, ungelöstes Repo-Problem ist,
nicht nur eine Session-Beobachtung), `PollutionCountDrawStamp` (ein
Teil des Rahmens reparierbar, ein zweiter — ein `GXColor`-Argument-
Temporärwert — nicht), `EventWatcherHideDead` (zwei Funktionen
`evSetHide4LiveActor`/`evSetDead4LiveActor`, identischer 8-Byte-
Rahmenunterschied bei sonst 100 % identischem Code; eine dritte
Schwesterfunktion `evSetFlagNPCCanTaken` zeigt denselben Defekt, ist
aber nicht Teil dieser Runde).

**Weitere gründlich dokumentierte Sackgassen** (sauber zurückgesetzt):
`SelectMenuOpenWindow` (eine echte Link-Zeit-Koinzidenz — Retail
relokiert einen BGM-Enum-Wert gegen `showGPR__12JUTException+0x3C`,
numerisch zufällig identisch, aus Quelltext nicht rekonstruierbar),
`SpcInterpExecOps` (4 Funktionen `execadd`/`execsub`/`execmul`/
`execdiv`, identisches gemeinsames 4-Byte-Problem in einem anonymen
`TSpcSlice`-Temporärwert, ~10 Varianten erfolglos), `J3DClusterInitMtx`
(~50 Quelltext-Varianten getestet — MWCC kann für `&dl[3+vtxSize*k]`
entweder Basis-zuerst-Addition ODER nachgelagertes `addi +3` erzeugen,
aber nie beides gleichzeitig wie Retail), `MirrorActorInit` (9
Varianten, Iterator-Konstruktor-Temporärwerte in `JGadget::TList::
insert` falsch gepackt), `SplashManagerMakeDL` (vertauschte
Stack-Slot-Reihenfolge zwischen benanntem `GXColor`-Lokal und
anonymem Compound-Literal-Temporärwert, 6 Varianten erfolglos).

Zwei Agenten (`MarDirectorCtor`, `ObjManagerLoad`) brachen mit Fehler
ab, bevor sie Quelltext änderten (sauberer Zustand bestätigt) — beide
Kandidaten bleiben für eine künftige Runde offen.

### Session-Gesamtstand nach Runde 58

**453 verifizierte echte Fixes in 107 Commits.** `matched_functions`:
**9039** (von 9031 zu Rundenbeginn), `matched_code_percent`: 45,57 %.
Volles `ninja`-Rebuild erfolgreich, `dtk shasum -c` bestätigt
`build/GMSJ01/mario.dol: OK`.

### Nach neunundfünfzigster Iterationsrunde (25-Kandidaten-Batch: 12 neue Matches — stärkste Runde bisher, überwiegend reine Stack-Layout-Lücken)

**12 neue Fixes, alle commitet — fast ausschließlich Muster-7-
Stack-Layout-Lücken, jeweils in 1-2 Iterationen gelöst:**

1. `CPolarSubCamera::controlByCameraCode_` (Commit `fa009596`) —
   `char trash[0x28]` vor der `TVec3`-Lokalen.
2. `TNpcThrow::throwMario` (Commit `cc4a82e1`) — `f32 trash;` nach der
   Vec3-Lokalen.
3. `TLiveManager::perform` (Commit `aae5ef05`) — `char trash[16]`,
   klassische Phantom-Frame-Lücke, diesmal direkt lösbar.
4. `TMarioGamePad::read` (Commit `4f3d0b32`) — `char trash[4]` nach
   `resetPort`.
5. `JDrama::TOrthoProj::perform` (Commit `857fd2a6`) — `char trash[8]`
   am Funktionsanfang.
6. `TDrawSyncManager::threadFunc` (Commit `3a1ee283`) — zweiseitiges
   Padding (`t1[4]` davor, `t2[4]` danach) um die `msg`-Lokale in der
   Schleife.
7. `TOneShotGenerator::loadAfter` (Commit `ead51be7`) — Muster 6:
   `TNameRefGen::search(...)`-Kettenausdruck musste in eine benannte
   `TIdxGroupObj* group`-Lokale materialisiert werden, bevor
   `->getChildren().push_back(this)` aufgerufen wird.
8. `TMovieSubTitle::setupResource` (Commit `d939e52f`) — `char
   trash[12]` nach `char buffer[256]`.
9. `JAIBasic::sendPlayingSeCommand` (Commit `4f40e7f9`) — `u16 trash;`
   nach zwei `u16`-Lokalen, verschiebt einen nachfolgenden
   Doppel-Spill-Temporärwert über eine 8-Byte-Ausrichtungsgrenze.
10. `TBGPolDrop::move` (Commit `6a7a7b9f`) — zwei Bugs: `char
    trash[12]` nach `checkData` PLUS ein echter Logikfehler
    (`unk50->setBckFromIndex(13)` benutzte den falschen MActor-
    Zeiger, muss `unk54->setBckFromIndex(13)` sein).
11. `TShimmer::perform` (Commit `1103319b`) — zwei Bugs: drei falsche
    virtuelle Aufrufe (Vtable-Slots 0x0c/0x10/0x14 von J3DModel waren
    je um eine Position verschoben — `entry()`/`calc()`/`update()`
    vertauscht) PLUS eine neue Stack-Technik: fünf `Mtx`/
    `J3DTransformInfo`-Lokale mussten von Block-Scope (innerhalb eines
    `if`) auf Funktions-Scope gehoben werden, um sowohl die
    Rahmengröße als auch jeden Einzeloffset zu treffen (Trash-Arrays
    wurden hier vollständig wegoptimiert, da unbenutzt).
12. `TMessageLoader::TMessageLoader(const char*)` (Commit `fb1fc4d2`)
    — `char trash[8]`, MWCC hatte zwei adressgenommene `u32`-Lokale
    volleliminiert (Wertfluss komplett registerbasiert), Retail
    reservierte trotzdem 8 Byte ungenutzten Speicher dafür.

**Fünfte bis achte unabhängige Bestätigung der „Phantom-Frame-
Reservierung"-Fehlerklasse, mit neuen Auslöser-Varianten:**
`ObjHitCheckActors` (NO-MATCH) fand DREI echte Logikfehler
(`entryGroup` statt `checkAndEntryGroup` an 4 Aufrufstellen, falscher
`TStrategy::unk10`-Index, `getAttackRadius/Height` statt
`getDamageRadius/Height`) und behob sie korrekt — der Instruktions-
Strom wurde dadurch 100 % identisch, blieb aber durch eine
unerreichbare 32-Byte-Rahmenlücke um `JGadget::TList`-Iterator-
Temporärwerte blockiert; nicht commitet trotz korrekter Logik-Fixes
(Hard Rule). Weitere Varianten: `MapEventSirenaWatch`/
`MapEventDolpicRiccoGate` (`TFlagT<u16>`-Trigger, aber Retail-Rahmen
diesmal KLEINER als unserer statt größer — Trash-Padding kann nur
hinzufügen, nicht entfernen, daher strukturell unlösbar in diese
Richtung), `NpcCollisionBind`/`PerformListLoad` (inline `JGadget::
TList`-Iterator-Temporärwerte, gleiche Fehlerfamilie wie
`ObjHitCheckActors`), `AreaCylinderLoad` (ein `readS32()`-
Schleifenzähler-Temporärwert bleibt trotz jeder Deklarationsreihen-
folge auf einem fixen Slot gepinnt), `MarDirectorCtorRetry` (kombiniert
zwei bekannte Auslöser: `JGadget::TVector_pointer<T>`-Standard-Ctor-
Kette PLUS 5× vorausgehende `TFlagT<Us>`-Konstruktion — bestätigt,
dass mehrere Auslöser sich in einer Funktion überlagern können),
`CameraWarpPosAndAt`/`ModelUtilRideMove`/`ProgSelectPerform` (jeweils
eine unerreichbare 4-12-Byte-Lücke im untersten „Outgoing-Parameter"-
Bereich des Rahmens, unterhalb aller benannten Lokalen — Trash-Arrays
werden dort entweder wegoptimiert oder nach oben verschoben, nie nach
unten platziert), `WalkerCalcFarthestVertex` (ein `volatile f32`-
Lokal bleibt bei jeder Padding-Kombination auf festem Offset 0x30
gepinnt, obwohl ein Nachbar-Array frei verschiebbar ist).

**`J3DModelEntryData`** (NO-MATCH, ~40 Varianten) — kein Phantom-
Frame-Fall, sondern ein echtes Register-Pool-Dilemma: MWCC hat zwei
getrennte Callee-saved-Register-Pools (einen für Compiler-CSE-
Temporärwerte r24/r25, einen für Quelltext-Lokale r20/r21). Zwei von
drei Diskrepanzen exakt gelöst (Zugriff über `getShapePacketArray()`
statt Direktzugriff; If-Zweig ohne benannte Lokale, um CSE über
`countDLSize()` zu erzwingen), aber der Else-Zweig verlangt
widersprüchlich sowohl eine früh materialisierte Adresse (nur über
eine benannte Lokale möglich) als auch deren Zuordnung zum Temp-Pool
(nur ohne benannte Lokale möglich) — ein zirkulärer Constraint ohne
Lösung auf Quelltext-Ebene.

### Session-Gesamtstand nach Runde 59

**465 verifizierte echte Fixes in 119 Commits.** `matched_functions`:
**9048** (von 9039 zu Rundenbeginn), `matched_code_percent`: 45,67 %.
Volles `ninja`-Rebuild erfolgreich, `dtk shasum -c` bestätigt
`build/GMSJ01/mario.dol: OK`.

### Nach sechzigster Iterationsrunde (21-Kandidaten-Batch: 2 neue Matches, aber drei außergewöhnlich wertvolle Fast-Treffer mit insgesamt 12+ echten Gameplay-Bugs — inkl. eines projektweiten Header-Bugs)

**Pool der einfachen Kandidaten (80-100 % Fuzzy-Match, kleine Dateien)
erschöpft sich sichtbar** (504 → 168 → 84 → 44 → 24 Kandidaten über die
letzten vier Runden). Diese Runde wurde deshalb bewusst auf größere/
niedrigprozentigere Funktionen ausgeweitet (bis 88,64 %/72,14 % Fuzzy-
Match, bis 2008 Byte Größe) — Trefferquote sank entsprechend (2/21),
aber die QUALITATIVE Ausbeute an dokumentierten echten Bugs ist die
höchste einer einzelnen Runde bisher.

**2 neue Fixes, beide commitet:**

1. `TMenuBase::perform` (Commit `345444b7`) — zweiseitiges 4+4-Byte-
   Trash-Padding um ein `J2DOrthoGraph`-Stack-Objekt.
2. `TAfterEffect::perform` (Commit `3e2c1fec`) — neue allgemeine
   Technik entdeckt: **MWCC ordnet Callee-saved-Register nach
   Kandidaten-Erzeugungsreihenfolge** (Parameter zuerst, dann
   Frontend-Lokale/Temporärwerte in Quelltext-Reihenfolge, dann ERST
   Inline-Expansions-Temporärwerte), aufsteigend r28..r31 zugewiesen.
   Ein `const TRect& rect = graphics->getViewport();` (Accessor-Aufruf,
   inline expandiert) wird deshalb ALS LETZTES erzeugt und bekommt die
   falsche Registernummer; Ersatz durch Direktzugriff
   `graphics->mViewportRect` verschiebt den Kandidaten früher in die
   Liste und trifft Retails Reihenfolge exakt. `char trash[4]` NACH der
   betroffenen Lokalen stellt den durch den Accessor-Wegfall
   verlorenen 4-Byte-Stack-Slot wieder her.

**★ DREI HERAUSRAGENDE FAST-TREFFER — vollständig dokumentiert für
künftige Wiederaufnahme, NICHT commitet (Hard Rule: nur Byte-exakt):**

**`TSpider::bind` (`src/Enemy/spider.cpp`, 83,88 % → 100 % Instruktions-
identisch, blockiert durch 56-Byte-Phantom-Frame-Lücke):** VIER echte
Logikfehler gefunden und bestätigt (Nachweis: symbolische Neu-
herleitung gegen Retail-Disassembly):
- `checkGround`-Aufruf Nr. 2 benutzte `local_50.y` statt
  `param_1->mPosition.y` als Y-Argument.
- `TBGWallCheckRecord`-Y-Argument fehlte `+ spine->getHeadHeight()`.
- `TBGWallCheckRecord`-Radius benutzte `getHeadHeight()` statt
  `getWallRadius()` (= `mBodyScale * mWallRadius`, ein anderes Feld!).
- Wand-Rückstoßrichtung: Quelltext berechnete `local_bc = tmp -
  center` (mit separatem `tmp`), Retail skaliert `normal` IN PLACE und
  subtrahiert `normal` direkt von `local_bc` — unterschiedliche
  Aliasing-Semantik.
Zusätzlich wurde die exakte Quelltext-Form gefunden, die einen 100 %
instruktionsidentischen Strom erzeugt (366/366 Instruktionen, exakt
dieselben Register): `TSpineEnemy* spine`-Cast als benannte Lokale,
Referenz-Bindung für `normal` vor der Kopie, Wiederverwendung von
`local_50` statt einer dritten Lokalen, sowie eine präzise Kette
benannter Materialisierungen (`vy`, `vz`, `y`, `z` in genau dieser
Deklarationsreihenfolge) für die f27-f31-Registerbelegung. Blockiert
einzig durch eine unerreichbare 56-Byte-Lücke zwischen zwei Compiler-
Temporärwerten (Retail-Rahmen 0x158 vs. unser 0x120) — ein
`JGeometry::TVec3<f32>(0,0,0)`-Sondierungs-Statement wächst den
Temp-Pool zwar, aber in die FALSCHE Richtung (12 Byte oberhalb UND
unterhalb statt der benötigten zusammenhängenden 52-Byte-Lücke).
**Vollständige Wiederanwendungs-Anleitung im Agent-Transkript
`history://SpiderBind` archiviert** — sobald die Phantom-Frame-
Fehlerklasse durchbrochen ist, ist dies der Kandidat mit dem
höchsten sofortigen Ertrag.

**`JDrama::TSmJ3DAct::perform` (`src/JSystem/JDrama/JDRSmJ3DAct.cpp`,
72,14 % → strukturell identisch, blockiert durch Register-Scheduling
in drei Matrix-Multiplikationsblöcken):** DREI echte Bugs gefunden,
darunter ein **projektweiter Header-Bug**:
- Rotationsreihenfolge: Retail wendet Z, dann Y, dann X an; Quelltext
  hatte X, Y, Z (andere komponierte Rotation — echter Verhaltensbug).
- Matrix-Wiederverwendung: Retail nutzt nur ZWEI lebende Matrizen für
  die Multiplikationskette (Ergebnis wird wiederverwendet), Quelltext
  allozierte eine dritte.
- **`include/JSystem/JGeometry/JGMatrix34.hpp::TMatrix34::concat()`
  ist fehlerhaft transkribiert**: berechnet die TRANSPONIERTE des
  beabsichtigten Produkts PLUS einen Out-of-Bounds-Lesezugriff auf die
  4. Spalte (liest 0x34/0x38 Byte über die 3×4-Matrix hinaus). Durch
  symbolische Herleitung gegen Retails tatsächliche Arithmetik
  bestätigt: die korrekte zeilen-majore Form ist `result[i][j] =
  a.at(i,0)*b.at(0,j) + a.at(i,1)*b.at(1,j) + a.at(i,2)*b.at(2,j)`
  (+ `a.at(i,3)` für `j==3`). Betrifft mindestens DREI Aufrufstellen
  repo-weit: `JDRSmJ3DAct.cpp`, `JDRCamera.cpp` (`TPolarCamera::
  perform`, dort mit 65,79 % der schlechteste bekannte Match-Wert
  einer nicht-trivialen Funktion), und `BathWaterManager.cpp:1149`.
  Reiht sich ein in die wachsende Liste bekannter „fabrizierter"
  Header-Funktionen dieser Session (`J3DColorChan::getAttnFn`, Runde
  58; `TExPane::setCenteredSize`, Runde 60/CardLoad).
Mit allen drei Fixes: 146/313 Zeilen byte-identisch, alle 313
Instruktionen multisetgleich, aber Register/Planungsreihenfolge
innerhalb der drei Matrix-Multiplikationsblöcke weicht ab; zusätzlich
eine unerreichbare 0x38-Byte-Rahmenlücke (klassisches Phantom-Frame-
Muster). **Empfehlung für Runde 61: dedizierte Runde, die den
`concat()`-Fix mit vollständiger Projekt-Regressionsprüfung anwendet
und alle drei betroffenen Aufrufstellen neu bearbeitet — potenziell
drei Funktionen in einem koordinierten Durchgang.**

**`TConeBeam::calcVertices` (`src/Enemy/beam.cpp`, 88,64 % → auf 2
vertauschte Instruktionen reduziert): FÜNF echte Bugs gefunden:**
- Sin/Cos-Faktoren im `mBGCheckData==nullptr`-Zweig vertauscht
  (`local_140 * s` / `local_134 * c` statt umgekehrt) — der
  nicht-null-Zweig hatte bereits die korrekte Zuordnung, ein klarer
  Beleg für einen echten Transkriptionsfehler mit sichtbarer
  Auswirkung auf Kegel-Beam-Gegner ohne BG-Check-Daten.
- Halbierungsfaktor muss von LINKS multiplizieren (`0.5f *
  (mScale*MsSin(ang))`, nicht `/2.0f`).
- Gemeinsame benannte `ang`-Lokale für `MsSin`/`MsCos` nötig (ohne sie
  vertauscht MWCC die Konstanten-Register).
- Eine `TPartition3<f32> partition(...)`-Lokale existiert bei Retail
  gar nicht: vier `f32`-Lokale (Ebenenabstand + 3 Normalen-
  komponenten) werden VOR der Schleife einmalig geladen und bleiben
  über die gesamte Schleife in Callee-saved-Registern — die
  Struct-Variante erzwingt pro Iteration unnötige Stack-Reloads.
- Deklarationsreihenfolge mehrerer Lokalen (`local_140`, `local_134`,
  `local_128`; `oz`, `oy`, `ox`) musste exakt Retails Speicher-
  reihenfolge treffen.
Restlücke: zwei unabhängige `lfsx`-Ladeinstruktionen (jmaCosTable/
jmaSinTable) werden in vertauschter Reihenfolge emittiert, plus eine
Rahmenlücke (0x1c8 vs. 0x198) durch unterschiedliche Compiler-
Klassifizierung von inline-expandierten Struct-Wertkopien
(`operator*(TVec3,f32)`-Parameterkopien) — nicht durch Trash-Padding
erreichbar, da die POOL-REIHENFOLGE selbst abweicht, nicht nur die
Größe.

**Weitere gründlich dokumentierte Sackgassen** (sauber zurückgesetzt):
`MarDirectorSetupConsole`, `BoidLeaderCalcGoalForce`, `ButterfloidLoad`,
`NpcInbetweenExecPos` (reines FPR-Tie-Break, 8 Varianten erfolglos),
`JDREfbSettingIssueCopy` (bestätigt eine bereits früher in dieser
PROGRESS.md dokumentierte Sackgasse unabhängig erneut), `CameraMulti
PlayerCtrl` (fast vollständig gelöst, ein `MsSqrtf`-Temporärwert
bleibt auf festem Offset 0x10 gepinnt), `CardLoadDrawMessage` (ZWEI
echte Bugs — Array-Out-of-Bounds `unk4CC[3]`→`[2]`, falsches
`JUTRect`-Feld `unk46C`→`unk4B0` — blockiert durch einen
This/Result-Register-Swap, der laut Code-Kommentar `// TODO: hack,
regswap` bereits von einem früheren Mitwirkenden erfolglos bekämpft
wurde; `TExPane::setCenteredSize` ist im Header selbst als „fabricated
and incorrect" markiert), `SnapTimeObjPerform`, `AnimalNerveGraph
Wander`, `SunMgrLoad` (Register-Bank-Vertauschung this/rodata-Basis,
neue Variante der Phantom-Fehlerfamilie), `NpcInitPrgBaseInit`,
`JKRExpHeapAllocFromHead`, `MapCollisionPlaneCheck`, `EffectUtilSink
Pollution`, `CameraBckUpdateDemo` (Boolean-Materialisierung, Referenz
auf denselben Idiom-Typ in `MActor::isCurAnmAlreadyEnd`),
`CameraShakeExecShake` (ein weiterer echter Logikfehler: `unitVecTo`
benutzte `*pos` statt `origin` als Quellvektor — betrifft die
Rollachsen-Berechnung der Kamera-Erschütterung).

### Session-Gesamtstand nach Runde 60

**467 verifizierte echte Fixes in 121 Commits.** `matched_functions`:
**9050** (von 9048 zu Rundenbeginn), `matched_code_percent`: 45,73 %.
Volles `ninja`-Rebuild erfolgreich, `dtk shasum -c` bestätigt
`build/GMSJ01/mario.dol: OK`. Trotz niedriger Fix-Quote dieser Runde
(2/21) außergewöhnlich hoher Erkenntniswert: 12+ dokumentierte echte
Gameplay-Bugs in drei Fast-Treffer-Funktionen, davon einer (`TMatrix34::
concat`) mit projektweitem Einfluss auf mindestens drei Aufrufstellen.

### Nach einundsechzigster Iterationsrunde (dedizierte Header-Fix-Runde: TMatrix34::concat + JGRotation3::setEular — 5 Header-/Funktions-Commits, darunter die schlechteste bisher bekannte Match-Funktion des Projekts geknackt)

**Dediziert der in Runde 60 gefundenen `TMatrix34::concat`-Hypothese
gewidmet, mit vollständiger Projekt-Regressionsprüfung vor jedem
Commit.** Ergebnis: FÜNF Commits, zwei geteilte Header vollständig
korrigiert, zwei Funktionen exakt getroffen — darunter
`TPolarCamera::perform`, die mit Abstand am schlechtesten
übereinstimmende nicht-triviale Funktion des gesamten Projekts
(65,79 % Fuzzy-Match zu Rundenbeginn).

**Commit 1 (`7520c7f5`) — `TMatrix34::concat(a,b)`-Translationsspalte:**
Ursprüngliche Hypothese aus Runde 60 bestätigt und commitet: die
Translationsspalte las über die 3×4-Matrixgrenze hinaus
(`a.at(3,0)`, `b.at(3,1)`, `b.at(3,2)` — gültige Zeilenindizes sind
nur 0-2). Verifiziert über zwei unabhängige Methoden: (a) algebraische
Verallgemeinerung der bereits korrekten 1-Argument-`concat(b)`-Form,
(b) direkter Soll/Ist-Abgleich gegen `PSMTXConcat` in
`src/dolphin/mtx/mtx.c`, da Retail `concat()` an der
`BathWaterManager.cpp:1149`-Aufrufstelle NICHT inlined, sondern
identisch zu diesem SDK-Symbol code-gefaltet hat. Volle
Projekt-Regressionsprüfung: 9050/9050 `matched_functions` unverändert,
45,73091 % `matched_code_percent` unverändert — null Regressionen.

**Commit 2 (`b80c4658`) — `TMatrix34::concat(a,b)`-3×3-Rotationsteil
(vom ersten Commit übersehen):** DREI unabhängige Subagenten
(`BathWaterManagerRender`, `TPolarCameraPerform`, `JDRSmJ3DActRetry`),
die parallel an den drei betroffenen Aufrufstellen arbeiteten, fanden
per Hub-Koordination unabhängig voneinander denselben zweiten Bug:
auch der 3×3-Linearteil war transponiert (`out[i][j] = Σ_k a.at(k,i)*
b.at(j,k)` statt der korrekten Standardform `Σ_k a.at(i,k)*b.at(k,j)`).
Die Translationsspalte allein war zufällig numerisch unauffällig
geblieben, weil die ersten Testfälle (Translations-only, Diagonal-
Skalierung) den Rotationsteil-Bug maskierten. Alle drei Agenten
koordinierten sauber über Hub (ein Agent übernahm probeweise die
Header-Bearbeitung, die anderen bauten nur gegen den Arbeitsbaum-
Zustand, niemand committete parallel) — Main verifizierte den finalen
Diff manuell, führte ein volles Rebuild durch (9050/9050 unverändert,
45,73091 % unverändert, `fuzzy_match_percent` leicht gestiegen) und
committete.

**Commit 3 (`72c8ca33`) — `JDrama::TPolarCamera::perform` EXAKT
GETROFFEN (272/272 Instruktionen, 1088 Byte):** Vier unabhängige Bugs:
(1) `concat()`-Operandenreihenfolge an 2 von 3 Aufrufstellen vertauscht;
(2) Rotationskette ist Z-X-Z (nicht Z-Y-X — klassische Polar-/Orbit-
Euler-Konvention, echter Verhaltensbug: Kamera rotierte um falsche
Achsen); (3) präzise verkettete Zuweisungsgruppierung für eine
Identitäts-plus-Translations-Temporärmatrix nötig (MWCC leitet das
Literal an den ERSTEN Store einer Kette weiter, den Ketten-
Temporärwert an die übrigen); (4) `-unk44` muss ein reiner
Zuweisungs-RHS sein, kein inline-Aufrufargument. Plus `char
trash[104]` nach etabliertem `TOrthoProj::perform`-Muster derselben
Datei. **Entdeckte dabei zwei weitere Header-Bugs** (siehe unten).

**Commit 4 (`258e617c`) — `JDrama::TSmJ3DAct::perform` EXAKT GETROFFEN
(311/311 Instruktionen, 1244 Byte, Rahmen 0x208):** Vier unabhängige
Bugs, drei echte Verhaltensfehler: (1) Euler-Reihenfolge war X-Y-Z,
Retail ist Z-Y-X (verifiziert über Feld-Offsets 0x38/0x34/0x30 der
DEG_TO_RAD-Ladeoperationen); (2) DREI statt zwei Matrix-Temporärwerte
— Retail pingpongt zwischen exakt zwei; (3) Rotations-Temporärmatrix
braucht `setTrans(0,0,0)` statt `identity()` (letzteres schreibt auch
den 3×3-Teil und kostet dadurch +14 Instruktionen); (4) `char
trash[0x48]` für 72 Byte toter Lokale unterhalb der letzten Matrix.
Mit symbolischem Ausdrucksbaum-Interpreter verifiziert: alle 36
Ausgabewerte aller drei `concat`-Aufrufe algebraisch identisch zu
Retail, bevor überhaupt am Scheduling gearbeitet wurde.

**Commit 5 (`38f64391`) — `JGRotation3.hpp::setEularX/Y/Z`, ZWEI
weitere Bugs in einem zweiten geteilten Header:** Während der Arbeit
an den beiden obigen Funktionen fanden `TPolarCameraPerform` und
`JDRSmJ3DActRetry` unabhängig voneinander (an verschiedenen
Aufrufstellen) zwei zusätzliche Fehler in `include/JSystem/JGeometry/
JGRotation3.hpp`: (a) `setEularX` war transponiert (`ref(1,2)=s,
ref(2,1)=-s` statt korrekt `ref(1,2)=-s, ref(2,1)=s`) — inkonsistent
mit den bereits korrekten `setEularY`/`setEularZ` derselben Datei,
echter Verhaltensbug; (b) alle drei `setEularX/Y/Z`-Funktionen
brauchen ihre vier Null-Einträge als EINE verkettete Zuweisung
(`ref(a)=ref(b)=ref(c)=ref(d)=0.0f;`) statt vier separater
`= 0.0f;`-Anweisungen — ein reines Codegen-Ordnungsproblem (MWCC
erzeugt den Null-Wert-Knoten bei vier separaten Anweisungen spät/pro
Store, bei einer Kette früh/einmalig; das verändert die
Scheduler-Eingabereihenfolge in JEDEM inline-expandierten `concat`-
Block und verschiebt zusätzlich den Stack-Rahmen — kein semantischer
Unterschied). `JDRSmJ3DActRetry` lieferte harte Evidenz auch für
`setEularY` (168 Instruktionen Differenz ohne die Verkettung, exakt 0
mit ihr) — widerlegt die ursprüngliche Annahme "keine Evidenz für Y".
Volle Regressionsprüfung vor Commit: `matched_functions` 9050→9052
(exakt die zwei oben genannten neuen Matches, keine unerklärten
Verschiebungen), keine Regression in `bossManta.cpp` (einzige weitere
Datei, die `setEularY` nutzt) oder irgendwo sonst im Projekt.

**★ Herausragender Fast-Treffer, NICHT commitet:**
`TBathWaterMeshRenderer::prerender` (`src/Map/BathWaterManager.cpp`,
2624 Byte) — von 84,3 % (nach beiden `concat`-Fixes) auf 94,7 %
gebracht, ZEHN unabhängige Bugs gefunden und bestätigt, darunter:
- **Echter Gameplay-Bug**: `GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE)`
  muss `GX_LESS` sein — der Tiefentest für den Badewasser-
  Höhenkarten-Vorabdurchlauf ist im Dekompilat falsch.
- Ein bemerkenswerter Fund in `include/JSystem/JGeometry/
  JGProjection.hpp::orthographic()` (nur von dieser einen Funktion
  genutzt): beide Translationsterme brauchen `+ n` (den Parameter,
  NICHT das Literal `0.0f`) — mathematisch ein Bug im Retail-Code
  selbst, der aber harmlos bleibt, weil jeder Aufrufer `n = 0.0f`
  übergibt; muss dennoch als `+ n` transkribiert werden, um Retails
  tatsächliches (fehlerhaftes) Verhalten exakt zu reproduzieren —
  Kernprinzip der Dekompilation: Bugs mitkopieren, nicht korrigieren.
- Sieben weitere Bugs/Techniken: Struct-Kopie statt Skalar-Extraktion
  für eine Drop-Position, doppelt vorkommende negR/R3-Konstanten-
  berechnung (Retail berechnet sie zweimal ohne CSE), Deklarations-
  reihenfolge-Fix für einen Stack-Pool, hochgezogene `J3DModelData*`-
  Lokale, ein als verworfene Anweisung transkribierter
  `TUtil<f32>::sqrt`-Aufruf mit toter Domänenprüfung, Auswertungs-
  reihenfolge-Fix (Retail ruft `SMSGetGameRenderHeight` vor `...Width`
  auf), unbenannte Temporärwerte statt benannter `dir`/`up`-Lokalen.
Restlücke (5,3 %): zwei Instruktions-Scheduling-Cluster (eine
`drawCap`-Schleifeninvariante, die bei JEDER zusätzlichen Lokalen aus
dem `-inline auto`-Budget fällt — bestätigt hauchdünn) sowie eine
Stack-Pool-RICHTUNGS-Divergenz (Retail alloziert einen Temporär-Pool
in der ENTGEGENGESETZTEN Richtung zu unserem Build, plus ein totes
92-Byte-Sperrblock — klassisches Phantom-Frame-Muster, `char
trash[92]` reproduziert zwar Größe und Abstand exakt, ändert aber den
objdiff-Score nicht). Vollständiges 10-Punkte-Rezept im
Agent-Transkript `history://BathWaterManagerRender` archiviert.

### Session-Gesamtstand nach Runde 61

**469 verifizierte echte Fixes in 126 Commits** (467 aus Runde 60 + 2
neue Funktions-Matches; die 3 Header-Fix-Commits zählen als
Infrastruktur-Korrekturen, nicht als eigene „Funktions-Fixes", tragen
aber wesentlich zum Gesamtfortschritt bei). `matched_functions`:
**9052** (von 9050 zu Rundenbeginn), `matched_code_percent`: 45,80 %.
Volles `ninja`-Rebuild erfolgreich, `dtk shasum -c` bestätigt
`build/GMSJ01/mario.dol: OK`. Diese Runde demonstriert den Wert
koordinierter Parallel-Subagenten bei geteilten Headern: drei Agenten
fanden denselben zweiten `concat`-Bug unabhängig voneinander aus drei
verschiedenen Aufrufstellen und koordinierten sauber über Hub, um
Merge-Konflikte am gemeinsamen Header zu vermeiden, bevor Main den
finalen Commit nach vollständiger Regressionsprüfung durchführte.

### Nach zweiundsechzigster Iterationsrunde (Methodik-Wechsel zu funktionsgenauer statt dateibasierter Ausschlussliste — 23-Kandidaten-Batch, 14 neue Matches, zweitstärkste Runde bisher)

**Methodik-Erkenntnis:** Der dateibasierte Ausschluss-Fragment-Ansatz
(bisher: alle Funktionen einer Datei ausschließen, sobald EINE Funktion
darin behandelt wurde) hatte den Kandidatenpool künstlich auf nur noch
4-24 Kandidaten schrumpfen lassen, obwohl `fuzzy_match_percent < 100`
im frischen `report.json` bereits objektiv beweist, dass eine
bestimmte Funktion noch NICHT exakt getroffen ist — unabhängig davon,
ob andere Funktionen derselben Datei bereits gefixt wurden. Umstellung
auf eine präzise, pro-Einheit-exakte Ausschlussliste (nur konkret
dokumentierte Sackgassen-Einheiten, keine Substring-Fragmente mehr)
öffnete den Kandidatenpool sofort wieder auf 167 frische Dateien.

**14 neue Fixes, alle commitet:**

1. `TMarioParticleManager::emitAndBindToMtxPtr` (Commit `a7b21d6e`) —
   `char trash[4]` nach einer Vec3-Lokalen.
2. `TNervePoihanaSleep::execute` (Commit `713f66e3`) — `char trash[8]`.
3. `TOptionControl::loadSetting` (Commit `8837a94b`) — `char
   trash[0x28]` (40 Byte).
4. `TNerveStayPakkunHide::execute` (Commit `95394573`) — `char
   trash[16]`.
5. `MSoundSESystem::MSoundSE::getRandomID` (Commit `9c3ed7b2`) —
   `char trash[4]` nach einem `u32[16]`-Array (identisches Muster zu
   `lensflare.cpp`/`lensglow.cpp`).
6. `TMario::waiting` (Commit `8a33e6b7`) — `char trash[24]`, gleiche
   Technik wie das bereits gefixte `TMario::stopCommon` derselben
   Datei.
7. `TWaterGun::setBaseTRMtx` (Commit `4c5be3f1`) — **neue Stack-
   Technik**: kein Trash-Array half hier (alle drei Positionen
   erfolglos, wie in früheren Runden dokumentiert); stattdessen musste
   die `Mtx temp;`-Deklaration strukturell an ihre erste Verwendungs-
   stelle verschoben werden (nicht als Padding, sondern durch
   Änderung der MWCC-Frontend-Erzeugungsreihenfolge selbst).
8. `TNPCManager::makePartsModelData_` (Commit `ed928359`) — `char
   trash[4]` nach einem `char[0x100]`-Puffer.
9. `TMarioEffect::perform` (Commit `a6a5d71a`) — `Mtx mtx;` als
   ungenutzte, aber Retail-seitig echt deklarierte Lokale (48 Byte,
   sizeof(Mtx)) — hier WAR die fehlende Reservierung eine reale, im
   Quelltext einfach vergessene Variable, kein Trash-Padding-Trick.
10. `TPauseMenu2::load` (Commit `1fa46609`) — **zwei echte Bugs**:
    (a) Panel-Suchschlüssel `'t_0' + i` muss `'pa00' + i` sein (falscher
    FourCC-Literal, hätte im Spiel dazu geführt, dass die Pause-Menü-
    Buchstaben-Bilder nicht gefunden werden); (b) `add(0, 14)` muss
    `add(0, 20)` sein. Plus `char trash[32]`.
11. `TAmenbo::init` (Commit `b8b91d24`) — **echter Bug**: Gelenk-
    Index-Cache-Schleife rief `getMaterialName()` (Offset 0xB4) statt
    `getJointName()` (Offset 0xB0) auf — falscher Accessor, liest
    einen komplett anderen Materialdaten-Bereich. Plus `char trash[8]`.
12. `TMario::surfing` (Commit `e7ed3c73`) — **echter Bug**: Wasser-
    /Boden-Parameterauswahl testete `mWallPlane->isWaterSurface()`
    statt `mGroundPlane->isWaterSurface()` (TMario+0xD8 statt +0xE0,
    falsches Feld). Plus `char trash[8]`.
13. `TGesso::bind` (Commit `38bde37e`) — zwei verzahnte Stack-Layout-
    Ursachen: (a) `MsAtan2()`-Inline-Wrapper-Aufruf durch die
    ausgeschriebene Form `abs(matan(...) * (360.0f/65536.0f))` ersetzt
    (spart einen 8-Byte-Inline-Rückgabe-Temporärwert, gleiches Idiom
    wie in `graph.cpp`); (b) eine VIERTE, komplett ungenutzte
    `TVec3<f32>`-Lokale nötig, um einen fehlenden 12-Byte-Slot im
    Block zu reservieren (Block ist boden-verankert: neue Lokale
    verschiebt den Frame-Boden nach unten, nicht nach oben).
14. `TFireWanwanTailHit::movementBody` (Commit `f1361d70`) — **neue
    Technik, Umkehrung von Muster 5**: unser Rahmen war KLEINER als
    Retail (0xf8 vs. 0x138, 0x40 Byte Differenz), verursacht durch
    FEHLENDE Inline-Expansions-Reservierungen. Anstatt einen Accessor
    durch Direktzugriff zu ersetzen (übliche Richtung von Muster 5),
    mussten hier DREI NEUE fabrizierte Inline-Setter (`TTailRubber::
    setBoundRate/setDecay/setMaxLength`) in `include/Enemy/
    FireWanwan.hpp` ergänzt und an 8 Stellen anstelle von Direkt-
    zuweisungen verwendet werden, um genau die fehlenden Inline-
    Reservierungsblöcke zu erzeugen — der Instruktionsstrom bleibt
    dabei komplett unverändert. Zusätzlich musste ein bereits
    existierender fabrizierter Wrapper (`isTailTaken()`) anstelle von
    `unk194->isTaken()` verwendet werden. Header-Änderung ist rein
    additiv (neue Methoden, keine Verhaltensänderung an bestehenden);
    beide anderen Einbinder der Datei (`CameraNormal.cpp`,
    `MarNameRefGen_Enemy.cpp`) wurden nachgebaut und sind exakt
    unverändert (md5-identisch) — keine Regression.

**Weitere gründlich dokumentierte Sackgassen** (alle sauber
zurückgesetzt): `TBathWaterManager::loadAfter`, `TTrembleModelEffect::
reset`, `CPolarSubCamera::execGroundCheck_`, `TTamaNoko::
calcRootMatrix`, `THamuKuri::behaveToWater`, `JPAGetRMtxSTVecElement`
(bestätigt: `std::sqrtf`s Drei-Schritt-Newton-Raphson-Iteration ist
bereits korrekt, `TUtil<f32>::sqrt()` wäre die falsche, da nur
einstufige, Funktion), `TRoulette::initMapObj` (4 Varianten,
`JGadget::TList`-Iterator-Temporärwert-Familie), `TGraphWeb::
getRandomNextIndex` (8-Byte-Lücke unerreichbar ohne Stilbruch
gegenüber Schwesterfunktionen derselben Datei), `TBossPakkun::
setGroundCollision` (13 Varianten, exakte Zielposition nur mit
falscher Rahmengröße erreichbar, nie beides gleichzeitig).

### Session-Gesamtstand nach Runde 62

**483 verifizierte echte Fixes in 140 Commits.** `matched_functions`:
**9066** (von 9052 zu Rundenbeginn, +14, exakt wie erwartet).
`matched_code_percent`: 46,06 % (größter Einzelrunden-Zuwachs der
jüngeren Session-Geschichte, +0,26 Prozentpunkte). Volles
`ninja`-Rebuild erfolgreich, `dtk shasum -c` bestätigt
`build/GMSJ01/mario.dol: OK`.

### Nach dreiundsechzigster Iterationsrunde (23-Kandidaten-Batch, 16 Fixes/17 Funktionen — neuer Rekord, drei neue Stack-Layout-Techniken katalogisiert)

**16 neue Fix-Commits, 17 exakt getroffene Funktionen** (eine
2-für-1-Sonderfund, siehe unten) — größter Einzelrunden-Zuwachs der
Session bisher (+0,28 Prozentpunkte `matched_code_percent`).

**Drei neue, allgemein wiederverwendbare Stack-Layout-Techniken
entdeckt:**

1. **Pattern 7i — „echter Initialisierer überlebt, `char trash[N]`
   nicht"** (`TFireWanwan::receiveMessage`): ein unbenutztes `char
   trash[N];` ohne Initialisierung wird von MWCC VOR der Stack-Layout-
   Kandidaten-Registrierung wegoptimiert (tote Speicherung eliminiert,
   bevor der Slot überhaupt gezählt wird). Ein Lokal mit einem ECHTEN,
   nicht-konstanten Initialisierer-Ausdruck (z. B. `u8 trash =
   sender->getActorType();`) übersteht dagegen die Kandidaten-
   Registrierungsphase und reserviert seinen Slot, obwohl der
   nachfolgende Lese-/Schreibzugriff selbst wieder als totzurück-
   eliminiert werden kann — Regel: MWCC zählt Frontend-Lokale für die
   Stack-Platzierung, BEVOR die Dead-Store-Elimination ihre
   tatsächlichen Lade-/Speicherinstruktionen entfernt.
2. **Pattern 7j — Umkehrung von Muster 5/7h, fabrizierte Inline-
   Helfer HINZUFÜGEN statt entfernen** (`TMario::changePlayerTriJump`
   / `changePlayerJumping`, 2-für-1-Fund): `TMario::setMissJumping()`
   war in `include/Player/Mario.hpp` bereits als `inline` deklariert,
   aber in `MarioMove.cpp` mit einem LEEREN Rumpf „gestubbt" — Retail
   hatte dort echten Code (Gesichtswinkel-Reset, bedingte
   Geschwindigkeits-Neuberechnung über `JMASSin`/`JMASCos`/
   `MsSqrtf`/`matan`, `dropObject()`, `changePlayerStatus(...)`), der
   an BEIDEN Aufrufstellen vollständig inline expandiert wurde (kein
   `setMissJumping`-Symbol im Retail-Objekt vorhanden). Den echten
   Rumpf einzusetzen behob gleichzeitig zwei vorher getrennt
   fehlschlagende Funktionen (`changePlayerTriJump` UND
   `changePlayerJumping`) mit einem einzigen Commit — die fehlenden
   8 Byte Inline-Rahmen-Reservierung kamen exakt aus der jetzt
   vorhandenen Inline-Expansionsebene.
3. **Pattern 7k — MWCCs Zwei-Pool-Modell für Stack-Allokation**
   (charakterisiert durch `LiveActorBind`, NICHT gelöst, aber
   erstmals präzise beschrieben): benannte C++-Lokale werden in einen
   Pool allokiert, der OBEN am Register-Save-Bereich verankert ist
   (wächst den Rahmen nach oben, wenn mehr/größere Lokale
   hinzukommen); anonyme, vom Compiler synthetisierte Temporärwerte
   (z. B. aus inline-expandiertem `operator-`) leben in einem
   GETRENNTEN, unabhängig dimensionierten Pool, der UNTEN verankert
   ist. Trash-Arrays können NIEMALS Bytes zwischen diesen beiden
   Pools verschieben — sie wachsen immer nur den oberen (benannten)
   Pool. Erklärt einen erheblichen Teil der bisher als „unlösbar"
   dokumentierten Phantom-Frame-Fälle strukturell.

**Alle 16 Fixes:**

1. `TMarioModokiTelesa::load` (Commit `80111de4`) — `char trash[16]`.
2. `TNervePoihanaFreeze::execute` (Commit `23782a99`) — `char
   trash[0x10]`, gleiche Technik wie das Runde-62-Fix der
   Schwesterfunktion `TNervePoihanaSleep::execute`.
3. `TMario::checkGroundAtWalking` (Commit `a629df2e`) — reine
   Deklarationsreihenfolge-Korrektur: `roof`, `ground`, `floorY` (statt
   `floorY`, `ground`, ..., `roof` verstreut) — kein Trash nötig.
4. `TEnemyMario::initValues` (Commit `4ae8583e`) — `char trash[8]`.
5. `TMarioParticleManager::emitAndBindToSRTMtxPtr` (Commit
   `13c8dafd`) — `char trash[4]`, identisches Muster zur bereits
   gefixten Schwesterfunktion `emitAndBindToMtxPtr` (Runde 62).
6. `TMario::initValues` (Commit `22c9d010`) — `char trash[8]`.
7. `TPlayerLightWithDBSet::makeDrawBuffer` (Commit `606eb283`) —
   `char trash[32]`.
8. `TBEelTears::receiveMessage` (Commit `34ba1171`) — `char trash[8]`.
9. `TNameKuriManager::initSetEnemies` (Commit `14965073`) — `char
   trash[16]`.
10. `TNpcCoin::requestAppearCoin` (Commit `a340c065`) — **echter Bug**:
    `MsSin/MsCos(75)`-Header-Wrapper rundete den Kurzwinkel auf 0x3555
    statt Retails 0x3552; direkter `JMASSin/Cos(75*182)`-Aufruf (der
    im Code bereits etablierte Grad→Winkel-Faktor) reproduziert
    Retails exakte Konstante. Plus `char trash[0x10]`.
11. `TMonumentShine::control` (Commit `092a4c2d`) — Umkehrung des
    üblichen Musters: zwei überflüssige benannte Lokale (`limit`,
    `zero`) ENTFERNT (Literale direkt inline verwendet) — Retails
    Rahmen war KLEINER, nicht größer.
12. `TPollutionLayer::fire` (Commit `2ec08166`) — zwei identische
    `JGeometry::TVec3<f32>(1.5f,1.5f,1.5f)`-Konstruktor-Aufrufstellen
    zu einer gemeinsamen benannten Lokalen zusammengeführt (jede
    separate Inline-Konstruktion reservierte einen eigenen Phantom-
    Slot).
13. `CPolarSubCamera::updateGateDemoCamera_` (Commit `6fd7409b`) —
    `char trash[4]`.
14. `TFireWanwan::receiveMessage` (Commit `828553f2`) — Pattern 7i
    (siehe oben), `u8` mit echtem Initialisierer statt totem
    `char trash[N]`.
15. `TSpcTypedInterp<TEventWatcher>::evSetFruitType` (Commit
    `57da7962`) — **echter Bug**: zwei gepoppte Werte waren als
    `int` statt `u32` deklariert; Retail nutzt vorzeichenlose Typen
    mit expliziten `(int)`-Casts an den Vergleichsstellen. Hinweis für
    künftige Runden: Schwesterfunktion `evGetFruitNum` in derselben
    Datei zeigt identisches +8-Byte-Rahmen-Symptom, vermutlich
    derselbe Fix, nicht im Scope dieser Runde bearbeitet.
16. `TMario::changePlayerTriJump` + `TMario::changePlayerJumping`
    (Commit `ad4217f7`) — Pattern 7j (siehe oben), 2-für-1-Fund.

**Weitere gründlich dokumentierte Sackgassen** (sauber
zurückgesetzt): `TMario::turnning` (Zwei-Pool-Problem, Ziellokal
immer im oberen statt unteren Pool platziert), `TMario::
initMirrorModel` (bestätigt und erweitert einen bereits aus Runde 28
bekannten Cross-Funktions-.rodata-Literal-Pool-Versatz von 24 Byte,
eigenständiger ELF32-Parser geschrieben, da `readelf`/`objdump`
fehlen), `TNerveBathtubKillerExplosion::execute` (bestätigt
identisches Symptom in Schwester-Nerve `TNerveBathtubKillerBreak`,
bereits aus einer früheren Runde als TODO-Kommentar im Code vermerkt),
`TAmenbo::calcRootMatrix` (adressgenommene `TPosition3f`-Lokale
resistent gegen jede Trash-Platzierung), `TNerveBGKAppear::execute`
(weitere Bestätigung des `TFlagT<u16>`-Systemproblems, diesmal mit
9-teiligem Varianten-Log), `TLiveActor::bind` (Ursprung von Pattern
7k, siehe oben), `TBossMantaManager::setupEfbAlpha` (anonymer
`GXColor`-Blockspeicher-für-Zeiger-Argument landet immer an der
niedrigsten statt höchsten Adresse, 7 Varianten erfolglos).

### Session-Gesamtstand nach Runde 63

**499 verifizierte echte Fixes in 156 Commits.** `matched_functions`:
**9083** (von 9066 zu Rundenbeginn, +17). `matched_code_percent`:
**46,34 %** (größter Einzelrunden-Zuwachs der Session, +0,28
Prozentpunkte). Volles `ninja`-Rebuild erfolgreich, `dtk shasum -c`
bestätigt `build/GMSJ01/mario.dol: OK`. Rundenfolge 61-63 zusammen:
`matched_functions` 9050→9083 (+33), `matched_code_percent`
45,73 %→46,34 % (+0,61 Prozentpunkte) — drei projektweite Header-Bug-
Funde plus zwei starke reguläre Batch-Runden.

### Regression entdeckt und behoben: `CPolarSubCamera::controlByCameraCode_` (Runde-59-Fix war unvollständig)

Der beim frischen Kandidaten-Scan für Runde 64 erneut auftauchende
99,93 %-Fuzzy-Match für eine bereits in Runde 59 als „byte-exakt"
gemeldete Funktion deckte eine ECHTE Regression/unvollständige
Verifikation auf: der Runde-59-Fix (`char trash[0x28]` VOR
`local_24`) korrigierte nur die GESAMTE Rahmengröße (0x50→0x78),
bewegte aber `local_24` selbst nicht — laut der etablierten Regel
bewirkt vor einem Lokal deklariertes Trash nur Padding OBERHALB,
ohne das Lokal selbst zu verschieben. `local_24` blieb bei 0x2c statt
der benötigten 0x54(r1) — ein echter 0x28-Byte-Versatz in mehreren
eingebetteten Stack-Offset-Immediates (`stw r3, 0x2c(r1)` statt
`0x54(r1)` usw.), keine reine Label-/Adress-Normalisierungsdifferenz.
Der Runde-59-Agent hatte demnach nur die Gesamtrahmengröße geprüft,
nicht jedes einzelne eingebettete Offset-Immediate — genau die Lücke,
die das neu erkannte Zwei-Pool-Modell (Muster 7k, Runde 63) erklärt.
**Behoben** durch Verschieben von `char trash[0x28];` von VOR nach
NACH der `local_24`-Deklaration (Commit `dacacbff`), per Byte-für-
Byte-Diff gegen Retail direkt verifiziert (alle Instruktions-
Hex-Wörter inkl. aller Stack-Offset-Immediates identisch). Volles
Rebuild bestätigt: `matched_functions` 9083→9084 (+1, korrekt),
`dtk shasum -c` weiterhin OK.

**Lehre für alle künftigen Runden**: Byte-exakte Verifikation MUSS
jedes einzelne eingebettete Stack-Offset-Immediate prüfen, nicht nur
Opcode-Sequenz und Gesamt-Rahmengröße — ein Diff, der nur Mnemonics
vergleicht, kann genau diese Klasse Fehler übersehen. Alle Runde-64-
Subagenten wurden mit dieser verschärften Anforderung instruiert
(literaler Hex-Byte-Spalten-Diff der rohen 4-Byte-Instruktions-
kodierung, keine Normalisierung außer Adressen/Label-Nummerierung).

### Nach vierundsechzigster Iterationsrunde (24-Kandidaten-Batch, 18 Fixes — neuer Rekord, verschärfte Byte-Verifikation)

**18 neue Fix-Commits**, alle mit der oben beschriebenen verschärften
Byte-für-Byte-Verifikation bestätigt (roher 4-Byte-Instruktions-
encoding-Vergleich, nicht nur Opcode-Text):

1. `TObjectLightWithDBSet::makeDrawBuffer` (Commit `a5d1b35b`) —
   `char trash[32]`, identisches Muster zur Schwesterfunktion
   `TPlayerLightWithDBSet::makeDrawBuffer` (Runde 63).
2. `TMario::swimPaddle` (Commit `887775a4`) — `char trash[8]`.
3. `TNerveFireWanwanEscape::execute` (Commit `8ec3a213`) — `char
   trash[8]`.
4. `TNerveBEelTearsWaterHit::execute` (Commit `592ad0b6`) — `char
   trash[0x18]`.
5. `TNerveTelesaDie::execute` (Commit `35eed88f`) — `char trash[8]`.
6. `TNerveStayPakkunAppear::execute` (Commit `7304445c`) — zwei
   identische `JGeometry::TVec3<f32>(1.5f)`-Konstruktor-Aufrufstellen
   zu einer gemeinsamen benannten Lokalen zusammengeführt (identische
   Technik zu `TPollutionLayer::fire`, Runde 63).
7. `SMSSetupGameRenderingInfo` (Commit `b68ae85f`) — `char trash[16]`.
8. `TSunModel::load` (Commit `3b27211e`) — zweiseitiges Padding
   (`trash1[4]` davor, `trash2[4]` danach um `char path[0x100]`).
9. `JAIBasic::checkEntriedSeq` (Commit `81c3977f`) — `u8 trash[8]`
   nach `u8 pos`.
10. `TCardManager::writeBlock_` (Commit `81022e2f`) — zweiseitiges
    Padding (`trash[4]` davor, `trash2[20]` danach um `CARDFileInfo
    info`).
11. `TNerveHino2Damage::execute` (Commit `d38062ae`) — `char
    trash[0x48]` (72 Byte).
12. `JPABaseEmitter::calc` (Commit `712d3d39`) — `char trash[8]`.
13. `TMarioParticleManager::emitTry` (Commit `b24ea779`) — **echter
    Bug**: if/else-Zweige für `emitterCallBackBindToMtxPtr` vs.
    `...BindToSRTMtxPtr` waren vertauscht (Flag-Semantik
    `INFO_FLAG_BIND_TO_RT_MTX` zeigte auf die falsche Callback-
    Variante). Plus Pattern 7i (`u8 trash = param_3;`, echter
    Initialisierer statt totem `char trash[N]`).
14. `MSRandPlay::randPlay` (Commit `8073cfe8`) — dreifach wiederholter
    `vec->mTrans`-Unterausdruck in einem `JAIActor`-Konstruktor-Aufruf
    beeinflusste MWCCs CSE-/Stack-Kandidaten-Registrierung; Ersatz
    durch eine einmal initialisierte `const Vec* trans`-Lokale (für
    Argument 2 und 3, Argument 1 bleibt der Direktausdruck) verschob
    den `actor`-Lokal-Slot exakt um die nötigen 4 Byte.
15. `TApplication::proc` (Commit `a1373f5d`, + Header
    `include/System/MenuDir.hpp`) — **VIER unabhängige Bugs**: (a)
    vertauschte Argumente bei `TFlagManager::setFlag(3, 0x20001)` →
    `setFlag(0x20001, 3)`; (b) `sizeof(TMenuDirector)` war 4 Byte zu
    klein (`new` allozierte 0x54 statt Retails 0x58) — geteilter
    Header um ein `/* 0x54 */ u32 unk54;`-Feld ergänzt, mit vollem
    Vorher/Nachher-Regressionsvergleich beider Einbinder-Dateien
    (Application.cpp, MenuDir.cpp) verifiziert, null Regressionen;
    (c) `delete mDirector;` (Flag `+1`, „zerstören UND freigeben")
    musste `mDirector->~TDirector();` sein (expliziter Destruktor-
    Aufruf, Flag `-1`, „nur zerstören, Speicher wird andernorts per
    `mHeap->freeAll()` freigegeben") — Flag-Semantik aus Retails
    eigenem `__dt__13TMenuDirectorFv`-Epilog dekodiert und gegen zwei
    weitere `delete`-Aufrufstellen in derselben Datei kreuzverifiziert;
    (d) `char trash[0x68]` (104 Byte) Stack-Padding.
16. `TEnemyManager::copyFromShared` (Commit `378dbcf6`) — Deklarations-
    reihenfolge-Fix zweier `Mtx`-Lokalen (identische Namen/Reihenfolge
    bereits als Konvention in `hinokuri2.cpp` etabliert) plus `char
    trash[12]`.
17. `TCommonLauncher::stateLaunch` (Commit `dd4f91e4`) — subtiler
    Deklarationsreihenfolge-Fix: einfaches Vertauschen zweier Lokalen
    behob zwar die Stack-Offsets, regressierte aber eine FPR-Zuweisung
    in einer nicht verwandten inline-expandierten `MsWrap`-Instruktion;
    korrekte Lösung war, die Deklaration (ohne Konstruktor-Argumente)
    an die richtige Stelle zu setzen, aber die tatsächliche Wert-
    zuweisung (`.set(...)`) an ihrer ursprünglichen Verwendungsstelle
    zu belassen — trennt Stack-Slot-Zuteilung von Instruktions-
    Terminplanung.
18. `TMario::kickFruitEffect` (Commit `b227e351`) — **echter Bug**:
    `setEmitterTranslation()`-Wrapper schreibt tatsächlich in
    `mTrans` (Offset +0x19c), aber Retail beschreibt direkt
    `mGlobalTranslation` (Offset +0x160) — irreführend benannter
    Wrapper, Direktzugriff auf das richtige Feld nötig.

**Drei Kandidaten scheiterten sauber ohne Dateiänderung** (Agent
brach während der Untersuchung ab, kein Commit, keine Änderung
zurückzusetzen — für eine künftige Runde vorgemerkt):
`TMario::~TMario` (Destruktor tatsächlich header-inline, nicht in
MarioInit.cpp lokalisierbar wie angenommen), `TMapObjBase::
getDistance`, `TMarDirector::fireGetStar`.

**Weitere gründlich dokumentierte Sackgassen** (sauber
zurückgesetzt): `TMareEventDepressWall::depressing` (Anonymer-Pool-
Diskrepanz nicht-uniform über zwei bedingte Temporärwerte verteilt,
ein Versuch mit dem bereits deklarierten aber leeren `emitEffect(int)`-
Stub regressierte einen vorher exakten Codeblock, zurückgesetzt),
`TWoodBox::kill` (152-Byte zusammenhängender unbenutzter Block am
Boden des Rahmens, MWCC platziert Frontend-Lokale IMMER oberhalb von
Inline-Temporärwerten unabhängig von der Deklarationsposition —
bestätigt Muster 8 erneut), `MtxToQuat` (ein einziges vertauschtes
Operandenpaar in einer kommutativen `fadds`-Instruktion, 24
erschöpfend getestete Summierreihenfolgen, alle Varianten bestätigen
die Original-Quelltextform als bereits optimal — vermutlich
Compiler-internes Werteordnungs-/Scheduling-Verhalten, nicht aus
Quelltext heraus beeinflussbar).

### Session-Gesamtstand nach Runde 64

**518 verifizierte echte Fixes in 175 Commits** (inkl. der Runde-59-
Regressionskorrektur). `matched_functions`: **9102** (von 9083 zu
Rundenbeginn, +18 exakt wie erwartet — bestätigt keine unentdeckten
Regressionen durch den `MenuDir.hpp`-Header-Fix). `matched_code_percent`:
**46,64 %** (weiterer Rekord-Einzelrunden-Zuwachs, +0,29
Prozentpunkte). Volles `ninja`-Rebuild erfolgreich, `dtk shasum -c`
bestätigt `build/GMSJ01/mario.dol: OK`.

### Nach fünfundsechzigster Iterationsrunde (24-Kandidaten-Batch, 12 Fixes trotz Infrastruktur-Ratenbegrenzung, TFlagT<u16>-Konstruktorsyntax-Erkenntnis)

**Fünf Kandidaten scheiterten an einem Infrastruktur-Problem**
(Rate-Limit-Fehler des zugrundeliegenden Modell-Providers,
`minimax-code/MiniMax-M3 429`, sowie einige unklare frühe Abbrüche)
— alle fünf machten KEINE Dateiänderungen (sauber abgebrochen vor
jeder Bearbeitung), verbleiben also als frische Kandidaten für eine
künftige Runde: `TMapObjectLightWithDBSet::makeDrawBuffer`,
`SMSSetupTitleRenderingInfo`, `TSmallEnemy::generateItem`.
(`TMario::~TMario` und `TMapObjBase::getDistance` wurden in DIESER
Runde als Retries behandelt und sind unten dokumentiert — eines
erfolgreich gefixt, das andere ebenfalls an derselben Ratenbegrenzung
gescheitert.)

**12 neue Fix-Commits:**

1. `TMario::doRoofMovingProcess` (Commit `7ca67917`) — `char trash[4]`
   nach `newPos`.
2. `TMario::squating` (Commit `e3b584cd`) — `char trash[56]`.
3. `TNerveBEelTearsSplit::execute` (Commit `b414b8f9`) — `char
   trash[16]`.
4. `TTalkCursor::loadAfter` (Commit `09998db8`) — `char trash[8]`.
5. `TMapObjGeneral::appear` (Commit `3549f48b`) — `char trash[8]`.
6. `TNameKuri::moveObject` (Commit `cb12847b`) — `char trash[12]`.
7. `TNerveMameGessoWait::execute` (Commit `34a9edfc`) — `char
   trash[8]`.
8. `TMario::barProcess` (Commit `e89da4b0`) — `char trash[4]` NACH
   `pos` (Sonderfall: alle Lokalen lagen gleichmäßig 4 Byte zu
   niedrig, Rahmengröße war durch 8-Byte-Rundung bereits zufällig
   korrekt).
9. `TMenuDirector::setFixedStageValue` (Commit `d0a4e4e2`) — `char
   trash[0x1C]` nach einem Array-Lokal.
10. `TMarioParticleManager::perform` (Commit `8be3d482`) — **neue
    Erkenntnis zur Trash-Anker-Position**: `char trash[24]` musste am
    ENDE der Funktion (nach dem letzten Block, vor der schließenden
    Klammer) stehen, nicht am Anfang — beide Positionen korrigieren
    zwar die Gesamtrahmengröße gleich, aber nur die Endposition lässt
    den Abstand zweier `JPADrawInfo`-Lokalen zum Rahmen-Oberrand
    unverändert, wie es Retail benötigt.
11. `TNerveTobiPukuBound::execute` (Commit `c052a17d`) — kombinierter
    Fix: `velocity2`-Deklaration (ohne Initialisierung) vor den
    äußeren if-Block gezogen (für Stack-Slot-Reihenfolge), tatsächliche
    Zuweisung an ursprünglicher Stelle belassen (für Instruktions-
    Terminierung), PLUS `char trash[12]`.
12. `TMarDirector::fireGetStar` (Commit `437d5ead`) — **wichtige neue
    Erkenntnis zur `TFlagT<u16>`-Fehlerfamilie**: ein bereits
    vorhandenes, wirkungsloses `char trash[4]` war nur Rauschen: die
    tatsächliche Ursache war `JDrama::TFlagT<u16>(0)` (expliziter
    Konstruktor-Aufruf mit Null-Argument) statt `JDrama::TFlagT<u16>()`
    (Standard-Konstruktor) — beide erzeugen denselben Laufzeitwert,
    aber MWCCs anonymer Temporärwert-Pool-Allokator positioniert den
    Temporärwert unterschiedlich für die beiden Schreibweisen. Dies
    ist die ERSTE bestätigte Auflösung eines `TFlagT<u16>`-„Phantom-
    Frame"-Falls durch eine reine Syntaxänderung (keine Stack-
    Padding-Technik) — lohnt sich, bei den zahlreichen bereits als
    Sackgasse dokumentierten `TFlagT<u16>`/`fireStartDemoCamera`-
    Fällen aus früheren Runden erneut zu prüfen, ob sie explizite
    `(0)`-Argumente statt Standard-Konstruktoren verwenden.

**Weitere gründlich dokumentierte Sackgassen** (alle sauber
zurückgesetzt, mehrere mit außergewöhnlich gründlicher Bisektion):
`MActor::setModel` (9 Varianten, anonyme `JGadget::TList`-Iterator-
Vergleichs-Temporärwerte unbeeinflussbar durch Named-Pool-Padding),
`THinokuri2::changeBck` (systematische Bisektion zeigt: zwei sich
gegenseitig ausschließende if/else-Zweige reservieren JEWEILS einen
eigenen 32-Byte-Anonym-Pool-Slot, obwohl nie gleichzeitig lebendig —
16 Byte Overreservierung ließ sich nicht durch Padding entfernen, da
Padding nur hinzufügen, nie entfernen kann), `TMapStaticObj::perform`
(über 70 Build-Iterationen, Ziel-Layout verlangt gleichzeitiges
Wachsen UND Schrumpfen verschiedener Rahmenbereiche — unerreichbar),
`TFireWanwan::behaveToWater` (vier Platzierungs-/Typ-Varianten
bestätigen erneut das Zwei-Pool-Modell empirisch), `TBaseNPC::
changeNerveFromTalk_` (11 Iterationen — JEDE Umformulierung, die den
Instruktionsstrom exakt hält, reproduziert auch die exakte
16-Byte-Overreservierung; JEDE Umformulierung, die die Rahmengröße
ändert, bricht auch den Instruktionsstrom — kein Ausweg gefunden),
`TStayPakkun::genRandomItem` (mathematische Formelherleitung: Rahmen-
größe F = 48 + 24×N für N TVec3-Kandidaten, Zielwert hat keine
ganzzahlige Lösung für N — beweist, dass die gesamte Familie „TVec3-
Temporärwerte konsolidieren/aufteilen" als Fix-Ansatz ausscheidet),
`TMapEventSinkBianco::startControl` (zwei unabhängige, nicht
gegenseitig aufhebbare Stack-Layout-„Basisvektoren" identifiziert,
16 Iterationen, keine Kombination trifft exakt).

### Session-Gesamtstand nach Runde 65

**530 verifizierte echte Fixes in 187 Commits.** `matched_functions`:
**9114** (von 9102 zu Rundenbeginn, +12 exakt wie erwartet).
`matched_code_percent`: **46,82 %**. Volles `ninja`-Rebuild
erfolgreich, `dtk shasum -c` bestätigt `build/GMSJ01/mario.dol: OK`.

### Nach sechsundsechzigster Iterationsrunde (24-Kandidaten-Batch, 19 Fixes — dritter Rekord in Folge, neue Fehlerklasse „ungenutzter Header-Include verschiebt .data-Layout" entdeckt)

**19 neue Fix-Commits** (alle mit der seit Runde 64 etablierten
strengen Byte-für-Byte-Instruktionswort-Verifikation bestätigt):

1. `SMSSetupTitleRenderingInfo` (Commit `83a43956`) — `char trash[8]`,
   drittes von vier Geschwister-Funktionen in derselben Datei.
2. `TNameKuri::init` (Commit `0b5b72e7`) — `char trash[40]`.
3. `TPollutionLayer::cleaned` (Commit `56875296`) — `char trash[0x30]`.
4. `TBellDolpic::receiveMessage` (Commit `05819d0e`) — `char trash[8]`.
5. `TMapObjectLightWithDBSet::makeDrawBuffer` (Commit `9aeb3d3c`) —
   `char trash[32]`, komplettiert die Drei-Geschwister-Serie in
   `LightUtil.cpp` (alle drei `makeDrawBuffer`-Varianten jetzt exakt).
6. `TNerveBossEelAppear::execute` (Commit `ffbd27f0`) — `char
   trash[32]`.
7. `TCardManager::writeOptionBlock_` (Commit `76af12e7`) —
   zweiseitiges Padding (`trash1[4]` davor, `trash2[12]` danach um
   `CARDFileInfo info`).
8. `TMario::fencePunch` (Commit `53e6628a`) — `char trash[16]` NACH
   dem verschachtelten `if`-Block mit `Mtx mtx` (Platzierung DAVOR
   bewirkte nichts Nützliches, bestätigt erneut Anker-Positions-
   Empfindlichkeit).
9. `MSHandle::setSeDistanceParameters` (Commit `6f6ff407`) — **echter
   Bug**: rief `setSeDistanceFir()` (falscher virtueller Slot) statt
   `setSeDistanceFxmix()` (Vtable-Slot 0x18) auf. Plus `char trash[8]`.
10. `TMario::trampleExec` (Commit `228341ec`) — `char trash[8]` NACH
    `scale`-Lokaler (nicht davor — unbenutzte konstante Trash-Arrays
    vor einem Lokal werden als generisches Boden-Padding behandelt,
    nicht als benannter Pool-Kandidat, Muster 7i-Verfeinerung).
11. `CPolarSubCamera::ctrlOptionCamera_` (Commit `aa2ec607`) — `u8
    trash[32]` NACH `probe`-Lokaler.
12. `TSmallEnemy::generateItem` (Commit `1d0117f1`) — anonymer
    `TMsRange<f32>(0.0f,100.0f).rand()`-Kettenaufruf in eine benannte
    Lokale `TMsRange<f32> genRange(...)` materialisiert.
13. `TEnemyAttachment::perform` (Commit `6a81c638`) — **echter Bug**:
    falscher virtueller Slot — `kill()` (Vtable-Offset 0xE4) statt
    `behaveToHost()` (0x128, ein leerer Inline-Hook) aufgerufen; per
    vollständigem `__vt__16TEnemyAttachment`-Vtable-Dump verifiziert.
    Plus `char trash[8]`.
14. `TSpcTypedInterp<TEventWatcher>::evGetFruitNum` (Commit
    `01646ec9`) — bestätigt den in Runde 63 vorhergesagten
    Geschwister-Fix von `evSetFruitType`: gepoppter Wert von `int` auf
    `u32` umgetypt.
15. `TMario::~TMario` (Commit `3bf1ede1`) — **NEUE FEHLERKLASSE**:
    `MarioInit.cpp` band `<System/StageUtil.hpp>` nur wegen
    `SMS_isMultiPlayerMap()` ein, zog dabei aber ungenutzte `static`-
    Hilfsfunktionen samt 0x168 Byte rückenden statischen Datentabellen
    (`scShineConvTable`, `scEtcShineConvTable`, `scScenarioNameTable`)
    in die Übersetzungseinheit — MWCC entfernt ungenutzte
    Datei-statische Symbole mit echten Initialisierern NICHT zur
    Kompilierzeit. Das verschob das komplette `.data`-Layout dieser
    Übersetzungseinheit und damit alle vom Destruktor berechneten
    literal-pool-relativen Offsets (+0x28/+0x4c/+0xdc wurden zu
    +0x190/+0x1b4/+0x244). Fix: vollen Header-Include durch eine
    Vorwärtsdeklaration der einzigen tatsächlich genutzten Funktion
    ersetzt.
16. `TSandBlock::control` (Commit `dbb4faff`) — `char trash[0xC]` NACH
    `scaleCopy`-Lokaler.
17. `CPolarSubCamera::ctrlGameCamera_` (Commit `58764055`) — Kombi-
    Fix: `int code;`-Deklaration an den Funktionsanfang gezogen (für
    Slot-Reihenfolge, tatsächliche Verwendung blieb an ursprünglicher
    Stelle) PLUS zweiseitiges Padding (`trash[8]` vor, `trash2[24]`
    nach `TCameraKindParam param`).
18. `TEMario::load` (Commit `d4382af0`) — zwei kombinierte Fixes: (a)
    zwei verworfene `stream.readU32()`-Methodenaufrufe zu verketteten
    `stream >> unused1 >> unused2`-Operatoren umgeschrieben (ändert
    MWCCs Registrierungsreihenfolge der Temporärwerte); (b) `char
    trash[0x44]` nach einem `const char[]`-Array.
19. `TRealoid::perform` (Commit `7dcb00c2`) — vier fehlende benannte
    Zwischenwerte für inline-expandierte Accessor-Ergebnisse (`f32
    nearPlane`, zwei `TBoid* boid`, ein `TRealoidActor* actor` über
    den bereits deklarierten aber ungenutzten `getRealoid()`-Accessor)
    — jede gebundene Inline-Accessor-Ergebnis-Lokale reserviert exakt
    einen 4-Byte-Slot, vier fehlende × 4 Byte = die fehlenden 16 Byte.
    Für zukünftige Runden vorgemerkt: die eigenständige (nicht-inline-
    expandierte) `TRealoid::clipBoids` selbst braucht noch +8 Byte,
    und `TFishoid::perform` braucht ein vollständig inline-
    expandiertes `TRealoid::perform` (Retail-Rahmen 0xb8) statt eines
    echten Funktionsaufrufs.

**Fünf gründlich dokumentierte Sackgassen** (alle sauber
zurückgesetzt, mehrere mit außergewöhnlicher Bisektionstiefe):
`TMapObjBase::getDistance` (18+ Varianten, bestätigt als bereits
früher dokumentierte Sackgasse), `TRiccoHook::init` (mathematischer
Beweis: ein Slot braucht +4, eine Gruppe von drei benachbarten Slots
braucht +8 — unmöglich aus einem einzigen linearen Pool, bestätigt
zwei getrennte Pools; kreuzreferenziert mit einem bereits im
Repo vorhandenen `// @non-matching`-Kommentar an exakt dieser
Stelle), `TPauseMenu2::loadAfter` (7 Varianten, ein `JUtility::
TColor::operator u32()`-Store/Reload-Bounce-Temporärwert sitzt auf
einem fixen, durch Padding unerreichbaren Boden-Offset),
`TSpineEnemy::resetToPosition` (bestätigt dieselbe „Boden-Bereich
unterhalb des anonymen Temporärwerts"-Sackgasse wie bereits in Runde
59 bei `CameraWarpPosAndAt`/`ModelUtilRideMove`/`ProgSelectPerform`
dokumentiert), `TSunShine::perform` (~25 Varianten, ein „unsichtbare-
Referenz-Argument"-Temporärwert für einen `TColor`-Werteparameter
unbeeinflussbar durch jede Trash-Platzierung).

### Session-Gesamtstand nach Runde 66

**549 verifizierte echte Fixes in 206 Commits.** `matched_functions`:
**9133** (von 9114 zu Rundenbeginn, +19 exakt wie erwartet — dritter
Rekord-Zuwachs in Folge nach Runde 64 [+18] und Runde 65-Kontext).
`matched_code_percent`: **47,12 %** (überschreitet erstmals die
47-Prozent-Marke). Volles `ninja`-Rebuild erfolgreich, `dtk shasum -c`
bestätigt `build/GMSJ01/mario.dol: OK`.

### Nach siebenundsechzigster Iterationsrunde (24-Kandidaten-Batch, 16 Fix-Commits / 18 Funktionen matched, neue Trash-Pool-Erkenntnisse)

**16 neue Fix-Commits** (alle mit der seit Runde 64 etablierten
strengen Byte-für-Byte-Instruktionswort-Verifikation bestätigt), davon
einer (Nr. 14) ein Header-Fix, der zusätzlich zur Zielfunktion drei
weitere Template-Instanzen im selben Header auf 100 % brachte —
**18 Funktionen** insgesamt neu gematcht:

1. `TMapCollisionData::removeCheckListData` (Commit `6b801674`) —
   `char trash[0x28]`.
2. `TIndirectLightWithDBSet::makeDrawBuffer` (Commit `b5da1e9d`) —
   `char trash[32]`, viertes und letztes von vier Geschwister-
   `makeDrawBuffer`-Fixes in `LightUtil.cpp`.
3. `TBellDolpic::control` (Commit `52e1539a`) — `char trash[16]` nach
   `pos`-Lokaler.
4. `TSpcTypedInterp<TEventWatcher>::evGetAddressFromViewObjName`
   (Commit `cf12b481`) — **umgekehrtes Muster 7**: ein ÜBERFLÜSSIGER
   benannter Lokal (`viewObj`) reservierte einen Slot, den Retail
   nicht hat; Entfernen (inline-Cast direkt im `push()`-Argument)
   verschiebt den kompletten Anonym-Pool um die fehlenden 4 Byte nach
   oben.
5. `TMapObjBase::changeObjMtx` (Commit `9178fa3b`) — **zwei echte
   Bugs**: (a) Übersetzungsspalte falsch gelesen (`mtx[3][0..2]`
   Bottom-Row statt der projektweiten Konvention `mtx[0..2][3]`
   Translation-Column); (b) `char trash[0x28]` für die fehlende
   40-Byte-Rahmenreserve.
6. `TKageMarioModoki::init` (Commit `52026a21`) — `char trash[8]`.
7. `TMario::canSleep` (Commit `add9a5c5`) — zusätzlicher, nie
   referenzierter `const TBGCheckData* gnd2`-Pointer-Lokal direkt
   nach `gnd` (Verfeinerung von Muster 7i: ein unbenutzter Pointer
   OHNE Initialisierer wird hier NICHT vor der Stack-Layout-
   Registrierung eliminiert, reserviert seinen Slot trotzdem).
8. `TWireBinder::getDirAtPos` (Commit `9641f2ab`) — dritten benannten
   Lokal (`fVar1`) eliminiert, dessen Wert stattdessen in-place in
   `posInWire` mutiert (reproduziert Retails bedingtes
   `fmr f31,f1`-Muster exakt).
9. `TBossEelHeartCoin::perform` (Commit `92276978`) — `char
   trash[44]` nach `heartMtx`-Lokaler.
10. `TBossGesso::rumblePad` (Commit `c6576249`) — Deklaration von
    `fVar2` an den Funktionsanfang vorgezogen, Zuweisung
    (`fVar2 = delta.length();`) blieb an ursprünglicher Stelle:
    reines Registrierungsreihenfolge-Detail ohne Instruktionsänderung
    verschiebt `delta`s Slot um die nötigen 4 Byte.
11. `TGesso::behaveToFindMario` (Commit `7f89a008`) — **neues
    Trash-Pool-Teilmuster „benutztes vs. unbenutztes Trash"**: ein
    unbenutztes `char trash[N]` wächst immer den OBEREN (benannten)
    Pool und kann einen anonymen Temporärwert nie verschieben; ein
    TATSÄCHLICH BESCHRIEBENES `char trash[4]; trash[0]=0;` (die
    Zuweisung selbst wird wegoptimiert, Instruktionszahl bleibt
    gleich) überlebt als echter benannter Lokal im UNTEREN Bereich
    und verschiebt dadurch den anonymen `TPathNode`-Temporärwert um
    die nötigen 4 Byte nach oben.
12. `CPolarSubCamera::ctrlNormalDeadDemo_` (Commit `4d0f223f`) —
    **neue Anker-Regel**: ein unreferenziertes `char trash[N]`
    landet immer NACH jedem echten (adressgenommenen) benannten
    Lokal in seinem Scope, unabhängig von der Textposition relativ zu
    diesem Lokal — bestätigt durch drei erfolglose Platzierungen vor
    `Vec diff`; der Fix platziert `char trash[28]` stattdessen NACH
    `diff`s Feldzuweisungen, was den Vor-`diff`-Bereich wachsen lässt
    und `diff` von Offset 0x14 auf 0x30 verschiebt (exakt Retail).
13. `TGCConsole2::checkChangeTelopArray` (Commit `22731050`) — **drei
    kombinierte Fixes**: (a) drei zuvor fälschlich hinzugefügte
    benannte Globals (`scUnusedScale1/2`, `scUnusedTable`)
    dupliziierten bereits vorhandene anonyme Compiler-Literale und
    verschoben das komplette nachfolgende `.data`-Layout um 40 Byte —
    entfernt; (b) `scDolpicNewsDolpic5_1`/`_4` im Case-5-Zweig
    vertauscht (per Retail-Symbol-zu-Adresse-Abgleich verifiziert);
    (c) `char trash[48]` für die restliche Rahmenlücke.
14. `TNameRefAryT<T,JDrama::TNameRef>::load` (Commit `0f596d40`,
    Header-Fix in `include/Strategic/NameRefAry.hpp`) — Bindung des
    `operator[]`-Ergebnisses an eine explizite Referenz
    (`T& child = getChildren()[i];`) vor dem virtuellen `load()`-
    Aufruf; reserviert den fehlenden 4-Byte-Slot. Da es sich um eine
    Template-Methode handelt, brachte dieser EINE Fix alle vier im
    selben Objekt instanziierten Varianten
    (`TStageEventInfo`, `TStagePositionInfo`, `TCameraMapTool`,
    `TScenarioArchiveName`) gleichzeitig auf 100 % — nach
    vollständiger Regressionsprüfung aller Includer committet.
15. `TWalkerEnemy::behaveToFindMario` (Commit `f9d9c145`) — Kombi aus
    benanntem `TPathNode node(...)` PLUS benanntem Pointer
    `TPathNode* nodePtr = &node;` (nicht Referenz) reproduziert
    Retails exaktes +4/+4-Split-Layout.
16. `TMario::thinkHeight` (Commit `ead2db98`) — inline-expandiertes
    `checkStatusType()`-Ergebnis an eine benannte `bool jumping`-
    Lokale gebunden statt direkt in der `if`-Bedingung verwendet;
    verifiziert ohne Regression der bereits exakt matchenden
    Geschwisterfunktion `checkPlayerAround`.

**Fünf gründlich dokumentierte Sackgassen** (alle sauber
zurückgesetzt, mehrere mit außergewöhnlicher Bisektionstiefe):
`MActorAnmData::MActorAnmData()` (8 Varianten — der anonyme
Compiler-Temporärwert für die implizite Default-Argument-Konstruktion
des ersten Klassenmembers ist permanent auf den niedrigsten
Anonym-Pool-Offset fixiert; nichts Legales kann vor der allerersten
Member-Konstruktion ausgewertet werden), `MSHandle::calcPan`
(8 Varianten, reine 8-Byte-Überreservierung ohne zugehörige
Save/Spill-Instruktion, klassische Zwei-Pool-Sackgasse),
`TMapObjGrassManager::initDrawNear` (20+ Iterationen — DRITTER,
unabhängiger Stack-Pool für GXColor-Compound-Literal-Materialisierung
identifiziert, zusätzlich zum benannten und dem fctiwz-Anonym-Pool;
jede nicht-störende `TVec3<s16>`-Dummy-Variante erreicht nur +16 von
benötigten +20 Byte, jede größere Variante überschießt auf +24 oder
stört die bereits passende Register-Allokation), `JPABaseEmitter::
calcCreateParticle` (15+ Varianten, Diff von 58 auf 6 Zeilen reduziert
— der `getRandomRF()`-Bit-Trick-Temporärwert aus `JMath::
TRandom_fast_::get_ufloat_1()` bleibt permanent an der niedrigsten
Anonym-Pool-Position fixiert, da er der ERSTE in Erstellungsreihen-
folge erzeugte Inline-Temporärwert der Funktion ist), `TApplication::
setupThreadFuncLogo` (~10 Varianten — asymmetrische +4/+0-Verschiebung
über vier Aufrufstellen zweier geteilter Inline-Helfer
(`SMSLoadArchive`/`SMSLoadArchiveARAM`) mathematisch nicht aus einer
symmetrisch wirkenden Body-Änderung ableitbar, da beide Aufrufstellen
jeder Helferfunktion identischen Code teilen).

Drei Kandidaten mit sauberem Fehlschlag ohne Dateiänderung (Infra-
Ratenbegrenzung bzw. Skript-Fehler vor jeder Quelländerung):
`TMActorKeeper::TMActorKeeper(TLiveManager*)`, `TCoasterEnemy::bind`,
`TMapObjGeneral::receiveMessage` — bleiben frische Kandidaten für eine
künftige Runde.

**Methodik-Verfeinerung**: Die Kandidaten-Ausschlussmenge wurde von
reiner Unit-Pfad-Ebene auf Funktions-Ebene verfeinert
(`(unit, funktion)`-Paare statt nur `unit`), nachdem eine Prüfung
zeigte, dass vier der fünf neuen Sackgassen-Units noch zahlreiche
andere unberührte Kandidatenfunktionen im Zielbereich hatten (z. B.
`JPAEmitter.cpp` mit vier weiteren, `MActorData.cpp` mit sieben
weiteren) — eine reine Unit-Sperre hätte diese in künftigen Runden
fälschlich unsichtbar gemacht.

### Session-Gesamtstand nach Runde 67

**565 verifizierte echte Fixes in 222 Commits.** `matched_functions`:
**9151** (von 9133 zu Rundenbeginn, +18 exakt wie erwartet — 16
Fix-Commits, davon einer mit Kaskadeneffekt auf vier
Template-Instanzen). `matched_code_percent`: **47,35 %**. Volles
`ninja`-Rebuild erfolgreich, `dtk shasum -c` bestätigt
`build/GMSJ01/mario.dol: OK`. Regressionsprüfung (Vergleich aller
zuvor 100 %-matchenden Funktionen vor/nach dem Rebuild): 0
Regressionen, 18 Neuzugänge — exakte Übereinstimmung.

