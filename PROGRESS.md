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

## Nächster GMSJ01-Kandidat

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
