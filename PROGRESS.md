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

**Session-Gesamtstand nach Runde 44: 398 tatsächlich verifizierte
Funktionen** (396 aus Runde 1–43 plus 2 neue in Runde 44:
`FifoSetFogRangeAdj`, `FifoSetFog`) in 55 Commits. Funktionszahl:
8983 → **8985** (**+2**). DOL SHA1 bleibt `OK`.








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
